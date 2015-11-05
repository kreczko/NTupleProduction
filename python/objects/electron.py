from PhysicsTools.Heppy.analyzers.core.autovars import NTupleObjectType, NTupleVariable
from BristolAnalysis.NTupleTools.objects.lepton import leptonType

electronType = NTupleObjectType(
    'electron',
    baseObjectTypes=[leptonType],
    variables=[
        # electron ID variables
        NTupleVariable("sigmaIEtaIEta", lambda x: x.full5x5_sigmaIetaIeta(),
                       help="Electron sigma(ieta ieta), with full5x5 cluster shapes"),
        NTupleVariable("dEtaScTrkIn", lambda x: x.deltaEtaSuperClusterTrackAtVtx(),
                       help="Electron deltaEtaSuperClusterTrackAtVtx (without absolute value!)"),
        NTupleVariable("dPhiScTrkIn", lambda x: x.deltaPhiSuperClusterTrackAtVtx(),
                       help="Electron deltaPhiSuperClusterTrackAtVtx (without absolute value!)"),
        NTupleVariable("hadronicOverEm", lambda x: x.hadronicOverEm(),
                       help="Electron hadronicOverEm"),
        NTupleVariable("eInvMinusPInv", lambda x: ((1.0 / x.ecalEnergy() - x.eSuperClusterOverP() / x.ecalEnergy()) if x.ecalEnergy() > 0. else 9e9),
                       help="Electron 1/E - 1/p  (without absolute value!)"),
        NTupleVariable("etaSc", lambda x: x.superCluster().eta(),
                       help="Electron supercluster pseudorapidity"),
#         NTupleVariable("eleCutIdCSA14_25ns_v1",
#                        lambda x: (1 * x.electronID("POG_Cuts_ID_CSA14_25ns_v1_Veto") + 1 * x.electronID("POG_Cuts_ID_CSA14_25ns_v1_Loose") + \
#                                   1 * x.electronID("POG_Cuts_ID_CSA14_25ns_v1_Medium") + 1 * x.electronID("POG_Cuts_ID_CSA14_25ns_v1_Tight")),
#                        int, help="Electron cut-based id (POG CSA14_25ns_v1): 0=none, 1=veto, 2=loose, 3=medium, 4=tight"),
#         NTupleVariable("eleCutIdCSA14_50ns_v1",
#                        lambda x: (1 * x.electronID("POG_Cuts_ID_CSA14_50ns_v1_Veto") + 1 * x.electronID("POG_Cuts_ID_CSA14_50ns_v1_Loose") + \
#                                   1 * x.electronID("POG_Cuts_ID_CSA14_50ns_v1_Medium") + 1 * x.electronID("POG_Cuts_ID_CSA14_50ns_v1_Tight")),
#                        int, help="Electron cut-based id (POG CSA14_50ns_v1): 0=none, 1=veto, 2=loose, 3=medium, 4=tight"),
    ]
)
