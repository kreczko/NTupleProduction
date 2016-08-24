"""
    ntp run condor merge:
        Just a dummy command to be copied & pasted when you create a new
        command.
        
    Usage:
        run condor analysis dataset=<data set> campaign=<campaign>
        
    Parameters:
        something: a weird parameter
"""
import logging
import os

from .. import Command as C
from ntp.commands.setup import WORKSPACE, LOGDIR
from ntp import NTPROOT

LOG = logging.getLogger(__name__)

try:
    import htcondenser as htc
except:
    LOG.error('Could not import htcondenser')

CONDOR_ROOT = os.path.join(WORKSPACE, 'condor')
RETRY_COUNT = 2
PREFIX = __name__.split('.')[-1]

SETUP_SCRIPT = """
tar -xf ntp.tar.gz
source bin/env.sh
ntp setup from_tarball=cmssw_src.tar.gz compile=0

"""

RUN_SCRIPT = """
ntp merge $@

"""

SETUP_SCRIPT_FILE = '{0}_setup.sh'.format(PREFIX)
RUN_SCRIPT_FILE = '{0}_run.sh'.format(PREFIX)
CONFIG_FILE = '{0}_config.json'.format(PREFIX)


LOG_STEM = '{0}_job.$(cluster).$(process)'.format(PREFIX)
OUT_FILE = LOG_STEM + '.out'
ERR_FILE = LOG_STEM + '.err'
LOG_FILE = LOG_STEM + '.log'


class Command(C):

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self, args, variables):
        self.__prepare(args, variables)
        self.__text = "NOT IMPLEMENTED"

        return True

    def create_job_layer(self, input_files, mode):
        config = self.__config

        hdfs_store = config['outLFNDirBase'].replace('ntuple', 'atOutput')
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

        parameters = '{files} output_file={output_file}'
        output_file = '{0}.root'.format(config['outputDatasetTag'])

        args = parameters.format(
            files=' '.join(input_files),
            output_file=output_file,
        )
        rel_log_dir = os.path.relpath(LOGDIR, NTPROOT)
        rel_log_file = os.path.join(rel_log_dir, 'ntp.log')
        job = htc.Job(
            name='{0}_{1}_job'.format(PREFIX, mode),
            args=args,
            output_files=[output_file, rel_log_file])
        job_set.add_job(job)

        return [job]

    def write_job_files(self):
        with open(self.__run_script, 'w+') as f:
            f.write(RUN_SCRIPT)

        with open(self.__setup_script, 'w+') as f:
            f.write(SETUP_SCRIPT)

    def __set_job_dir(self, campaign, dataset):
        out_dir = os.path.join(CONDOR_ROOT, campaign, dataset, PREFIX)
        out_dir = self.__get_latest_outdir(out_dir)

        self.set_job_dir(out_dir)

    def set_job_dir(self, job_dir):
        self.__job_dir = job_dir
        self.__outdirs.append(job_dir)
        self.__job_log_dir = os.path.join(self.__job_dir, 'log')

        self.__setup_script = os.path.join(self.__job_dir, SETUP_SCRIPT_FILE)
        self.__run_script = os.path.join(self.__job_dir, RUN_SCRIPT_FILE)
        self.__run_config = os.path.join(self.__job_dir, CONFIG_FILE)
