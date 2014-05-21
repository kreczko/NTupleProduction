import FWCore.ParameterSet.Config as cms
rootTupleEvent = cms.EDProducer(
        "BristolNTuple_Event",
        DCSInputTag=cms.InputTag('scalersRawToDigi'),
        HBHENoiseFilterInput=cms.InputTag('HBHENoiseFilterResultProducer', 'HBHENoiseFilterResult'),
        HCALLaserFilterInput=cms.InputTag('HcalLaserEventFilter'),
        ECALDeadCellFilterInput=cms.InputTag('EcalDeadCellBoundaryEnergyFilter'),
        ECALDeadCellTriggerPrimitiveFilterInput=cms.InputTag('EcalDeadCellTriggerPrimitiveFilter'),
        TrackingFailureFilterInput=cms.InputTag('trackingFailureFilter'),
        EEBadSCFilterInput = cms.InputTag('eeBadScFilter'),
        ECALLaserCorrFilterInput = cms.InputTag('ecalLaserCorrFilter'),
        #tracking POG filters
        manystripclus53XInput = cms.InputTag('manystripclus53X'),
        toomanystripclus53XInput = cms.InputTag('toomanystripclus53X'),
        logErrorTooManyClustersInput = cms.InputTag('logErrorTooManyClusters'),
        
        recoVertexInputTag = cms.InputTag('rootTupleVertex','goodOfflinePrimaryVertices.NRecoVertices'),

        METInputForSumET=cms.InputTag('patMETsPFlow'),
        Prefix=cms.string('Event.'),
        Suffix=cms.string('')
)
