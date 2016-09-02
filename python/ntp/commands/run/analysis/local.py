"""
    run analysis where=local:
        Runs the ntuple based analysis code (AnalysisSoftware) to produce the
        final set of trees on the current machine.
"""
from __future__ import print_function
import os
import logging
import glob
import shutil

from .. import Command as C
from ntp.interpreter import time_function
from ntp import NTPROOT
from ntp.commands.setup import CMSSW_SRC, TMPDIR, RESULTDIR, LOGDIR
from crab.util import find_input_files
from . import BASE

BAT_BASE = os.path.join(CMSSW_SRC, 'BristolAnalysis', 'Tools')
BAT_PYTHON = os.path.join(BAT_BASE, 'python')
LOG = logging.getLogger(__name__)
PSET = os.path.join(TMPDIR, 'bat.py')
OUTPUT_FILE = os.path.join(RESULTDIR, 'atOutput.root')


class Command(C):

    DEFAULTS = {
        'campaign': 'Test',
        'dataset': 'TTJets_PowhegPythia8',
        'nevents': 1000,
        'files': '',
        'noop': False,
        'output_file': OUTPUT_FILE,
        'pset_template': BASE,
        'mode': 'central',
    }

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    @time_function('run local', LOG)
    def run(self, args, variables):
        if 'output_file' in variables:
            output_file = os.path.join(RESULTDIR, variables['output_file'])
            if not output_file.endswith('.root'):
                output_file += '.root'
            variables['output_file'] = output_file

        self.__prepare(args, variables)
#         campaign = self.__variables['campaign']
        chosen_dataset = self.__variables['dataset']
#         from BristolAnalysis.Tools.analysis_info import datasets_13TeV
        analysis_info_file = os.path.join(BAT_PYTHON, 'analysis_info.py')
        from imp import load_source
        analysis_info = load_source('analysis_info', analysis_info_file)
        datasets_13TeV = analysis_info.datasets_13TeV
        if not chosen_dataset in datasets_13TeV and variables['files'] == "":
            LOG.error('Cannot find dataset {0}'.format(chosen_dataset))
            return False

        input_files = []
        if variables['files'] != "":
            input_files = variables['files'].split(',')
            input_files = [os.path.abspath(f) for f in input_files]
        else:
            input_files = datasets_13TeV[chosen_dataset]
        LOG.debug(
            "Using files for NTP input:\n{0}".format('\n'.join(input_files)))

        self.__output_file = self.__variables['output_file']

        self.__write_pset(input_files)

        # making sure the correct HLT menu is read
        if 'reHLT' in input_files[0]:
            self.__variables['isReHLT'] = 1

        if not self.__variables['noop']:
            code = self.__run_analysisSoftware(
                chosen_dataset, self.__variables['mode'])
            self.__text = "Ran {PSET}\n"
            self.__text += "Logging information can be found in {LOGDIR}/ntp.log\n"
            if code == 0:
                self.__text += "Created atOutput: {OUTPUT_FILE}\n"
                self.__text = self.__text.format(
                    PSET=PSET, LOGDIR=LOGDIR, OUTPUT_FILE=self.__output_file)
            else:
                self.__text += "BAT experienced an error,"
                self.__text += " return code: {code}\n"
                self.__text = self.__text.format(
                    PSET=PSET, LOGDIR=LOGDIR, code=code)
                if code == 139:
                    LOG.warning(
                        '########################################################')
                    LOG.warning(
                        '####  Ignoring segault (hopefully) at the end of AS ####')
                    LOG.warning(
                        '########################################################')
                    return True
                return False

        else:
            LOG.info('Found "noop", not running CMSSW')

        return True

    def __write_pset(self, input_files):
        nevents = int(self.__variables['nevents'])
        input_files = self.__format_input_files(input_files)

        with open(PSET, 'w+') as f:
            content = self.__variables['pset_template'].format(
                nevents=nevents,
                input_files=input_files,
                BAT_BASE=BAT_BASE,
                maxEvents=nevents,
                mode='central',
                dataset=self.__variables['dataset'],
                #                 OUTPUT_FILE=self.__output_file,
            )
            f.write(content)

    @time_function('__run_analysisSoftware', LOG)
    def __run_analysisSoftware(self, dataset, mode):
        commands = [
            'cd {CMSSW_SRC}',
            'source /cvmfs/cms.cern.ch/cmsset_default.sh',
            'eval `/cvmfs/cms.cern.ch/common/scram runtime -sh`',
            'sample="{dataset}" analysisMode="{mode}" BAT {PSET} {params}',
        ]

        all_in_one = ' && '.join(commands)
        all_in_one = all_in_one.format(
            CMSSW_SRC=CMSSW_SRC,
            dataset=dataset,
            mode=mode,
            PSET=PSET,
            params=self.__extract_params()
        )

        LOG.info("Executing BAT")
        from ntp.interpreter import call
        code, _, _ = call(
            [all_in_one], LOG, stdout_log_level=logging.INFO, shell=True)
        self.__move_output_files()

        return code

    def __move_output_files(self):
        output_files = glob.glob(
            '{CMSSW_SRC}/*.root'.format(CMSSW_SRC=CMSSW_SRC))

        output_file = self.__output_file
        for f in output_files:
            if 'tree_' in f:
                LOG.debug('Moving {0} -> {1}'.format(f, output_file))
                shutil.move(f, output_file)
            else:
                LOG.debug('Removing obsolete file: {0}'.format(f))
                os.remove(f)
