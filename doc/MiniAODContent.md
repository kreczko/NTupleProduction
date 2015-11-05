# data
Event content produced with `edmDumpEventContent <root file>`.
```
Type                                  Module                      Label             Process   
----------------------------------------------------------------------------------------------          
edm::TriggerResults                   "TriggerResults"            ""                "HLT"     
HcalNoiseSummary                      "hcalnoise"                 ""                "RECO"    
L1GlobalTriggerReadoutRecord          "gtDigis"                   ""                "RECO"    
double                                "fixedGridRhoAll"           ""                "RECO"    
double                                "fixedGridRhoFastjetAll"    ""                "RECO"    
double                                "fixedGridRhoFastjetAllCalo"   ""                "RECO"    
double                                "fixedGridRhoFastjetCentralCalo"   ""                "RECO"    
double                                "fixedGridRhoFastjetCentralChargedPileUp"   ""                "RECO"    
double                                "fixedGridRhoFastjetCentralNeutral"   ""                "RECO"    
edm::TriggerResults                   "TriggerResults"            ""                "RECO"    
reco::BeamSpot                        "offlineBeamSpot"           ""                "RECO"    
vector<l1extra::L1EmParticle>         "l1extraParticles"          "Isolated"        "RECO"    
vector<l1extra::L1EmParticle>         "l1extraParticles"          "NonIsolated"     "RECO"    
vector<l1extra::L1EtMissParticle>     "l1extraParticles"          "MET"             "RECO"    
vector<l1extra::L1EtMissParticle>     "l1extraParticles"          "MHT"             "RECO"    
vector<l1extra::L1HFRings>            "l1extraParticles"          ""                "RECO"    
vector<l1extra::L1JetParticle>        "l1extraParticles"          "Central"         "RECO"    
vector<l1extra::L1JetParticle>        "l1extraParticles"          "Forward"         "RECO"    
vector<l1extra::L1JetParticle>        "l1extraParticles"          "IsoTau"          "RECO"    
vector<l1extra::L1JetParticle>        "l1extraParticles"          "Tau"             "RECO"    
vector<l1extra::L1MuonParticle>       "l1extraParticles"          ""                "RECO"    
edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >    "reducedEgamma"             "reducedEBRecHits"   "PAT"     
edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >    "reducedEgamma"             "reducedEERecHits"   "PAT"     
edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >    "reducedEgamma"             "reducedESRecHits"   "PAT"     
edm::TriggerResults                   "TriggerResults"            ""                "PAT"     
edm::ValueMap<float>                  "offlineSlimmedPrimaryVertices"   ""                "PAT"     
pat::PackedTriggerPrescales           "patTrigger"                ""                "PAT"     
pat::PackedTriggerPrescales           "patTrigger"                "l1max"           "PAT"     
pat::PackedTriggerPrescales           "patTrigger"                "l1min"           "PAT"      
vector<pat::Electron>                 "slimmedElectrons"          ""                "PAT"     
vector<pat::Jet>                      "slimmedJets"               ""                "PAT"     
vector<pat::Jet>                      "slimmedJetsAK8"            ""                "PAT"     
vector<pat::Jet>                      "slimmedJetsPuppi"          ""                "PAT"     
vector<pat::Jet>                      "slimmedJetsAK8PFCHSSoftDropPacked"   "SubJets"         "PAT"     
vector<pat::Jet>                      "slimmedJetsCMSTopTagCHSPacked"   "SubJets"         "PAT"     
vector<pat::MET>                      "slimmedMETs"               ""                "PAT"     
vector<pat::MET>                      "slimmedMETsNoHF"           ""                "PAT"     
vector<pat::MET>                      "slimmedMETsPuppi"          ""                "PAT"     
vector<pat::Muon>                     "slimmedMuons"              ""                "PAT"     
vector<pat::PackedCandidate>          "lostTracks"                ""                "PAT"     
vector<pat::PackedCandidate>          "packedPFCandidates"        ""                "PAT"          
vector<pat::Photon>                   "slimmedPhotons"            ""                "PAT"     
vector<pat::Tau>                      "slimmedTaus"               ""                "PAT"     
vector<pat::TriggerObjectStandAlone>    "selectedPatTrigger"        ""                "PAT"     
vector<reco::CATopJetTagInfo>         "caTopTagInfosPAT"          ""                "PAT"     
vector<reco::CaloCluster>             "reducedEgamma"             "reducedEBEEClusters"   "PAT"     
vector<reco::CaloCluster>             "reducedEgamma"             "reducedESClusters"   "PAT"     
vector<reco::Conversion>              "reducedEgamma"             "reducedConversions"   "PAT"     
vector<reco::Conversion>              "reducedEgamma"             "reducedSingleLegConversions"   "PAT"          
vector<reco::GsfElectronCore>         "reducedEgamma"             "reducedGedGsfElectronCores"   "PAT"     
vector<reco::PhotonCore>              "reducedEgamma"             "reducedGedPhotonCores"   "PAT"     
vector<reco::SuperCluster>            "reducedEgamma"             "reducedSuperClusters"   "PAT"     
vector<reco::Vertex>                  "offlineSlimmedPrimaryVertices"   ""                "PAT"     
vector<reco::VertexCompositePtrCandidate>    "slimmedSecondaryVertices"   ""                "PAT" 
```


# MC only
In addition to what is available in data, MC also contains the following
```
Type                                  Module                      Label             Process   
----------------------------------------------------------------------------------------------
LHEEventProduct                       "externalLHEProducer"       ""                "LHE"     
GenEventInfoProduct                   "generator"                 ""                "SIM"     
edm::TriggerResults                   "TriggerResults"            ""                "SIM"          
vector<PileupSummaryInfo>             "slimmedAddPileupInfo"      ""                "PAT"          
vector<pat::PackedGenParticle>        "packedGenParticles"        ""                "PAT"          
vector<reco::GenJet>                  "slimmedGenJets"            ""                "PAT"     
vector<reco::GenJet>                  "slimmedGenJetsAK8"         ""                "PAT"     
vector<reco::GenParticle>             "prunedGenParticles"        ""                "PAT"     
```

# Available electron IDs
```
The available IDs are: 
# PHYS14
'cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-loose' 'cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-medium'
'cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-tight' 'cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-veto' 
# Spring15 25ns
'cutBasedElectronID-Spring15-25ns-V1-standalone-loose' 'cutBasedElectronID-Spring15-25ns-V1-standalone-medium'
'cutBasedElectronID-Spring15-25ns-V1-standalone-tight' 'cutBasedElectronID-Spring15-25ns-V1-standalone-veto'
'mvaEleID-Spring15-25ns-nonTrig-V1-wp80' 'mvaEleID-Spring15-25ns-nonTrig-V1-wp90'
# Spring15 50ns
'cutBasedElectronID-Spring15-50ns-V1-standalone-loose' 'cutBasedElectronID-Spring15-50ns-V1-standalone-medium'
'cutBasedElectronID-Spring15-50ns-V1-standalone-tight' 'cutBasedElectronID-Spring15-50ns-V1-standalone-veto'
# others
'eidLoose' 'eidRobustHighEnergy' 'eidRobustLoose' 'eidRobustTight' 'eidTight' 'heepElectronID-HEEPV60'  .

```