
from crab.base import config
NAME = __file__.split('/')[-1]
NAME = NAME.replace('.py', '')

config.General.requestName = NAME
config.Data.outputDatasetTag = NAME
config.Data.inputDataset = '/QCD_Pt-80to170_EMEnriched_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_castor_PHYS14_25_V1-v1/MINIAODSIM'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10


