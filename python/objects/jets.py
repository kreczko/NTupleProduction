from PhysicsTools.Heppy.analyzers.core.autovars import NTupleObjectType, NTupleVariable
from BristolAnalysis.NTupleTools.objects.particle import particleType

jetType = NTupleObjectType(
    'jet',
    baseObjectTypes = [ particleType ],
    variables=[
        NTupleVariable("loosePfId",
            lambda x : x.jetID("POG_PFID") , int, mcOnly=False,
            help="POG Loose PF jet ID"),
        NTupleVariable("btagCSV",
            lambda x : x.btag('pfCombinedInclusiveSecondaryVertexV2BJetTags'),
            help="CSV-IVF v2 discriminator"),
    ]
)
