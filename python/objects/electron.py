from PhysicsTools.Heppy.analyzers.core.autovars import NTupleObjectType, NTupleVariable
from BristolAnalysis.NTupleTools.objects.lepton import leptonType

electronType = NTupleObjectType(
    'electron',
    baseObjectTypes = [ leptonType ],
    variables=[
        # electron ID variables
        NTupleVariable("sigmaIEtaIEta", lambda x : x.full5x5_sigmaIetaIeta(),
            help="Electron sigma(ieta ieta), with full5x5 cluster shapes"),
        NTupleVariable("dEtaScTrkIn", lambda x : x.deltaEtaSuperClusterTrackAtVtx(),
            help="Electron deltaEtaSuperClusterTrackAtVtx (without absolute value!)"),
        NTupleVariable("dPhiScTrkIn", lambda x : x.deltaPhiSuperClusterTrackAtVtx(),
            help="Electron deltaPhiSuperClusterTrackAtVtx (without absolute value!)"),
        NTupleVariable("hadronicOverEm", lambda x : x.hadronicOverEm(),
            help="Electron hadronicOverEm"),
        NTupleVariable("eInvMinusPInv", lambda x : ((1.0/x.ecalEnergy() - x.eSuperClusterOverP()/x.ecalEnergy()) if     x.ecalEnergy()>0. else 9e9),
            help="Electron 1/E - 1/p  (without absolute value!)"),
        NTupleVariable("etaSc", lambda x : x.superCluster().eta(),
            help="Electron supercluster pseudorapidity"),
    ]
)
