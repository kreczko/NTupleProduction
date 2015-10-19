import ROOT
from PhysicsTools.Heppy.analyzers.core.autovars import *
from PhysicsTools.HeppyCore.utils.deltar import deltaR

objectFloat = NTupleObjectType("builtInType", variables=[
    NTupleVariable("", lambda x: x),
])
objectInt = NTupleObjectType("builtInType", variables=[
    NTupleVariable("", lambda x: x, int),
])

ourVectorType = NTupleObjectType("fourVector", variables=[
    NTupleVariable("pt", lambda x: x.pt()),
    NTupleVariable("eta", lambda x: x.eta()),
    NTupleVariable("phi", lambda x: x.phi()),
    NTupleVariable("mass", lambda x: x.mass()),
    NTupleVariable("p4", lambda x: x, "TLorentzVector", default=ROOT.reco.Particle.LorentzVector(
        0., 0., 0., 0.), filler=lambda vector, obj: vector.SetPtEtaPhiM(obj.pt(), obj.eta(), obj.phi(), obj.mass())),
    #               ^^^^------- Note: p4 normally is not saved unless 'saveTLorentzVectors' is enabled in the tree producer
])
