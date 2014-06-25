#include "BristolAnalysis/NTupleTools/interface/BristolNTuple_Taus.h"
#include "BristolAnalysis/NTupleTools/interface/PatUtilities.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include <DataFormats/Common/interface/Ref.h>
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/TauReco/interface/PFTau.h"

BristolNTuple_Taus::BristolNTuple_Taus(const edm::ParameterSet& iConfig) :
		inputTag(iConfig.getParameter < edm::InputTag > ("InputTag")), prefix(
				iConfig.getParameter < std::string > ("Prefix")), suffix(
				iConfig.getParameter < std::string > ("Suffix")), maxSize(iConfig.getParameter<unsigned int>("MaxSize")) {
	produces < std::vector<float> > (prefix + "Px" + suffix);
	produces < std::vector<float> > (prefix + "Py" + suffix);
	produces < std::vector<float> > (prefix + "Pz" + suffix);
	produces < std::vector<float> > (prefix + "Energy" + suffix);
	produces < std::vector<int> > (prefix + "Charge" + suffix);
	produces < std::vector<int> > (prefix + "IsPFTau" + suffix);
	produces < std::vector<int> > (prefix + "IsCaloTau" + suffix);
	produces < std::vector<int> > (prefix + "DecayMode" + suffix);
	produces < std::vector<float> > (prefix + "EmFraction" + suffix);
	produces < std::vector<float> > (prefix + "Hcal3x3OverPLead" + suffix);
	produces < std::vector<float> > (prefix + "HcalMaxOverPLead" + suffix);
	produces < std::vector<float> > (prefix + "HcalTotOverPLead" + suffix);
	produces < std::vector<float> > (prefix + "IsolationPFChargedHadrCandsPtSum" + suffix);
	produces < std::vector<float> > (prefix + "IsolationPFGammaCandsEtSum" + suffix);
	produces < std::vector<float> > (prefix + "LeadPFChargedHadrCandsignedSipt" + suffix);
//	produces < std::vector<float> > (prefix + "EtaLeadCharged" + suffix);
//	produces < std::vector<float> > (prefix + "PhiLeadCharged" + suffix);
//	produces < std::vector<float> > (prefix + "PtLeadCharged" + suffix);
	produces < std::vector<int> > (prefix + "AgainstElectronMVADiscr" + suffix);
	produces < std::vector<int> > (prefix + "AgainstMuonMediumDiscr" + suffix);
	produces < std::vector<int> > (prefix + "ByMediumIsolationDiscr" + suffix);
//	produces < std::vector<int> > (prefix + "ByIsolationUsingLeadingPionDiscr" + suffix);
//	produces < std::vector<int> > (prefix + "LeadingPionPtCutDiscr" + suffix);
//	produces < std::vector<int> > (prefix + "LeadingTrackFindingDiscr" + suffix);
//	produces < std::vector<int> > (prefix + "LeadingTrackPtCutDiscr" + suffix);
//	produces < std::vector<int> > (prefix + "TrackIsolationDiscr" + suffix);
//	produces < std::vector<int> > (prefix + "TrackIsolationUsingLeadingPionDiscr" + suffix);
//	produces < std::vector<int> > (prefix + "EcalIsolationDiscr" + suffix);
//	produces < std::vector<int> > (prefix + "EcalIsolationUsingLeadingPionDiscr" + suffix);

}

void BristolNTuple_Taus::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

	std::auto_ptr < std::vector<float> > px(new std::vector<float>());
	std::auto_ptr < std::vector<float> > py(new std::vector<float>());
	std::auto_ptr < std::vector<float> > pz(new std::vector<float>());
	std::auto_ptr < std::vector<float> > energy(new std::vector<float>());
	std::auto_ptr < std::vector<int> > charge(new std::vector<int>());
	std::auto_ptr < std::vector<int> > ispftau(new std::vector<int>());
	std::auto_ptr < std::vector<int> > iscalotau(new std::vector<int>());
	std::auto_ptr < std::vector<int> > decaymode(new std::vector<int>());
	std::auto_ptr < std::vector<float> > emfraction(new std::vector<float>());
	std::auto_ptr < std::vector<float> > hcal3x3overplead(new std::vector<float>());
	std::auto_ptr < std::vector<float> > hcalmaxoverplead(new std::vector<float>());
	std::auto_ptr < std::vector<float> > hcaltotoverplead(new std::vector<float>());
	std::auto_ptr < std::vector<float> > isolationpfchargedhadrcandsptsum(new std::vector<float>());
	std::auto_ptr < std::vector<float> > isolationpfgammacandsetsum(new std::vector<float>());
	std::auto_ptr < std::vector<float> > leadpfchargedhadrcandsignedsipt(new std::vector<float>());
//	std::auto_ptr < std::vector<float> > etaleadcharged(new std::vector<float>());
//	std::auto_ptr < std::vector<float> > phileadcharged(new std::vector<float>());
//	std::auto_ptr < std::vector<float> > ptleadcharged(new std::vector<float>());
	std::auto_ptr < std::vector<int> > againstElectronMVADiscr(new std::vector<int>());
	std::auto_ptr < std::vector<int> > againstMuonMediumDiscr(new std::vector<int>());
//	std::auto_ptr < std::vector<int> > byisolationusingleadingpiondiscr(new std::vector<int>());
//	std::auto_ptr < std::vector<int> > leadingpionptcutdiscr(new std::vector<int>());
//	std::auto_ptr < std::vector<int> > leadingtrackfindingdiscr(new std::vector<int>());
//	std::auto_ptr < std::vector<int> > leadingtrackptcutdiscr(new std::vector<int>());
//	std::auto_ptr < std::vector<int> > trackisolationdiscr(new std::vector<int>());
//	std::auto_ptr < std::vector<int> > trackisolationusingleadingpiondiscr(new std::vector<int>());
//	std::auto_ptr < std::vector<int> > ecalisolationdiscr(new std::vector<int>());
//	std::auto_ptr < std::vector<int> > ecalisolationusingleadingpiondiscr(new std::vector<int>());
	std::auto_ptr < std::vector<int> > byMediumIsolationDiscr(new std::vector<int>());

	edm::Handle < std::vector<pat::Tau> > taus;
	iEvent.getByLabel(inputTag, taus);

	if (taus.isValid()) {
		edm::LogInfo("BristolNTuple_TausInfo") << "Total # Taus: " << taus->size();

		std::vector<pat::Tau>::const_iterator it = taus->begin();
		std::vector<pat::Tau>::const_iterator it_end = taus->end();
		//
		//
		for (; it != it_end; ++it) {
			if (px->size() > maxSize)
				break;
			//
			// Discriminators are defined in:
			// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/PhysicsTools/PatAlgos/python/producersLayer1/tauProducer_cfi.py?revision=1.27
			/* ID's in 2012 CMSSW 52X changed:
			 * The available IDs are: 'againstElectronLoose' 'againstElectronMVA' 'againstElectronMedium'
			 * 'againstElectronTight' 'againstMuonLoose' 'againstMuonMedium' 'againstMuonTight'
			 * 'byLooseCombinedIsolationDeltaBetaCorr' 'byLooseIsolation' 'byLooseIsolationDeltaBetaCorr'
			 *  'byMediumCombinedIsolationDeltaBetaCorr' 'byMediumIsolation' 'byMediumIsolationDeltaBetaCorr'
			 *   'byTightCombinedIsolationDeltaBetaCorr' 'byTightIsolation' 'byTightIsolationDeltaBetaCorr'
			 *   'byVLooseCombinedIsolationDeltaBetaCorr' 'byVLooseIsolation' 'byVLooseIsolationDeltaBetaCorr'
			 *   'decayModeFinding'
			 *
			 *   IDs in 53X git version
			 *   'againstElectronDeadECAL' 'againstElectronLoose' 'againstElectronLooseMVA3'
			 *   'againstElectronMVA3category' 'againstElectronMVA3raw' 'againstElectronMedium'
			 *   'againstElectronMediumMVA3' 'againstElectronTight' 'againstElectronTightMVA3'
			 *   'againstElectronVTightMVA3' 'againstMuonLoose' 'againstMuonLoose2' 'againstMuonLoose3'
			 *   'againstMuonMedium' 'againstMuonMedium2' 'againstMuonTight' 'againstMuonTight2'
			 *   'againstMuonTight3' 'byCombinedIsolationDeltaBetaCorrRaw' 'byCombinedIsolationDeltaBetaCorrRaw3Hits'
			 *   'byIsolationMVA2raw' 'byIsolationMVAraw' 'byLooseCombinedIsolationDeltaBetaCorr'
			 *   'byLooseCombinedIsolationDeltaBetaCorr3Hits' 'byLooseIsolation' 'byLooseIsolationDeltaBetaCorr'
			 *   'byLooseIsolationMVA' 'byLooseIsolationMVA2' 'byMediumCombinedIsolationDeltaBetaCorr'
			 *   'byMediumCombinedIsolationDeltaBetaCorr3Hits' 'byMediumIsolation' 'byMediumIsolationDeltaBetaCorr'
			 *   'byMediumIsolationMVA' 'byMediumIsolationMVA2' 'byTightCombinedIsolationDeltaBetaCorr'
			 *   'byTightCombinedIsolationDeltaBetaCorr3Hits' 'byTightIsolation' 'byTightIsolationDeltaBetaCorr'
			 *    'byTightIsolationMVA' 'byTightIsolationMVA2' 'byVLooseCombinedIsolationDeltaBetaCorr'
			 *    'byVLooseIsolation' 'byVLooseIsolationDeltaBetaCorr' 'decayModeFinding'
			 */
			againstElectronMVADiscr->push_back(it->tauID("againstElectronMVA3raw") > 0.5 ? 1 : 0);
			againstMuonMediumDiscr->push_back(it->tauID("againstMuonMedium") > 0.5 ? 1 : 0);
			byMediumIsolationDiscr->push_back(it->tauID("byMediumIsolation") > 0.5 ? 1 : 0);

			//old, not working
//			byisolationusingleadingpiondiscr->push_back(it->tauID("byIsolationUsingLeadingPion") > 0.5 ? 1 : 0);
//			leadingpionptcutdiscr->push_back(it->tauID("leadingPionPtCut") > 0.5 ? 1 : 0);
//			leadingtrackfindingdiscr->push_back(it->tauID("leadingTrackFinding") > 0.5 ? 1 : 0);
//			leadingtrackptcutdiscr->push_back(it->tauID("leadingTrackPtCut") > 0.5 ? 1 : 0);
//			trackisolationdiscr->push_back(it->tauID("trackIsolation") > 0.5 ? 1 : 0);
//			trackisolationusingleadingpiondiscr->push_back(it->tauID("trackIsolationUsingLeadingPion") > 0.5 ? 1 : 0);
//			ecalisolationdiscr->push_back(it->tauID("ecalIsolation") > 0.5 ? 1 : 0);
//			ecalisolationusingleadingpiondiscr->push_back(it->tauID("ecalIsolationUsingLeadingPion") > 0.5 ? 1 : 0);

			//
			px->push_back(it->px());
			py->push_back(it->py());
			pz->push_back(it->pz());
			energy->push_back((float) (it->energy()));
			charge->push_back((int) (it->charge()));
			if (it->isPFTau()) {
				ispftau->push_back(1);
			}
			if (!it->isPFTau()) {
				ispftau->push_back(0);
			}
			if (it->isCaloTau()) {
				iscalotau->push_back(1);
			}
			if (!it->isCaloTau()) {
				iscalotau->push_back(0);
			}
			decaymode->push_back((float) (it->decayMode()));
			emfraction->push_back((float) (it->emFraction()));
			hcal3x3overplead->push_back((float) (it->hcal3x3OverPLead()));
			hcalmaxoverplead->push_back((float) (it->hcalMaxOverPLead()));
			hcaltotoverplead->push_back((float) (it->hcalTotOverPLead()));
			isolationpfchargedhadrcandsptsum->push_back((float) (it->isolationPFChargedHadrCandsPtSum()));
			isolationpfgammacandsetsum->push_back((float) (it->isolationPFGammaCandsEtSum()));
			leadpfchargedhadrcandsignedsipt->push_back((float) (it->leadPFChargedHadrCandsignedSipt()));
			//causing exceptions
//			reco::PFCandidateRef leadPFChargedHadrCand_Ref = it->leadPFChargedHadrCand();
//			etaleadcharged->push_back((float) (leadPFChargedHadrCand_Ref->eta()));
//			phileadcharged->push_back((float) (leadPFChargedHadrCand_Ref->phi()));
//			ptleadcharged->push_back((float) (leadPFChargedHadrCand_Ref->pt()));
			//
		}
	} else {
		edm::LogError("BristolNTuple_TausError") << "Error! Can't get the product " << inputTag;
	}

	iEvent.put(px, prefix + "Px" + suffix);
	iEvent.put(py, prefix + "Py" + suffix);
	iEvent.put(pz, prefix + "Pz" + suffix);
	iEvent.put(energy, prefix + "Energy" + suffix);
	iEvent.put(charge, prefix + "Charge" + suffix);
	iEvent.put(ispftau, prefix + "IsPFTau" + suffix);
	iEvent.put(iscalotau, prefix + "IsCaloTau" + suffix);
	iEvent.put(decaymode, prefix + "DecayMode" + suffix);
	iEvent.put(emfraction, prefix + "EmFraction" + suffix);
	iEvent.put(hcal3x3overplead, prefix + "Hcal3x3OverPLead" + suffix);
	iEvent.put(hcalmaxoverplead, prefix + "HcalMaxOverPLead" + suffix);
	iEvent.put(hcaltotoverplead, prefix + "HcalTotOverPLead" + suffix);
	iEvent.put(isolationpfchargedhadrcandsptsum, prefix + "IsolationPFChargedHadrCandsPtSum" + suffix);
	iEvent.put(isolationpfgammacandsetsum, prefix + "IsolationPFGammaCandsEtSum" + suffix);
	iEvent.put(leadpfchargedhadrcandsignedsipt, prefix + "LeadPFChargedHadrCandsignedSipt" + suffix);
//	iEvent.put(etaleadcharged, prefix + "EtaLeadCharged" + suffix);
//	iEvent.put(phileadcharged, prefix + "PhiLeadCharged" + suffix);
//	iEvent.put(ptleadcharged, prefix + "PtLeadCharged" + suffix);
	iEvent.put(againstElectronMVADiscr, prefix + "AgainstElectronMVADiscr" + suffix);
	iEvent.put(againstMuonMediumDiscr, prefix + "AgainstMuonMediumDiscr" + suffix);
	iEvent.put(byMediumIsolationDiscr, prefix + "ByMediumIsolationDiscr" + suffix);
//	iEvent.put(byisolationusingleadingpiondiscr, prefix + "ByIsolationUsingLeadingPionDiscr" + suffix);
//	iEvent.put(leadingpionptcutdiscr, prefix + "LeadingPionPtCutDiscr" + suffix);
//	iEvent.put(leadingtrackfindingdiscr, prefix + "LeadingTrackFindingDiscr" + suffix);
//	iEvent.put(leadingtrackptcutdiscr, prefix + "LeadingTrackPtCutDiscr" + suffix);
//	iEvent.put(trackisolationdiscr, prefix + "TrackIsolationDiscr" + suffix);
//	iEvent.put(trackisolationusingleadingpiondiscr, prefix + "TrackIsolationUsingLeadingPionDiscr" + suffix);
//	iEvent.put(ecalisolationdiscr, prefix + "EcalIsolationDiscr" + suffix);
//	iEvent.put(ecalisolationusingleadingpiondiscr, prefix + "EcalIsolationUsingLeadingPionDiscr" + suffix);

}
