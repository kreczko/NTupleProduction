
from crab.base import config
NAME = __file__.split('/')[-1]
NAME = NAME.replace('.py', '')

config.General.requestName = NAME
config.Data.outputDatasetTag = NAME
config.Data.inputDataset = '/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-scaleup-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
config.Data.outLFNDirBase += '/50ns'
config.JobType.pyCfgParams = ['isTTbarMC=1']
