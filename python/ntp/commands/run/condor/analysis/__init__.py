"""
    run condor analysis: Runs the n-tuple production on the a condor batch system.
                All run commands require a valid grid certificate as they
                either read data from the grid via XRootD or run on grid
                resources.
                The command will use python/run/miniAODToNTuple_cfg.py.
                All unknown parameters will be bassed to 'ntp run local'
                inside the condor job.
        Usage:
            run condor analysis [campaign=<X>]  [dataset=<X>] [file=<path>]

        Parameters:
            campaign: which campaign to run. Corresponds to the folder
                      structure in crab/*
                      Default is 'Spring16'.
            dataset:  Alias for the single dataset you want to run over. Corresponds
                      to the file names (without extension) in crab/*/*.py.
                      Accepts wild-cards and comma-separated lists.
                      Default is 'TTJets_PowhegPythia8'.
                      Special value 'all' will submit all datasets for a given
                      campaign.
                      This parameter is ignored if parameter file is given.
            files:    Instead of running on a specific dataset, run over the
                      given (comma-separated) list of files
            noop:     'NO OPeration', will do everything except submitting jobs.
                      Default: false
            nevents:  Number of events to process.
                      Default is all (-1).
            test:     Run just one job for testing. Default: false.
            search_path: path to search for ntuples. Default: not set   
"""
from __future__ import print_function
import os
import getpass
import logging
import glob

from .. import Command as C
from ntp.utils.hdfs import HDFS_STORE_BASE
from crab.util import get_files
from ntp import NTPROOT
from ntp.commands.setup import WORKSPACE, LOGDIR, CACHEDIR, RESULTDIR
from ntp.utils import make_even_chunks, find_latest_iteration

LOG = logging.getLogger(__name__)

try:
    import htcondenser as htc
except:
    LOG.error('Could not import htcondenser')

CONDOR_ROOT = os.path.join(WORKSPACE, 'condor')
RETRY_COUNT = 10
PREFIX = __name__.split('.')[-1]

SETUP_SCRIPT = """
tar -xf ntp.tar.gz
source bin/env.sh
ntp setup from_tarball=cmssw_src.tar.gz

"""

RUN_SCRIPT = """
ntp run analysis $@ nevents=0

"""

SETUP_SCRIPT_FILE = '{0}_setup.sh'.format(PREFIX)
RUN_SCRIPT_FILE = '{0}_run.sh'.format(PREFIX)
CONFIG_FILE = '{0}_config.json'.format(PREFIX)

LOG_STEM = '{0}_job.$(cluster).$(process)'.format(PREFIX)
OUT_FILE = LOG_STEM + '.out'
ERR_FILE = LOG_STEM + '.err'
LOG_FILE = LOG_STEM + '.log'

ANALYSIS_MODES = [
    'central',
    'JES_down',
    'JES_up',
    'JetSmearing_down',
    'JetSmearing_up',
]


# file splitting for datasets containing 'key'
SPLITTING_BY_FILE = {
    'SingleElectron': 3,
    'SingleMuon': 3,
    'TTJet': 5,
    'TT_': 5,
    'DEFAULT': 10,
}

# Analysis jobs: 1 file = 17s processing time
N_FILES_PER_ANALYSIS_JOB = 50  # ~= 14 min


class Command(C):

    DEFAULTS = {
        'campaign': 'Spring16',
        'dataset': 'TTJets_PowhegPythia8',
        'files': '',
        'noop': False,
        'nevents': -1,
        'test': False,
        'search_path': '',
    }

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self, args, variables):
        self.__prepare(args, variables)

        campaign = self.__variables['campaign']
        chosen_dataset = self.__variables['dataset']

        # create tarball
        self.__create_tar_file(args, variables)

        datasets = [chosen_dataset]
        if chosen_dataset.lower() == 'all':
            from crab.datasets import create_sample_list
            samples = create_sample_list()
            if campaign in samples:
                datasets = samples[campaign]
            else:
                LOG.error(
                    'Cannot find datasets for campaign {0}'.format(campaign))
                return False

        for dataset in datasets:
            self.__run_dataset(campaign, dataset)
        # to check status:
        msg = 'To check the status you can run\n'
        if len(self.__outdirs) == 1:
            msg += 'DAGstatus {0}/*.status -s'.format(self.__outdirs[0])
        else:
            msg += 'DAGstatus workspace/condor/*/*.status -s'
        LOG.info(msg)

        return True

    def __run_dataset(self, campaign, dataset):
        self.__set_job_dir(campaign, dataset)

        self.__create_folders()

        # which dataset, file, etc
        self.create_config(campaign, dataset)
        # create DAG for condor
        self.__create_dag()

        if not self.__variables['noop']:
            self.__dag.submit(force=True)
            self.__text += "\n Submitted {0} jobs".format(len(self.__dag))
#             for job in self.__dag:
#                 print(job.name, 'running:', job.manager.exe, ' '.join(job.args))

    def create_config(self, campaign, dataset):
        self.__config = self.__get_crab_config(campaign, dataset)

        self.__config['unitsPerJob'] = N_FILES_PER_ANALYSIS_JOB

        search_path = self.__variables['search_path']
        output_dir = self.__config['outLFNDirBase']
        path = search_path if search_path else output_dir
        self.__find_ntuple_files(path)

        self.__config['outputDir'] = output_dir.replace(
            'ntuple', 'atOutput'
        )

    def __find_ntuple_files(self, path):
        try_suffixes = ['', 'tmp', 'ntuple_job_*']

        for s in try_suffixes:
            path = os.path.join(path, s)
            search_path = os.path.join(path, '*.root')
            files = glob.glob(search_path)
            if files:
                self.__config['files'] = files
                break

    def __set_job_dir(self, campaign, dataset):
        out_dir = os.path.join(CONDOR_ROOT, campaign, dataset, PREFIX)
        out_dir = self.__get_latest_outdir(out_dir)

        self.set_job_dir(out_dir)

    def set_job_dir(self, job_dir):
        self.__job_dir = job_dir
        self.__outdirs.append(job_dir)
        self.__job_log_dir = os.path.join(self.__job_dir, 'log')
        self.__setup_script = os.path.join(
            self.__job_dir, SETUP_SCRIPT_FILE)
        self.__run_script = os.path.join(self.__job_dir, RUN_SCRIPT_FILE)
        self.__run_config = os.path.join(self.__job_dir, CONFIG_FILE)

    def write_job_files(self):
        with open(self.__setup_script, 'w+') as f:
            f.write(SETUP_SCRIPT)

        with open(self.__run_script, 'w+') as f:
            f.write(RUN_SCRIPT)

        import json
        with open(self.__run_config, 'w+') as f:
            f.write(json.dumps(self.__config, indent=4))

    def __create_dag(self):
        """ Creates a Directional Acyclic Grag (DAG) for condor """
        dag_file = os.path.join(self.__job_dir, '{0}.dag'.format(PREFIX))
        dag_status = os.path.join(self.__job_dir, '{0}.status'.format(PREFIX))
        dot_file = '{0}.dot'.format(PREFIX)
        dag_man = htc.DAGMan(
            filename=dag_file, status_file=dag_status, dot=dot_file,
        )

        input_files = self.__config['files']
        # layer 2 - analysis
        for mode in ANALYSIS_MODES:
            analysis_jobs = self.create_job_layer(input_files, mode)
            for job in analysis_jobs:
                dag_man.add_job(job, retry=RETRY_COUNT)

        self.__dag = dag_man

        self.write_job_files()

    def create_job_layer(self, input_files, mode):
        jobs = []
        self.__root_output_files = []

        config = self.__config
        if self.__variables['test']:
            input_files = [input_files[0]]
        self.__config['files'] = input_files

        hdfs_store = config['outputDir']
        job_set = htc.JobSet(
            exe=self.__run_script,
            copy_exe=True,
            setup_script=self.__setup_script,
            filename=os.path.join(self.__job_dir, '{0}.condor'.format(PREFIX)),
            out_dir=self.__job_log_dir,
            out_file=OUT_FILE,
            err_dir=self.__job_log_dir,
            err_file=ERR_FILE,
            log_dir=self.__job_log_dir,
            log_file=LOG_FILE,
            share_exe_setup=True,
            common_input_files=self.__input_files,
            transfer_hdfs_input=False,
            hdfs_store=hdfs_store,
            certificate=self.REQUIRE_GRID_CERT,
            cpus=1,
            memory='1500MB'
        )

        parameters = 'files={files} output_file={output_file} mode={mode}'

        n_files_per_group = N_FILES_PER_ANALYSIS_JOB
        grouped_files = make_even_chunks(input_files, n_files_per_group)

        for i, f in enumerate(grouped_files):
            output_file = '{dataset}_atOutput_{mode}_{job_number}.root'.format(
                dataset=config['outputDatasetTag'],
                mode=mode,
                job_number=i
            )

            args = parameters.format(
                files=','.join(f),
                output_file=output_file,
                mode=mode,
            )
            rel_out_dir = os.path.relpath(RESULTDIR, NTPROOT)
            rel_log_dir = os.path.relpath(LOGDIR, NTPROOT)
            rel_out_file = os.path.join(rel_out_dir, output_file)
            rel_log_file = os.path.join(rel_log_dir, 'ntp.log')
            job = htc.Job(
                name='{0}_{1}_job_{2}'.format(PREFIX, mode, i),
                args=args,
                output_files=[rel_out_file, rel_log_file])
            job_set.add_job(job)
            jobs.append(job)

        return jobs
