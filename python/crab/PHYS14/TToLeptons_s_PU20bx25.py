
from crab.base import config
NAME = __file__.split('/')[-1]
NAME = NAME.replace('.py', '')

config.General.requestName = NAME
config.Data.outputDatasetTag = NAME
config.Data.inputDataset = '/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.outLFNDirBase += '/PHYS14'

