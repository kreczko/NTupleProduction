import os
import datetime
from WMCore.Configuration import Configuration
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRABClientLibraryAPI
from CRABClient.UserUtilities import getUsernameFromSiteDB

# env variables
if not 'NTPROOT' in os.environ:
    import sys
    print('Cannot find NTPROOT env variable. Did you "source bin/env.sh"?')
    sys.exit(-1)

# this is the NTupleVersion!
__version__ = '0.0.1'
NTPROOT = os.environ['NTPROOT']
WORKSPACE = NTPROOT + '/workspace'
CRAB_WS = WORKSPACE + '/crab'
TODAY = date.today().isoformat()
PSET = NTPROOT + '/python/run/miniAODToNTuple_cfg.py'
INPUT_FILES = [
    'data/BTagSF/CSVv2.csv',
    'data/JEC/Fall15_25nsV2_DATA.db',
    'data/JEC/Fall15_25nsV2_MC.db',
]

config = Configuration()

config.section_("General")
config.General.requestName = 'TESTING'
config.General.workArea = '{crab_work_dir}/v{version}/{date}'.format(
    crab_work_dir=CRAB_WS, version=__version__, date=TODAY)
config.General.transferOutputs = True
# config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = PSET
config.JobType.allowUndistributedCMSSW = True
config.JobType.inputFiles = [NTPROOT + '/' + f for f in INPUT_FILES]

config.section_("Data")
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.outLFNDirBase = '/store/user/{user}/ntuple/v{version}'.format(
    user=getUsernameFromSiteDB(), version=__version__)
config.Data.publication = True
# config.Data.ignoreLocality = True

config.section_("Site")
config.Site.storageSite = 'T2_UK_SGrid_Bristol'
config.Site.blacklist = ["T2_BR_SPRACE", "T2_UA_KIPT", "T2_BR_UERJ"]