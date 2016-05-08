
from crab.base import config
NAME = __file__.split('/')[-1]
NAME = NAME.replace('.py', '')

config.General.requestName = NAME
config.Data.outputDatasetTag = NAME
config.Data.inputDataset = '/SingleMuon/Run2015D-16Dec2015-v1/MINIAOD'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 500000
config.Data.outLFNDirBase += '/FALL15'
config.Data.lumiMask = '/hdfs/TopQuarkGroup/run2/json/ReReco_MinusBeamSpotIssue.txt'
