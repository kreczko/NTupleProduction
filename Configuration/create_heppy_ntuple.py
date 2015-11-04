#! /usr/bin/env python
import ROOT
import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
# need to rename due to * import in AutoFillTreeProducer which imports the default jet type
# which is not necessary!!!
from BristolAnalysis.NTupleTools.objects.jets import jetType as bristolJetType

# The content of the output tree is defined here
# the definitions of the NtupleObjects are located under
# PhysicsTools/Heppy/pythonanalyzers/objects/autophobj.py

from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer import *
from BristolAnalysis.NTupleTools.producers.event import EventProducer
treeProducer = cfg.Analyzer(
    class_object=EventProducer,
    verbose=False,
    vectorTree=True,
    # here the list of simple event variables (floats, int) can be specified
    globalVariables=[
        #              NTupleVariable("rho",  lambda ev: ev.fixedGridRhoFastjetAll, float, help="jets rho"),
        #              NTupleVariable("n_jets",  lambda ev: len(ev.slimmedJets), int, help="number of jets"),
    ],
    # here one can specify compound objects
    globalObjects={
        #           "met"    : NTupleObject("met",     metType, help="PF E_{T}^{miss}, after default type 1 corrections"),
    },
    collections={
        'slimmedJets': (AutoHandle(("slimmedJets",), "std::vector<pat::Jet>"),
                        NTupleCollection("jets", bristolJetType, 99, help="jets, directly from MINIAOD")),
    }
)

sequence = [treeProducer]

# use tfile service to provide a single TFile to all modules where they
# can write any root object. If the name is 'outputfile' or the one specified in treeProducer
# also the treeProducer uses this file
from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name="outputfile",
    fname='tree.root',
    option='recreate'
)

sample = cfg.Component(
    files=[
        '/hdfs/TopQuarkGroup/run2/miniAOD/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_74X_mcRun2_asymptotic_v2-v3_miniAODv2.root'],
    name="SingleSample", isMC=False, isEmbed=False
)


# the following is declared in case this cfg is used in input to the
# heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
selectedComponents = [sample]
config = cfg.Config(components=selectedComponents,
                    sequence=sequence,
                    services=[output_service],
                    events_class=Events)

# and the following runs the process directly if running as with python
# filename.py
if __name__ == '__main__':
    from PhysicsTools.HeppyCore.framework.looper import Looper
    looper = Looper('Loop', config, nPrint=5, nEvents=300)
    looper.loop()
    looper.write()
