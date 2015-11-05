from PhysicsTools.Heppy.analyzers.core.autovars import NTupleObjectType, NTupleVariable
from BristolAnalysis.NTupleTools.objects.lepton import leptonType

muonType = NTupleObjectType(
    'muon',
    baseObjectTypes = [ leptonType ],
    variables=[
        # muon ID variables
        NTupleVariable("nStations", lambda x : x.numberOfMatchedStations(),
            help="Number of matched muons stations (4 for electrons)"),
        NTupleVariable("caloCompatibility", lambda x : x.caloCompatibility(),
            help="Calorimetric compatibility"),
        NTupleVariable("globalTrackChi2", lambda x : x.globalTrack().normalizedChi2() if x.globalTrack().isNonnull() else 0,
            help="Global track normalized chi2"),
        NTupleVariable("trkKink", lambda x : x.combinedQuality().trkKink,
            help="Tracker kink-finder"),
        NTupleVariable("segmentCompatibility", lambda x : x.segmentCompatibility(),
            help="Segment-based compatibility"),
        NTupleVariable("chi2LocalPosition", lambda x : x.combinedQuality().chi2LocalPosition,
            help="Tracker-Muon matching in position"),
        NTupleVariable("chi2LocalMomentum", lambda x : x.combinedQuality().chi2LocalMomentum,
            help="Tracker-Muon matching in momentum"),
    ]
)
