#! /usr/bin/env python
import ROOT
import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
# need to rename due to * import in AutoFillTreeProducer which imports the default jet type
# which is not necessary!!!
from BristolAnalysis.NTupleTools.objects.jets import jetType as bristolJetType
from BristolAnalysis.NTupleTools.objects.electron import electronType as bristolElectronType
from BristolAnalysis.NTupleTools.objects.muon import muonType as bristolMuonType

# The content of the output tree is defined here
# the definitions of the NtupleObjects are located under
# PhysicsTools/Heppy/pythonanalyzers/objects/autophobj.py

from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer import *
from BristolAnalysis.NTupleTools.producers.event import EventProducer

# there is no distinction between analysers and producers in Heppy
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
                        NTupleCollection("jets", bristolJetType, 99,
                            help="jets, directly from MINIAOD")),
        'slimmedElectrons': (AutoHandle(("slimmedElectrons",), "std::vector<pat::Electron>"),
                        NTupleCollection("electrons", bristolElectronType,
                            99, help="electrons, directly from MINIAOD")),
        'slimmedMuons': (AutoHandle(("slimmedMuons",), "std::vector<pat::Muon>"),
                        NTupleCollection("muons", bristolMuonType,
                            99, help="muons, directly from MINIAOD")),
    },
    saveTLorentzVectors = True,
)

# these 'analysers' produce additinal event content
"""
    The LHEAnalyzer reads the externalLHEProducer::LHEEventProduct and adds 
    lheHT, lheHTIncoming, lheNj/b/c/l/h and lheV_pt (vector boson pt)
    to the event
"""
from PhysicsTools.Heppy.analyzers.gen.LHEAnalyzer import LHEAnalyzer 
LHEAna = LHEAnalyzer.defaultConfig
"""
    The GeneratorAnalyser looks for Higgs, W/Z, neutrinos, leptons, taus, b quarks,
    b-quarks from top and b-quarks from Higgs as well as W/Z quarks and leptons from top.
    It also adds the LHE_weights
"""
from PhysicsTools.Heppy.analyzers.gen.GeneratorAnalyzer import GeneratorAnalyzer 
GenAna = GeneratorAnalyzer.defaultConfig


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

# Use data component for data with JSON files
# or MCComponent for MC with lumi weights (int lumi always == 1?)
# kwargs for Component like isMC are not used!
sample = cfg.MCComponent(
    files=[
        'BristolAnalysis/NTupleTools/data/test/TTJets_powhegPythia8_25ns.root',
#         'BristolAnalysis/NTupleTools/data/test/SingleElectron25ns.root',
#         'BristolAnalysis/NTupleTools/data/test/SingleMuon25ns.root',
        ],
    name="SingleSample",
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
