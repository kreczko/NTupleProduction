
from crab.base import config
NAME = __file__.split('/')[-1]
NAME = NAME.replace('.py', '')

config.General.requestName = NAME
config.Data.outputDatasetTag = NAME
config.Data.inputDataset = '/QCD_Pt-30to50_MuEnrichedPt5_PionKaonDecay_Tune4C_13TeV_pythia8/Phys14DR-AVE20BX25_tsg_PHYS14_25_V3-v2/MINIAODSIM'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10


