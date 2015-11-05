from PhysicsTools.Heppy.analyzers.core.autovars import NTupleObjectType, NTupleVariable
from BristolAnalysis.NTupleTools.objects.particle import particleType

jetType = NTupleObjectType(
    'jet',
    baseObjectTypes = [ particleType ],
    variables=[
        NTupleVariable("partonFlavour",
            lambda x : x.partonFlavour() , int, mcOnly=True,
            help="parton flavour of the jet"),
        NTupleVariable("btagCSVv2Disc",
            lambda x : x.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
            help="CSV-IVF v2 discriminator"),
    ]
)
