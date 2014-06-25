#include "BristolAnalysis/NTupleTools/interface/BristolNTuple_Electrons.h"
#include "BristolAnalysis/NTupleTools/interface/DirectionalIsolation.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "BristolAnalysis/NTupleTools/interface/PatUtilities.h"

BristolNTuple_Electrons::BristolNTuple_Electrons(const edm::ParameterSet& iConfig) :
		trkInputTag(iConfig.getParameter < edm::InputTag > ("TracksInputTag")), //
		dcsInputTag(iConfig.getParameter < edm::InputTag > ("DCSInputTag")), //
		inputTag(iConfig.getParameter < edm::InputTag > ("InputTag")), //
		prefix(iConfig.getParameter < std::string > ("Prefix")), //
		suffix(iConfig.getParameter < std::string > ("Suffix")), //
		maxSize(iConfig.getParameter<unsigned int>("MaxSize")), //
		storePFIsolation_(iConfig.getParameter<bool>("storePFIsolation")), //
		debugRelease_(iConfig.getParameter<bool>("debugRelease")), //
		vtxInputTag(iConfig.getParameter < edm::InputTag > ("VertexInputTag")), //
		beamSpotInputTag(iConfig.getParameter < edm::InputTag > ("BeamSpotInputTag")), //
		conversionsInputTag(iConfig.getParameter < edm::InputTag > ("ConversionsInputTag")), //
		likelihoodInputTag(iConfig.getParameter < edm::InputTag > ("LikelihoodInputTag")) //
{

	//kinematic variables
	produces < std::vector<float> > (prefix + "Px" + suffix);
	produces < std::vector<float> > (prefix + "Py" + suffix);
	produces < std::vector<float> > (prefix + "Pz" + suffix);
	produces < std::vector<float> > (prefix + "Energy" + suffix);
	//extra properties
	produces < std::vector<int> > (prefix + "Charge" + suffix);
	produces < std::vector<float> > (prefix + "ecalDrivenMomentum.Px" + suffix);
	produces < std::vector<float> > (prefix + "ecalDrivenMomentum.Py" + suffix);
	produces < std::vector<float> > (prefix + "ecalDrivenMomentum.Pz" + suffix);

	//electron ID variables
	produces < std::vector<float> > (prefix + "HadronicOverEM" + suffix);
	produces < std::vector<float> > (prefix + "eSeedClusterOverPout" + suffix);
	produces < std::vector<float> > (prefix + "ESuperClusterOverP" + suffix);
	produces < std::vector<float> > (prefix + "Fbrem" + suffix);
	produces < std::vector<float> > (prefix + "SigmaEtaEta" + suffix);
	produces < std::vector<float> > (prefix + "SigmaIEtaIEta" + suffix);
	produces < std::vector<float> > (prefix + "DeltaPhiTrkSC" + suffix);
	produces < std::vector<float> > (prefix + "DeltaEtaTrkSC" + suffix);
	produces < std::vector<float> > (prefix + "Likelihood" + suffix);
	produces < std::vector<int> > (prefix + "NumberOfBrems" + suffix);
	produces < std::vector<float> > (prefix + "mvaTrigV0" + suffix);
	produces < std::vector<float> > (prefix + "mvaNonTrigV0" + suffix);

	//electron isolation variables
	produces < std::vector<float> > (prefix + "TrkIso03" + suffix);
	produces < std::vector<float> > (prefix + "EcalIso03" + suffix);
	produces < std::vector<float> > (prefix + "HcalIso03" + suffix);
	produces < std::vector<float> > (prefix + "RelIso03" + suffix);
	produces < std::vector<float> > (prefix + "TrkIso04" + suffix);
	produces < std::vector<float> > (prefix + "EcalIso04" + suffix);
	produces < std::vector<float> > (prefix + "HcalIso04" + suffix);
	produces < std::vector<float> > (prefix + "RelIso04" + suffix);

	//electron PF isolation variables
	if (storePFIsolation_) {
		produces < std::vector<float> > (prefix + "PfChargedHadronIso03" + suffix);
		produces < std::vector<float> > (prefix + "PfNeutralHadronIso03" + suffix);
		produces < std::vector<float> > (prefix + "PFGammaIso03" + suffix);
		produces < std::vector<float> > (prefix + "PFRelIso03" + suffix);
		produces < std::vector<float> > (prefix + "PFRelIso03DeltaBeta" + suffix);
		produces < std::vector<float> > (prefix + "PFRelIso03RhoEA" + suffix);

		produces < std::vector<float> > (prefix + "PfChargedHadronIso04" + suffix);
		produces < std::vector<float> > (prefix + "PfNeutralHadronIso04" + suffix);
		produces < std::vector<float> > (prefix + "PFGammaIso04" + suffix);
		produces < std::vector<float> > (prefix + "PFRelIso04" + suffix);

		produces < std::vector<float> > (prefix + "PfChargedHadronIso05" + suffix);
		produces < std::vector<float> > (prefix + "PfNeutralHadronIso05" + suffix);
		produces < std::vector<float> > (prefix + "PFGammaIso05" + suffix);
		produces < std::vector<float> > (prefix + "PFRelIso05" + suffix);
		//directional isolation
		produces < std::vector<float> > (prefix + "DirectionalPFIso02" + suffix);
		produces < std::vector<float> > (prefix + "DirectionalPFIso02FallOff" + suffix);
		produces < std::vector<float> > (prefix + "PfRelIso02FallOff" + suffix);
		produces < std::vector<float> > (prefix + "DirectionalPFIso03" + suffix);
		produces < std::vector<float> > (prefix + "DirectionalPFIso03FallOff" + suffix);
		produces < std::vector<float> > (prefix + "PfRelIso03FallOff" + suffix);
		//PU charged hadron isolation
		produces < std::vector<float> > (prefix + "PfPUChargedHadronIso03" + suffix);
		produces < std::vector<float> > (prefix + "PfPUChargedHadronIso04" + suffix);
		produces < std::vector<float> > (prefix + "PfPUChargedHadronIso05" + suffix);
	}

	//high energy electron isolation variables
	produces < std::vector<float> > (prefix + "EcalIsoHeep03" + suffix);
	produces < std::vector<float> > (prefix + "HcalIsoD1Heep03" + suffix);
	produces < std::vector<float> > (prefix + "HcalIsoD2Heep03" + suffix);
	produces < std::vector<float> > (prefix + "TrkIsoHeep03" + suffix);
	produces < std::vector<float> > (prefix + "EcalIsoHeep04" + suffix);
	produces < std::vector<float> > (prefix + "HcalIsoD1Heep04" + suffix);
	produces < std::vector<float> > (prefix + "HcalIsoD2Heep04" + suffix);
	produces < std::vector<float> > (prefix + "TrkIsoHeep04" + suffix);

	//electron conversion identification variables
	produces < std::vector<float> > (prefix + "Dist" + suffix);
	produces < std::vector<float> > (prefix + "DCotTheta" + suffix);
	produces < std::vector<float> > (prefix + "ConvRadius" + suffix);
	produces < std::vector<int> > (prefix + "MissingHits" + suffix);
	//this might be identical to passConversionVeto
	produces < std::vector<bool> > (prefix + "HasMatchedConvPhot" + suffix);
	produces < std::vector<float> > (prefix + "shFracInnerHits" + suffix);
	produces < std::vector<bool> > (prefix + "passConversionVeto" + suffix);

	//calorimeter variables
	produces < std::vector<float> > (prefix + "SCEta" + suffix);
	produces < std::vector<float> > (prefix + "SCPhi" + suffix);
	produces < std::vector<float> > (prefix + "SCPt" + suffix);
	produces < std::vector<float> > (prefix + "SCP" + suffix);
	produces < std::vector<float> > (prefix + "SCEnergy" + suffix);
	produces < std::vector<float> > (prefix + "SCRawEnergy" + suffix);

	//electron vertex variables
	produces < std::vector<int> > (prefix + "VtxIndex" + suffix);
	produces < std::vector<float> > (prefix + "VtxDistZ" + suffix);
	produces < std::vector<float> > (prefix + "PrimaryVertexDXY" + suffix); //2D impact parameter, XY distance to vertex
	produces < std::vector<float> > (prefix + "PrimaryVertexDXYError" + suffix);
	produces < std::vector<float> > (prefix + "BeamSpotDXY" + suffix);
	produces < std::vector<float> > (prefix + "BeamSpotDXYError" + suffix);
	//quantities corrected for position of primary vertex
	produces < std::vector<float> > (prefix + "PrimaryVertexDXYCorr" + suffix);
	produces < std::vector<float> > (prefix + "VtxDistZCorr" + suffix);
}

void BristolNTuple_Electrons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
	//kinematic variables
	std::auto_ptr < std::vector<float> > px(new std::vector<float>());
	std::auto_ptr < std::vector<float> > py(new std::vector<float>());
	std::auto_ptr < std::vector<float> > pz(new std::vector<float>());
	std::auto_ptr < std::vector<float> > energy(new std::vector<float>());
	//extra properties
	std::auto_ptr < std::vector<int> > charge(new std::vector<int>());
	std::auto_ptr < std::vector<float> > ecalDrivenMomentumPx(new std::vector<float>());
	std::auto_ptr < std::vector<float> > ecalDrivenMomentumPy(new std::vector<float>());
	std::auto_ptr < std::vector<float> > ecalDrivenMomentumPz(new std::vector<float>());
	//electron ID variables
	std::auto_ptr < std::vector<float> > hadronicOverEM(new std::vector<float>());
	std::auto_ptr < std::vector<float> > EseedOverPout(new std::vector<float>());
	std::auto_ptr < std::vector<float> > eSuperClusterOverP(new std::vector<float>());
	std::auto_ptr < std::vector<float> > fbrem(new std::vector<float>());
	std::auto_ptr < std::vector<float> > sigmaEtaEta(new std::vector<float>());
	std::auto_ptr < std::vector<float> > sigmaIEtaIEta(new std::vector<float>());
	std::auto_ptr < std::vector<float> > deltaPhiTrkSC(new std::vector<float>());
	std::auto_ptr < std::vector<float> > deltaEtaTrkSC(new std::vector<float>());
	std::auto_ptr < std::vector<float> > likelihood(new std::vector<float>());
	std::auto_ptr < std::vector<int> > numberOfBrems(new std::vector<int>());
	std::auto_ptr < std::vector<float> > mvaTrigV0(new std::vector<float>());
	std::auto_ptr < std::vector<float> > mvaNonTrigV0(new std::vector<float>());

	//electron isolation variables
	std::auto_ptr < std::vector<float> > trkIso04(new std::vector<float>());
	std::auto_ptr < std::vector<float> > ecalIso04(new std::vector<float>());
	std::auto_ptr < std::vector<float> > hcalIso04(new std::vector<float>());
	std::auto_ptr < std::vector<float> > relIso04(new std::vector<float>());
	//smaller cone
	std::auto_ptr < std::vector<float> > trkIso03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > ecalIso03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > hcalIso03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > relIso03(new std::vector<float>());

	//electron PF isolation variables
	std::auto_ptr < std::vector<float> > PfChargedHadronIso03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PfNeutralHadronIso03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PFGammaIso03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PFRelIso03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PFRelIso03DeltaBeta(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PFRelIso03RhoEA(new std::vector<float>());

	std::auto_ptr < std::vector<float> > PfChargedHadronIso04(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PfNeutralHadronIso04(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PFGammaIso04(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PFRelIso04(new std::vector<float>());

	std::auto_ptr < std::vector<float> > PfChargedHadronIso05(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PfNeutralHadronIso05(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PFGammaIso05(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PFRelIso05(new std::vector<float>());

	std::auto_ptr < std::vector<float> > DirectionalPFIso02(new std::vector<float>());
	std::auto_ptr < std::vector<float> > DirectionalPFIso02FallOff(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PfRelIso02FallOff(new std::vector<float>());
	std::auto_ptr < std::vector<float> > DirectionalPFIso03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > DirectionalPFIso03FallOff(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PfRelIso03FallOff(new std::vector<float>());
	//PU charged hadron isolation
	std::auto_ptr < std::vector<float> > PfPUChargedHadronIso03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PfPUChargedHadronIso04(new std::vector<float>());
	std::auto_ptr < std::vector<float> > PfPUChargedHadronIso05(new std::vector<float>());

//    std::auto_ptr < std::vector<float> > dB(new std::vector<float>());

	//high energy electron isolation variables
	std::auto_ptr < std::vector<float> > ecalIsoHeep03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > hcalIsoD1Heep03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > hcalIsoD2Heep03(new std::vector<float>());
	std::auto_ptr < std::vector<float> > trkIsoHeep03(new std::vector<float>());

	std::auto_ptr < std::vector<float> > ecalIsoHeep04(new std::vector<float>());
	std::auto_ptr < std::vector<float> > hcalIsoD1Heep04(new std::vector<float>());
	std::auto_ptr < std::vector<float> > hcalIsoD2Heep04(new std::vector<float>());
	std::auto_ptr < std::vector<float> > trkIsoHeep04(new std::vector<float>());

	//electron conversion identification variables
	std::auto_ptr < std::vector<int> > missingHits(new std::vector<int>());
	std::auto_ptr < std::vector<float> > dist_vec(new std::vector<float>());
	std::auto_ptr < std::vector<float> > dCotTheta(new std::vector<float>());
	std::auto_ptr < std::vector<float> > conversionRadius(new std::vector<float>());
	std::auto_ptr < std::vector<bool> > hasMatchedConvPhot(new std::vector<bool>());
	std::auto_ptr < std::vector<float> > shFracInnerHits(new std::vector<float>());
	std::auto_ptr < std::vector<bool> > passConversionVeto(new std::vector<bool>());

	//calorimeter variables
	std::auto_ptr < std::vector<float> > scEta(new std::vector<float>());
	std::auto_ptr < std::vector<float> > scPhi(new std::vector<float>());
	std::auto_ptr < std::vector<float> > scPt(new std::vector<float>());
	std::auto_ptr < std::vector<float> > scP(new std::vector<float>());
	std::auto_ptr < std::vector<float> > scEnergy(new std::vector<float>());
	std::auto_ptr < std::vector<float> > scRawEnergy(new std::vector<float>());

	//electron vertex variables
	std::auto_ptr < std::vector<int> > vtxIndex(new std::vector<int>());
	std::auto_ptr < std::vector<float> > vtxDistZ(new std::vector<float>());
	std::auto_ptr < std::vector<float> > primaryVertexDXY(new std::vector<float>());
	std::auto_ptr < std::vector<float> > primaryVertexDXYError(new std::vector<float>());
	std::auto_ptr < std::vector<float> > beamSpotDXY(new std::vector<float>());
	std::auto_ptr < std::vector<float> > beamSpotDXYError(new std::vector<float>());
	std::auto_ptr < std::vector<float> > vtxDistZCorr(new std::vector<float>());
	std::auto_ptr < std::vector<float> > primaryVertexDXYCorr(new std::vector<float>());

	//-----------------------------------------------------------------
	edm::Handle<double> rhoH;
	iEvent.getByLabel(edm::InputTag("kt6PFJets", "rho"), rhoH);
	double rho(*rhoH);

	edm::Handle < std::vector<pat::Electron> > electrons;
	iEvent.getByLabel(inputTag, electrons);

	edm::Handle < reco::BeamSpot > bsHandle;
	iEvent.getByLabel(beamSpotInputTag, bsHandle);

	edm::Handle < reco::ConversionCollection > hConversions;
	iEvent.getByLabel(conversionsInputTag, hConversions);

	edm::Handle < reco::TrackCollection > tracks;
	iEvent.getByLabel(trkInputTag, tracks);

	edm::Handle < DcsStatusCollection > dcsHandle;
	iEvent.getByLabel(dcsInputTag, dcsHandle);

	edm::Handle < reco::VertexCollection > primaryVertices;
	iEvent.getByLabel(vtxInputTag, primaryVertices);

	std::vector < edm::Handle<edm::ValueMap<float> > > eIDValueMap(1);
	iEvent.getByLabel(likelihoodInputTag, eIDValueMap[0]);
	const edm::ValueMap<float> & eIDmapLikelihood = *eIDValueMap[0];

	edm::Handle < reco::PFCandidateCollection > pfCandidates;
	iEvent.getByLabel("particleFlow", pfCandidates);

	if (electrons.isValid()) {
		edm::LogInfo("BristolNTuple_ElectronsExtraInfo") << "Total # Electrons: " << electrons->size();
		for (std::vector<pat::Electron>::const_iterator it = electrons->begin(); it != electrons->end(); ++it) {
			// exit from loop when you reach the required number of electrons
			if (px->size() >= maxSize)
				break;

			/* Conversion (fit)
			 * See https://indico.cern.ch/getFile.py/access?contribId=12&sessionId=0&resId=0&materialId=slides&confId=133587
			 * and
			 * https://hypernews.cern.ch/HyperNews/CMS/get/egamma/999.html ( N.1 )
			 */
			bool matchesConv = false;
			if (hConversions.isValid() && bsHandle.isValid()) {
				matchesConv = ConversionTools::hasMatchedConversion(*it, hConversions, bsHandle->position());
			} else {
				if (!bsHandle.isValid())
					edm::LogError("RootTupleMakerV2_ElectronsError") << "Error! Can't get the product "
							<< beamSpotInputTag;
				if (!hConversions.isValid())
					edm::LogError("RootTupleMakerV2_ElectronsError") << "Error! Can't get the product "
							<< conversionsInputTag;
			}
			// Vertex association
			float minVtxDist3D = 9999.;
			int vtxIndex_ = -1;
//			float vtxDistXY_ = -9999.;
			float vtxDistZ_ = -9999.;
			float vtxDistXY_Corr = -9999.;
			float vtxDistZ_Corr = -9999.;

			if (primaryVertices.isValid()) {
				edm::LogInfo("RootTupleMakerV2_ElectronsInfo") << "Total # Primary Vertices: "
						<< primaryVertices->size();
				reco::Vertex pv = primaryVertices->front();
				vtxDistXY_Corr = it->gsfTrack()->d0() - pv.x() * sin(it->gsfTrack()->phi())
						+ pv.y() * cos(it->gsfTrack()->phi());
				vtxDistZ_Corr = (it->vz() - pv.z())
						- ((it->vx() - pv.x()) * it->px() + (it->vy() - pv.y()) * it->py()) / it->pt() / it->pt()
								* it->pz();

				for (reco::VertexCollection::const_iterator v_it = primaryVertices->begin();
						v_it != primaryVertices->end(); ++v_it) {

					float distXY = it->gsfTrack()->dxy(v_it->position());
					float distZ = it->gsfTrack()->dz(v_it->position());
					float dist3D = sqrt(pow(distXY, 2) + pow(distZ, 2));

					if (dist3D < minVtxDist3D) {
						minVtxDist3D = dist3D;
						vtxIndex_ = int(std::distance(primaryVertices->begin(), v_it));
//						vtxDistXY_ = distXY;
						vtxDistZ_ = distZ;
					}
				}
			} else {
				edm::LogError("RootTupleMakerV2_ElectronsError") << "Error! Can't get the product " << vtxInputTag;
			}

			// Likelihood Based Ele ID ( https://twiki.cern.ch/twiki/bin/view/CMS/LikelihoodBasedEleID2011 )
			float likelihood_ = -999.;
			if (eIDValueMap[0].isValid()) {
				likelihood_ = eIDmapLikelihood[it->originalObjectRef()];
			}

			//kinematic variables
			px->push_back(it->px());
			py->push_back(it->py());
			pz->push_back(it->pz());
			energy->push_back(it->energy());
			//extra properties
			charge->push_back(it->charge());
			ecalDrivenMomentumPx->push_back(it->ecalDrivenMomentum().Px());
			ecalDrivenMomentumPy->push_back(it->ecalDrivenMomentum().Py());
			ecalDrivenMomentumPz->push_back(it->ecalDrivenMomentum().Pz());
			// ID variables
			hadronicOverEM->push_back(it->hadronicOverEm());
			EseedOverPout->push_back(it->eSeedClusterOverPout());
			eSuperClusterOverP->push_back(it->eSuperClusterOverP());
			fbrem->push_back(it->fbrem());
			sigmaEtaEta->push_back(it->sigmaEtaEta());
			sigmaIEtaIEta->push_back(it->sigmaIetaIeta());
			deltaPhiTrkSC->push_back(it->deltaPhiSuperClusterTrackAtVtx());
			deltaEtaTrkSC->push_back(it->deltaEtaSuperClusterTrackAtVtx());
			likelihood->push_back(likelihood_);
			numberOfBrems->push_back(it->numberOfBrems());
			mvaTrigV0->push_back(it->electronID("mvaTrigV0"));
			mvaNonTrigV0->push_back(it->electronID("mvaNonTrigV0"));

			//electron isolation variables
			trkIso03->push_back(it->dr03TkSumPt());
			ecalIso03->push_back(it->dr03EcalRecHitSumEt());
			hcalIso03->push_back(it->dr03HcalTowerSumEt());
			relIso03->push_back((it->dr03HcalTowerSumEt() + it->dr03EcalRecHitSumEt() + it->dr03TkSumPt()) / it->et());
			trkIso04->push_back(it->dr04TkSumPt());
			ecalIso04->push_back(it->dr04EcalRecHitSumEt());
			hcalIso04->push_back(it->dr04HcalTowerSumEt());
			relIso04->push_back((it->dr04HcalTowerSumEt() + it->dr04EcalRecHitSumEt() + it->dr04TkSumPt()) / it->et());

			//electron PF isolation variables
			if (storePFIsolation_) {
				pat::IsolationKeys isokeyPfChargedHadronIso = pat::IsolationKeys(4);
				pat::IsolationKeys isokeyPfNeutralHadronIso = pat::IsolationKeys(5);
				pat::IsolationKeys isokeyPFGammaIso = pat::IsolationKeys(6);
				pat::IsolationKeys isokeyPfPUChargedHadronIso = pat::IsolationKeys(12);

				float pfRelIso03(0), pfRelIso04(0), pfRelIso05(0);
				float directionalPFIso02(0), directionalPFIso02FallOff(0), pfIso02FallOff(0);
				float directionalPFIso03(0), directionalPFIso03FallOff(0), pfIso03FallOff(0);

				const reco::IsoDeposit * PfChargedHadronIsolation = it->isoDeposit(isokeyPfChargedHadronIso);
				const reco::IsoDeposit * PfNeutralHadronIsolation = it->isoDeposit(isokeyPfNeutralHadronIso);
				const reco::IsoDeposit * PFGammaIsolation = it->isoDeposit(isokeyPFGammaIso);
				const reco::IsoDeposit * PfPUChargedHadronIso = it->isoDeposit(isokeyPfPUChargedHadronIso);

				directionalPFIso02 = customIsolation(*it, pfCandidates, 0.2, true, false, reco::PFCandidate::e);
				directionalPFIso02FallOff = customIsolation(*it, pfCandidates, 0.2, true, true, reco::PFCandidate::e);
				pfIso02FallOff = customIsolation(*it, pfCandidates, 0.2, false, true, reco::PFCandidate::e);

				directionalPFIso03 = customIsolation(*it, pfCandidates, 0.3, true, false, reco::PFCandidate::e);
				directionalPFIso03FallOff = customIsolation(*it, pfCandidates, 0.3, true, true, reco::PFCandidate::e);
				pfIso03FallOff = customIsolation(*it, pfCandidates, 0.3, false, true, reco::PFCandidate::e);

				DirectionalPFIso02->push_back(directionalPFIso02);
				DirectionalPFIso02FallOff->push_back(directionalPFIso02FallOff);
				PfRelIso02FallOff->push_back(pfIso02FallOff);
				DirectionalPFIso03->push_back(directionalPFIso03);
				DirectionalPFIso03FallOff->push_back(directionalPFIso03FallOff);
				PfRelIso03FallOff->push_back(pfIso03FallOff);

				if (PfChargedHadronIsolation) {
					PfChargedHadronIso03->push_back(PfChargedHadronIsolation->depositWithin(0.3));
					PfChargedHadronIso04->push_back(PfChargedHadronIsolation->depositWithin(0.4));
					PfChargedHadronIso05->push_back(PfChargedHadronIsolation->depositWithin(0.5));
					pfRelIso03 += PfChargedHadronIsolation->depositWithin(0.3);
					pfRelIso04 += PfChargedHadronIsolation->depositWithin(0.4);
					pfRelIso05 += PfChargedHadronIsolation->depositWithin(0.5);

				} else
					edm::LogError("BristolNTuple_ElectronsExtraError") << "Error! Can't get the isolation deposit "
							<< "PfChargedHadronIsolation";
				if (PfNeutralHadronIsolation) {
					PfNeutralHadronIso03->push_back(PfNeutralHadronIsolation->depositWithin(0.3));
					PfNeutralHadronIso04->push_back(PfNeutralHadronIsolation->depositWithin(0.4));
					PfNeutralHadronIso05->push_back(PfNeutralHadronIsolation->depositWithin(0.5));
					pfRelIso03 += PfNeutralHadronIsolation->depositWithin(0.3);
					pfRelIso04 += PfNeutralHadronIsolation->depositWithin(0.4);
					pfRelIso05 += PfNeutralHadronIsolation->depositWithin(0.5);
				}

				else
					edm::LogError("BristolNTuple_ElectronsExtraError") << "Error! Can't get the isolation deposit "
							<< "PfNeutralHadronIsolation";
				if (PFGammaIsolation) {
					PFGammaIso03->push_back(PFGammaIsolation->depositWithin(0.3));
					PFGammaIso04->push_back(PFGammaIsolation->depositWithin(0.4));
					PFGammaIso05->push_back(PFGammaIsolation->depositWithin(0.5));
					pfRelIso03 += PFGammaIsolation->depositWithin(0.3);
					pfRelIso04 += PFGammaIsolation->depositWithin(0.4);
					pfRelIso05 += PFGammaIsolation->depositWithin(0.5);
				} else
					edm::LogError("BristolNTuple_ElectronsExtraError") << "Error! Can't get the isolation deposit "
							<< "PFGammaIsolation";
				PFRelIso03->push_back(pfRelIso03 / it->et());
				PFRelIso04->push_back(pfRelIso04 / it->et());
				PFRelIso05->push_back(pfRelIso05 / it->et());
				PFRelIso03DeltaBeta->push_back(getRelativeIsolation(*it, 0.3, rho, iEvent.isRealData(), true, false));
				PFRelIso03RhoEA->push_back(getRelativeIsolation(*it, 0.3, rho, iEvent.isRealData(), false, true));

				if (PfPUChargedHadronIso) {
					PfPUChargedHadronIso03->push_back(PfPUChargedHadronIso->depositWithin(0.3));
					PfPUChargedHadronIso04->push_back(PfPUChargedHadronIso->depositWithin(0.4));
					PfPUChargedHadronIso05->push_back(PfPUChargedHadronIso->depositWithin(0.5));
				} else
					edm::LogError("BristolNTuple_ElectronsExtraError") << "Error! Can't get the isolation deposit "
							<< "PfPUChargedHadronIso";
			}

			// Iso variables (Heep)
			ecalIsoHeep03->push_back(it->dr03EcalRecHitSumEt());
			hcalIsoD1Heep03->push_back(it->dr03HcalDepth1TowerSumEt());
			hcalIsoD2Heep03->push_back(it->dr03HcalDepth2TowerSumEt());
			trkIsoHeep03->push_back(it->dr03TkSumPt());

			ecalIsoHeep04->push_back(it->dr04EcalRecHitSumEt());
			hcalIsoD1Heep04->push_back(it->dr04HcalDepth1TowerSumEt());
			hcalIsoD2Heep04->push_back(it->dr04HcalDepth2TowerSumEt());
			trkIsoHeep04->push_back(it->dr04TkSumPt());

			// Conversion variables
			missingHits->push_back(it->gsfTrack()->trackerExpectedHitsInner().numberOfHits());
			dist_vec->push_back(it->convDist());
			dCotTheta->push_back(it->convDcot());
			conversionRadius->push_back(it->convRadius());
			hasMatchedConvPhot->push_back(matchesConv);
			shFracInnerHits->push_back(it->shFracInnerHits());
//			bool passesConversionVeto = !ConversionTools::hasMatchedConversion(it,hConversions,beamspot.position());
			passConversionVeto->push_back(it->passConversionVeto());

			// SC associated with electron
			scEta->push_back(it->superCluster()->eta());
			scPhi->push_back(it->superCluster()->phi());
			scPt->push_back(it->superCluster()->energy() / cosh(it->superCluster()->eta()));
			scP->push_back(it->eSuperClusterOverP() / it->superCluster()->energy());
			scEnergy->push_back(it->superCluster()->energy());
			scRawEnergy->push_back(it->superCluster()->rawEnergy());

			// Vertex association variables
			vtxIndex->push_back(vtxIndex_);
			vtxDistZ->push_back(vtxDistZ_);
			primaryVertexDXY->push_back(it->dB());
			primaryVertexDXYError->push_back(it->edB());
			beamSpotDXY->push_back(it->dB(pat::Electron::BS2D));
			beamSpotDXYError->push_back(it->edB(pat::Electron::BS2D));
			vtxDistZCorr->push_back(vtxDistZ_Corr);
			primaryVertexDXYCorr->push_back(vtxDistXY_Corr);

		}
	} else {
		edm::LogError("BristolNTuple_ElectronsExtraError") << "Error! Can't get the product " << inputTag;
	}

	//-----------------------------------------------------------------
	// put vectors in the event
	//kinematic variables
	iEvent.put(px, prefix + "Px" + suffix);
	iEvent.put(py, prefix + "Py" + suffix);
	iEvent.put(pz, prefix + "Pz" + suffix);
	iEvent.put(energy, prefix + "Energy" + suffix);
	//extra properties
	iEvent.put(charge, prefix + "Charge" + suffix);
	iEvent.put(ecalDrivenMomentumPx, prefix + "ecalDrivenMomentum.Px" + suffix);
	iEvent.put(ecalDrivenMomentumPy, prefix + "ecalDrivenMomentum.Py" + suffix);
	iEvent.put(ecalDrivenMomentumPz, prefix + "ecalDrivenMomentum.Pz" + suffix);
	//electron ID variables
	iEvent.put(hadronicOverEM, prefix + "HadronicOverEM" + suffix);
	iEvent.put(EseedOverPout, prefix + "eSeedClusterOverPout" + suffix);
	iEvent.put(eSuperClusterOverP, prefix + "ESuperClusterOverP" + suffix);
	iEvent.put(fbrem, prefix + "Fbrem" + suffix);
	iEvent.put(sigmaEtaEta, prefix + "SigmaEtaEta" + suffix);
	iEvent.put(sigmaIEtaIEta, prefix + "SigmaIEtaIEta" + suffix);
	iEvent.put(deltaPhiTrkSC, prefix + "DeltaPhiTrkSC" + suffix);
	iEvent.put(deltaEtaTrkSC, prefix + "DeltaEtaTrkSC" + suffix);
	iEvent.put(likelihood, prefix + "Likelihood" + suffix);
	iEvent.put(numberOfBrems, prefix + "NumberOfBrems" + suffix);
	iEvent.put(mvaTrigV0, prefix + "mvaTrigV0" + suffix);
	iEvent.put(mvaNonTrigV0, prefix + "mvaNonTrigV0" + suffix);

	//electron isolation variables
	iEvent.put(trkIso04, prefix + "TrkIso04" + suffix);
	iEvent.put(ecalIso04, prefix + "EcalIso04" + suffix);
	iEvent.put(hcalIso04, prefix + "HcalIso04" + suffix);
	iEvent.put(relIso04, prefix + "RelIso04" + suffix);

	iEvent.put(trkIso03, prefix + "TrkIso03" + suffix);
	iEvent.put(ecalIso03, prefix + "EcalIso03" + suffix);
	iEvent.put(hcalIso03, prefix + "HcalIso03" + suffix);
	iEvent.put(relIso03, prefix + "RelIso03" + suffix);

	//electron PF isolation variables
	if (storePFIsolation_) {
		iEvent.put(PfChargedHadronIso03, prefix + "PfChargedHadronIso03" + suffix);
		iEvent.put(PfNeutralHadronIso03, prefix + "PfNeutralHadronIso03" + suffix);
		iEvent.put(PFGammaIso03, prefix + "PFGammaIso03" + suffix);
		iEvent.put(PFRelIso03, prefix + "PFRelIso03" + suffix);
		iEvent.put(PFRelIso03DeltaBeta, prefix + "PFRelIso03DeltaBeta" + suffix);
		iEvent.put(PFRelIso03RhoEA, prefix + "PFRelIso03RhoEA" + suffix);

		iEvent.put(PfChargedHadronIso04, prefix + "PfChargedHadronIso04" + suffix);
		iEvent.put(PfNeutralHadronIso04, prefix + "PfNeutralHadronIso04" + suffix);
		iEvent.put(PFGammaIso04, prefix + "PFGammaIso04" + suffix);
		iEvent.put(PFRelIso04, prefix + "PFRelIso04" + suffix);

		iEvent.put(PfChargedHadronIso05, prefix + "PfChargedHadronIso05" + suffix);
		iEvent.put(PfNeutralHadronIso05, prefix + "PfNeutralHadronIso05" + suffix);
		iEvent.put(PFGammaIso05, prefix + "PFGammaIso05" + suffix);
		iEvent.put(PFRelIso05, prefix + "PFRelIso05" + suffix);

		iEvent.put(DirectionalPFIso02, prefix + "DirectionalPFIso02" + suffix);
		iEvent.put(DirectionalPFIso02FallOff, prefix + "DirectionalPFIso02FallOff" + suffix);
		iEvent.put(PfRelIso02FallOff, prefix + "PfRelIso02FallOff" + suffix);
		iEvent.put(DirectionalPFIso03, prefix + "DirectionalPFIso03" + suffix);
		iEvent.put(DirectionalPFIso03FallOff, prefix + "DirectionalPFIso03FallOff" + suffix);
		iEvent.put(PfRelIso03FallOff, prefix + "PfRelIso03FallOff" + suffix);
		iEvent.put(PfPUChargedHadronIso03, prefix + "PfPUChargedHadronIso03" + suffix);
		iEvent.put(PfPUChargedHadronIso04, prefix + "PfPUChargedHadronIso04" + suffix);
		iEvent.put(PfPUChargedHadronIso05, prefix + "PfPUChargedHadronIso05" + suffix);

	}

	//high energy electron isolation variables
	iEvent.put(ecalIsoHeep03, prefix + "EcalIsoHeep03" + suffix);
	iEvent.put(hcalIsoD1Heep03, prefix + "HcalIsoD1Heep03" + suffix);
	iEvent.put(hcalIsoD2Heep03, prefix + "HcalIsoD2Heep03" + suffix);
	iEvent.put(trkIsoHeep03, prefix + "TrkIsoHeep03" + suffix);

	iEvent.put(ecalIsoHeep04, prefix + "EcalIsoHeep04" + suffix);
	iEvent.put(hcalIsoD1Heep04, prefix + "HcalIsoD1Heep04" + suffix);
	iEvent.put(hcalIsoD2Heep04, prefix + "HcalIsoD2Heep04" + suffix);
	iEvent.put(trkIsoHeep04, prefix + "TrkIsoHeep04" + suffix);

	//electron conversion identification variables
	iEvent.put(missingHits, prefix + "MissingHits" + suffix);
	iEvent.put(dist_vec, prefix + "Dist" + suffix);
	iEvent.put(dCotTheta, prefix + "DCotTheta" + suffix);
	iEvent.put(conversionRadius, prefix + "ConvRadius" + suffix);
	iEvent.put(hasMatchedConvPhot, prefix + "HasMatchedConvPhot" + suffix);
	iEvent.put(shFracInnerHits, prefix + "shFracInnerHits" + suffix);
	iEvent.put(passConversionVeto, prefix + "passConversionVeto" + suffix);

	//calorimeter variables
	iEvent.put(scEta, prefix + "SCEta" + suffix);
	iEvent.put(scPhi, prefix + "SCPhi" + suffix);
	iEvent.put(scPt, prefix + "SCPt" + suffix);
	iEvent.put(scP, prefix + "SCP" + suffix);
	iEvent.put(scEnergy, prefix + "SCEnergy" + suffix);
	iEvent.put(scRawEnergy, prefix + "SCRawEnergy" + suffix);

	//calorimeter variables
	iEvent.put(vtxIndex, prefix + "VtxIndex" + suffix);
	iEvent.put(vtxDistZ, prefix + "VtxDistZ" + suffix);
	iEvent.put(primaryVertexDXY, prefix + "PrimaryVertexDXY" + suffix);
	iEvent.put(primaryVertexDXYError, prefix + "PrimaryVertexDXYError" + suffix);
	iEvent.put(beamSpotDXY, prefix + "BeamSpotDXY" + suffix);
	iEvent.put(beamSpotDXYError, prefix + "BeamSpotDXYError" + suffix);
	iEvent.put(vtxDistZCorr, prefix + "VtxDistZCorr" + suffix);
	iEvent.put(primaryVertexDXYCorr, prefix + "PrimaryVertexDXYCorr" + suffix);

}
