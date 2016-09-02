"""
    run analysis:
        Runs the ntuple based analysis code (AnalysisSoftware) to produce the
        final set of trees.
        
    Usage:
        run analysis dataset=<data set> [files=<f1,f2,..>]
                    [nevents=1000] [noop=0] [output_file=atOutput.root]
        
    Parameters:
        dataset:  Alias for the single dataset you want to run over. Corresponds
                  to the file names (without extension) in crab/*/*.py.
                  Accepts wild-cards and comma-separated lists.
                  Default is 'TTJets_PowhegPythia8'.
                  This parameter is ignored if parameter file is given.
        files:    Instead of running on a specific dataset, run over the
                  given (comma-separated) list of files
        nevents:  Number of events to process.
                  Default is 1000.
        noop:     'NO OPeration', will not run AnalysisSoftware. Default: false
        output_file: Name of the output file. Default: atOutput.root
        where:    Where to run the analysis. Can be 'local|DICE'.
                  Default is 'local'.
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

BAT_BASE = os.path.join(CMSSW_SRC, 'BristolAnalysis', 'Tools')
BAT_PYTHON = os.path.join(BAT_BASE, 'python')
LOG = logging.getLogger(__name__)
PSET = os.path.join(TMPDIR, 'bat.py')
OUTPUT_FILE = os.path.join(RESULTDIR, 'atOutput.root')

ANALYSIS_MODES = [
    'central',
    'JES_down',
    'JES_up',
    'JetSmearing_down',
    'JetSmearing_up',
]

BASE = """
import os
import sys
from copy import deepcopy
from imp import load_source
dirname, _ = os.path.split(os.path.abspath(__file__))
analysis_info = load_source( 'analysis_info', '{BAT_BASE}/python/analysis_info.py' )

mc_path = analysis_info.mc_path_13TeV
data_path = analysis_info.data_path_13TeV
datasets = analysis_info.datasets_13TeV

analysisModes = analysis_info.analysis_modes_13TeV

available_settings = [
    'ElectronScaleFactorSystematic', 
    'MuonScaleFactorSystematic', 
    'JESsystematic', 
    'JetSmearingSystematic', 
    'PUsystematic', 
    'BTagSystematic', 
    'LightTagSystematic', 
    'custom_file_suffix'
]

default_settings = {{
    'ElectronScaleFactorSystematic':0,
    'MuonScaleFactorSystematic':0,
    'JESsystematic':0,
    'JetSmearingSystematic':0,
    'PUFile':'PileUp_2015_truth_central.root',
    'PUFile_up':'PileUp_2015_truth_up.root',
    'PUFile_down':'PileUp_2015_truth_down.root',
    'MuonIdIsoScaleFactorsFile':'nofile.root',
    'TTbarLikelihoodFile' : 'LikelihoodInputAnalyserOutput.root',
    'BTagEfficiencyFile' : 'BTagEfficiency.root',
    'BTagSystematic':0,
    'LightTagSystematic':0,
    'custom_file_suffix':'',
    'pdfWeightNumber' : 0,
}}

analysis_settings = {{
    'Electron_down':{{'ElectronScaleFactorSystematic':-1}},
    'Electron_up':{{'ElectronScaleFactorSystematic':1}},
    'Muon_down':{{'MuonScaleFactorSystematic':-1}},
    'Muon_up':{{'MuonScaleFactorSystematic':1}},
    'BJet_down':{{'BTagSystematic':-1}},
    'BJet_up':{{'BTagSystematic':1}},
    'JES_down':{{'JESsystematic':-1}},
    'JES_up':{{'JESsystematic':1}},
    'JetSmearing_up':{{'JetSmearingSystematic':1}},
    'JetSmearing_down':{{'JetSmearingSystematic':-1}},
    'LightJet_down':{{'LightTagSystematic':-1}},
    'LightJet_up':{{'LightTagSystematic':1}},
    'PU_down':{{ 'PUFile':'PileUp_2015_truth_down.root', 'custom_file_suffix':'PU_down' }},
    'PU_up':{{'PUFile':'PileUp_2015_truth_up.root', 'custom_file_suffix':'PU_up' }},
    'Test': {{'custom_file_suffix':'TESTING'}}
}}

#helper functions
def getAnalysisSettings(analysisMode):
    if not analysisMode in analysis_settings.keys():
        return default_settings
    
    settings = deepcopy(default_settings)
    settings.update(analysis_settings[analysisMode])
    return settings


#config start
#number of events to be processed
maxEvents = {nevents}# 0 == all

toolsFolder = "{BAT_BASE}"

sample = "{dataset}"
analysisMode = "{mode}"
    
settings = getAnalysisSettings(analysisMode)
if sample in ['TTJets-mcatnlo','TTJets-powheg']:
    suffixes = {{'TTJets-mcatnlo':'MCatNLO','TTJets-powheg':'POWHEG'}}
    if settings['custom_file_suffix'] == "":
        settings['custom_file_suffix'] = suffixes[sample]
    else:
        settings['custom_file_suffix'] = suffixes[sample] + '_' + settings['custom_file_suffix']    


# Option to process a single ntuple of a sample rather than all of them
ntupleToProcess = -1

#File for pile-up re-weighting
PUFile = "{BAT_BASE}/data/" + settings['PUFile']
PUFile_up = "{BAT_BASE}/data/" + settings['PUFile_up']
PUFile_down = "{BAT_BASE}/data/" + settings['PUFile_down']
getMuonScaleFactorsFromFile = True
getElectronScaleFactorsFromFile = True
ElectronIdScaleFactorsFile = '{BAT_BASE}/data/ElectronCutBasedID_MediumWP_76X_SF2D.root'
ElectronIsoScaleFactorsFile = '{BAT_BASE}/data/Elec_SF_Fit_Syst.root'
ElectronTriggerScaleFactorsFile = '{BAT_BASE}/data/ElectronTriggerEfficiencies.root'
MuonIdScaleFactorsFile = '{BAT_BASE}/data/MuonID_Z_RunCD_Reco76X_Feb15.root'
MuonIsoScaleFactorsFile = '{BAT_BASE}/data/MuonIso_Z_RunCD_Reco76X_Feb15.root'
MuonTriggerScaleFactorsFile = '{BAT_BASE}/data/SingleMuonTrigger_Z_RunCD_Reco76X_Feb15.root'
getHadronTriggerFromFile = True
hadronTriggerFile = ''
ElectronScaleFactorSystematic = settings['ElectronScaleFactorSystematic']
MuonScaleFactorSystematic = settings['MuonScaleFactorSystematic']

TTbarLikelihoodFile = "{BAT_BASE}/data/" + settings['TTbarLikelihoodFile']
BTagEfficiencyFile = "{BAT_BASE}/data/" + settings['BTagEfficiencyFile']

#JES Systematic, the +/- number of uncertainties to vary the jets with
JESsystematic = settings['JESsystematic']
JetSmearingSystematic = settings['JetSmearingSystematic']
BTagSystematic = settings['BTagSystematic']
LightTagSystematic = settings['LightTagSystematic']
custom_file_suffix = settings['custom_file_suffix']

if ntupleToProcess > 0 :
    filetype = '*%03d.root' % ntupleToProcess
    print 'Will only consider ntuple : ',filetype
    settings['custom_file_suffix'] += str(ntupleToProcess)
    custom_file_suffix = settings['custom_file_suffix']

inputFiles = inputFiles = [
{input_files},
]

print 'Parsed config settings:'
for setting,value in settings.iteritems():
    print setting, '=', value


#Jet Energy Resolutions files (L7 corrections)
bJetResoFile = "{BAT_BASE}/data/bJetReso.root"
lightJetResoFile = "{BAT_BASE}/data/lightJetReso.root"

#Jet Smearing application
applyJetSmearing = True

#Apply Top Pt reweighting
applyTopPtReweighting = False

#use HitFit for analysis
useHitFit = False
produceFitterASCIIoutput = False

#MET corrections application
applyMetSysShiftCorr = False
applyMetType0Corr = False

#relative Path from calling BAT to the TopQuarkAnalysis folder
TQAFPath = ""

#integrated luminosity the MC simulation will be scaled to
lumi = 15933 
#this value will be part of the output file name: DataType_CenterOfMassEnergyTeV_lumipb-1_....
centerOfMassEnergy = 13

#file with information (cross-section, number of processed events) for event weight calculation
datasetInfoFile = ""
if centerOfMassEnergy == 13:
    datasetInfoFile = "{BAT_BASE}/python/DataSetInfo_13TeV_25ns.py"


nTuple_version = 0

"""


class Command(C):
    #     """
    #         This command is used for input file discovery & steering of anlysis
    #         jobs. Once all arguments and parameters are passed this commands calls
    #         one of its sub-commands, local or DICE, to execute the job in the
    #         correct environment.
    #     """

    DEFAULTS = {
        'dataset': 'TTJets_PowhegPythia8',
        'nevents': 1000,
        'files': '',
        'noop': False,
        'output_file': OUTPUT_FILE,
        'pset_template': BASE,
        'mode': 'central',
        'where': 'local',
    }

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    @time_function('run local', LOG)
    def run(self, args, variables):
        self.__add_output_file(variables)

        self.__prepare(args, variables)
        chosen_dataset = self.__variables['dataset']
        from imp import load_source
        analysis_info = load_source(
            'analysis_info', BAT_PYTHON + '/analysis_info.py')
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

        self.__variables['input_files'] = input_files

        return self.__run_payload()

    def __add_output_file(self, variables):
        if 'output_file' in variables:
            output_file = os.path.join(RESULTDIR, variables['output_file'])
            if not output_file.endswith('.root'):
                output_file += '.root'
            variables['output_file'] = output_file

    def __add_input_files(self):
        input_files = []
        path = self.__variables['files']
        dataset = self.__variables['dataset']
        if path != '':
            input_files = self.input_files_from_path(path)
        else:
            input_files = self.input_files_from_dataset(dataset)
        self.__variables['input_files'] = input_files

    def input_files_from_path(self, path):
        """
            Converts given path(s) to input files. 
        """
        if ',' in path:
            input_files = path.split(',')
        elif '*' in path:
            input_files = glob.glob(path)
        else:  # neither wildcard nor comma separated list
            input_files = [path]
        input_files = [os.path.abspath(f) for f in input_files]
        return [f for f in input_files if os.path.exists(f)]

    def input_files_from_dataset(self, dataset):
        analysis_info_file = os.path.join(BAT_PYTHON, 'analysis_info.py')
        from imp import load_source
        analysis_info = load_source('analysis_info', analysis_info_file)

        datasets = analysis_info.datasets_13TeV
        if not dataset in datasets_13TeV:
            msg = 'Cannot find dataset {0}'.format(dataset)
            LOG.error(msg)
            import sys
            sys.exit(msg)
        paths = [os.path.join(p, '*.root') for p in datasets_13TeV[dataset]]
        return self.input_files_from_path(path)

    def __run_payload(self):
        # TODO: add checking/dynamic subcommand discovery
        if self.__variables['where'] == 'local':
            from .local import Command as PayLoad
        else:
            from .DICE import Command as PayLoad

        payload = PayLoad()
        return payload.run([], self.__variables)

    def __can_run(self):
        return True
