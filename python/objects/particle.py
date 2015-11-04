from PhysicsTools.Heppy.analyzers.objects.autophobj import fourVectorType

"""
    based on fourVectorType which has pT, eta, phi, mass and full four-vector
    lambda x : x.pdgId() is the accessor function for variable 'pdgId'. It will
    try to call that function on the object that is passed to this NTupleObjectType.
"""
particleType = NTupleObjectType("particle", baseObjectTypes = [ fourVectorType ],
    variables = [
        NTupleVariable("pdgId", lambda x : x.pdgId(), int, mcOnly=True,
        help='PDG particle ID (google it)'),
        ]
    )
