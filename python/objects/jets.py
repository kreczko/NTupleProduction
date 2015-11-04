from PhysicsTools.Heppy.analyzers.core.autovars import NTupleObjectType, NTupleVariable

jetType = NTupleObjectType(
    'jet',
    variables=[
        NTupleVariable('pt', lambda x: x.pt())
    ]
)
