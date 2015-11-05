from PhysicsTools.Heppy.analyzers.core.autovars import NTupleObjectType, NTupleVariable
from BristolAnalysis.NTupleTools.objects.particle import particleType

leptonType = NTupleObjectType(
    'electron',
    baseObjectTypes = [ particleType ],
    variables=[
        NTupleVariable("charge",   lambda x : x.charge(), int),
        # Impact parameter
#         NTupleVariable("dxy", lambda x : x.dxy(),
#             help="d_{xy} with respect to PV, in cm (with sign)"),
#         NTupleVariable("dz", lambda x : x.dz(),
#             help="d_{z} with respect to PV, in cm (with sign)"),
        # Isolations with the two radia
#         NTupleVariable("relIso03", lambda x : x.relIso03,
#             help="PF Rel Iso, R=0.3, pile-up corrected"),
#         NTupleVariable("relIso04", lambda x : x.relIso04,
#             help="PF Rel Iso, R=0.4, pile-up corrected"),
    ]
)
