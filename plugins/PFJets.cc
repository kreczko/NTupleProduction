#include "BristolAnalysis/NTupleTools/interface/BristolNTuple_PFJets.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include <iostream>

BristolNTuple_PFJets::BristolNTuple_PFJets(const edm::ParameterSet& iConfig) :
		inputTag(iConfig.getParameter < edm::InputTag > ("InputTag")), //
		prefix(iConfig.getParameter < std::string > ("Prefix")), //
		suffix(iConfig.getParameter < std::string > ("Suffix")), //
		maxSize(iConfig.getParameter<unsigned int>("MaxSize")), //
		jecUncPath(iConfig.getParameter < std::string > ("JECUncertainty")), //
		readJECuncertainty(iConfig.getParameter<bool>("ReadJECuncertainty")), //
		doVertexAssociation(iConfig.getParameter<bool>("DoVertexAssociation")), //
		vtxInputTag(iConfig.getParameter < edm::InputTag > ("VertexInputTag")), //
		isRealData(iConfig.getParameter<bool>("isRealData")) {
	//kinematic variables
	produces < std::vector<float> > (prefix + "Px" + suffix);
	produces < std::vector<float> > (prefix + "Py" + suffix);
	produces < std::vector<float> > (prefix + "Pz" + suffix);
	produces < std::vector<float> > (prefix + "Energy" + suffix);
	//kinematic variables before corrections
	produces < std::vector<float> > (prefix + "PxRAW" + suffix);
	produces < std::vector<float> > (prefix + "PyRAW" + suffix);
	produces < std::vector<float> > (prefix + "PzRAW" + suffix);
	produces < std::vector<float> > (prefix + "EnergyRAW" + suffix);
	//extra properties
	produces < std::vector<float> > (prefix + "Charge" + suffix);
	produces < std::vector<float> > (prefix + "Mass" + suffix);
	produces < std::vector<int> > (prefix + "PartonFlavour" + suffix);
	//generated jet properties
        if (!isRealData) {
            produces < std::vector<float> > (prefix + "GenJet.Energy" + suffix);
	    produces < std::vector<float> > (prefix + "GenJet.Pt" + suffix);
	    produces < std::vector<float> > (prefix + "GenJet.Px" + suffix);
	    produces < std::vector<float> > (prefix + "GenJet.Py" + suffix);
	    produces < std::vector<float> > (prefix + "GenJet.Pz" + suffix);
	    produces < std::vector<float> > (prefix + "GenJet.Eta" + suffix);
	    produces < std::vector<float> > (prefix + "GenJet.Phi" + suffix);
        }
	//jet energy correction and uncertainties
	produces < std::vector<float> > (prefix + "JECUnc" + suffix);
	produces < std::vector<float> > (prefix + "L2L3ResJEC" + suffix);
	produces < std::vector<float> > (prefix + "L3AbsJEC" + suffix);
	produces < std::vector<float> > (prefix + "L2RelJEC" + suffix);
	produces < std::vector<float> > (prefix + "L1OffJEC" + suffix);
	//jet ID variables
	produces < std::vector<float> > (prefix + "ChargedEmEnergyFraction" + suffix);
	produces < std::vector<float> > (prefix + "ChargedHadronEnergyFraction" + suffix);
	produces < std::vector<float> > (prefix + "ChargedMuEnergyFraction" + suffix);
	produces < std::vector<float> > (prefix + "ElectronEnergyFraction" + suffix);
	produces < std::vector<float> > (prefix + "MuonEnergyFraction" + suffix);
	produces < std::vector<float> > (prefix + "NeutralEmEnergyFraction" + suffix);
	produces < std::vector<float> > (prefix + "NeutralHadronEnergyFraction" + suffix);
	produces < std::vector<float> > (prefix + "PhotonEnergyFraction" + suffix);
	produces < std::vector<int> > (prefix + "ChargedHadronMultiplicity" + suffix);
	produces < std::vector<int> > (prefix + "ChargedMultiplicity" + suffix);
	produces < std::vector<int> > (prefix + "ElectronMultiplicity" + suffix);
	produces < std::vector<int> > (prefix + "MuonMultiplicity" + suffix);
	produces < std::vector<int> > (prefix + "NeutralHadronMultiplicity" + suffix);
	produces < std::vector<int> > (prefix + "NeutralMultiplicity" + suffix);
	produces < std::vector<int> > (prefix + "PhotonMultiplicity" + suffix);
	produces < std::vector<int> > (prefix + "NConstituents" + suffix);
	produces < std::vector<float> > (prefix + "ChargedEmEnergyFractionRAW" + suffix);
	produces < std::vector<float> > (prefix + "ChargedHadronEnergyFractionRAW" + suffix);
	produces < std::vector<float> > (prefix + "NeutralEmEnergyFractionRAW" + suffix);
	produces < std::vector<float> > (prefix + "NeutralHadronEnergyFractionRAW" + suffix);
	produces < std::vector<int> > (prefix + "PassLooseID" + suffix);
	produces < std::vector<int> > (prefix + "PassTightID" + suffix);
	//b-tagging information
	produces < std::vector<float> > (prefix + "TrackCountingHighEffBTag" + suffix);
	produces < std::vector<float> > (prefix + "TrackCountingHighPurBTag" + suffix);
	produces < std::vector<float> > (prefix + "SimpleSecondaryVertexHighEffBTag" + suffix);
	produces < std::vector<float> > (prefix + "SimpleSecondaryVertexHighPurBTag" + suffix);
	produces < std::vector<float> > (prefix + "JetProbabilityBTag" + suffix);
	produces < std::vector<float> > (prefix + "JetBProbabilityBTag" + suffix);

	produces < std::vector<float> > (prefix + "SoftMuonBJetTag" + suffix);
	produces < std::vector<float> > (prefix + "SoftMuonByIP3dBJetTag" + suffix);
	produces < std::vector<float> > (prefix + "SoftMuonByPtBJetTag" + suffix);
	produces < std::vector<float> > (prefix + "CombinedSecondaryVertexMVABJetTag" + suffix);
	produces < std::vector<float> > (prefix + "CombinedSecondaryVertexBJetTag" + suffix);

	//jet-vertex association
	if (doVertexAssociation) {
		produces < std::vector<float> > (prefix + "BestVertexTrackAssociationFactor" + suffix);
		produces < std::vector<int> > (prefix + "BestVertexTrackAssociationIndex" + suffix);
		produces < std::vector<float> > (prefix + "ClosestVertexWeighted3DSeparation" + suffix);
		produces < std::vector<float> > (prefix + "ClosestVertexWeightedXYSeparation" + suffix);
		produces < std::vector<float> > (prefix + "ClosestVertexWeightedZSeparation" + suffix);
		produces < std::vector<int> > (prefix + "ClosestVertex3DIndex" + suffix);
		produces < std::vector<int> > (prefix + "ClosestVertexXYIndex" + suffix);
		produces < std::vector<int> > (prefix + "ClosestVertexZIndex" + suffix);
	}

}

void BristolNTuple_PFJets::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
	//kinematic variables
	std::auto_ptr < std::vector<float> > px(new std::vector<float>());
	std::auto_ptr < std::vector<float> > py(new std::vector<float>());
	std::auto_ptr < std::vector<float> > pz(new std::vector<float>());
	std::auto_ptr < std::vector<float> > energy(new std::vector<float>());
	//kinematic variables before corrections
	std::auto_ptr < std::vector<float> > px_raw(new std::vector<float>());
	std::auto_ptr < std::vector<float> > py_raw(new std::vector<float>());
	std::auto_ptr < std::vector<float> > pz_raw(new std::vector<float>());
	std::auto_ptr < std::vector<float> > energy_raw(new std::vector<float>());
	//extra properties
	std::auto_ptr < std::vector<float> > charge(new std::vector<float>());
	std::auto_ptr < std::vector<float> > mass(new std::vector<float>());
	std::auto_ptr < std::vector<int> > partonFlavour(new std::vector<int>());
	//generated jet properties
	std::auto_ptr < std::vector<float> > genJet_energy(new std::vector<float>());
	std::auto_ptr < std::vector<float> > genJet_pt(new std::vector<float>());
	std::auto_ptr < std::vector<float> > genJet_px(new std::vector<float>());
	std::auto_ptr < std::vector<float> > genJet_py(new std::vector<float>());
	std::auto_ptr < std::vector<float> > genJet_pz(new std::vector<float>());
	std::auto_ptr < std::vector<float> > genJet_eta(new std::vector<float>());
	std::auto_ptr < std::vector<float> > genJet_phi(new std::vector<float>());
	//jet energy correction and uncertainties
	std::auto_ptr < std::vector<float> > jecUnc_vec(new std::vector<float>());
	std::auto_ptr < std::vector<float> > l2l3resJEC_vec(new std::vector<float>());
	std::auto_ptr < std::vector<float> > l3absJEC_vec(new std::vector<float>());
	std::auto_ptr < std::vector<float> > l2relJEC_vec(new std::vector<float>());
	std::auto_ptr < std::vector<float> > l1offJEC_vec(new std::vector<float>());
	//jet ID variables
	std::auto_ptr < std::vector<float> > chargedEmEnergyFraction(new std::vector<float>());
	std::auto_ptr < std::vector<float> > chargedHadronEnergyFraction(new std::vector<float>());
	std::auto_ptr < std::vector<float> > chargedMuEnergyFraction(new std::vector<float>());
	std::auto_ptr < std::vector<float> > electronEnergyFraction(new std::vector<float>());
	std::auto_ptr < std::vector<float> > muonEnergyFraction(new std::vector<float>());
	std::auto_ptr < std::vector<float> > neutralEmEnergyFraction(new std::vector<float>());
	std::auto_ptr < std::vector<float> > neutralHadronEnergyFraction(new std::vector<float>());
	std::auto_ptr < std::vector<float> > photonEnergyFraction(new std::vector<float>());
	std::auto_ptr < std::vector<int> > chargedHadronMultiplicity(new std::vector<int>());
	std::auto_ptr < std::vector<int> > chargedMultiplicity(new std::vector<int>());
	std::auto_ptr < std::vector<int> > electronMultiplicity(new std::vector<int>());
	std::auto_ptr < std::vector<int> > muonMultiplicity(new std::vector<int>());
	std::auto_ptr < std::vector<int> > neutralHadronMultiplicity(new std::vector<int>());
	std::auto_ptr < std::vector<int> > neutralMultiplicity(new std::vector<int>());
	std::auto_ptr < std::vector<int> > photonMultiplicity(new std::vector<int>());
	std::auto_ptr < std::vector<int> > nConstituents(new std::vector<int>());
	std::auto_ptr < std::vector<float> > chargedEmEnergyFractionRAW(new std::vector<float>());
	std::auto_ptr < std::vector<float> > chargedHadronEnergyFractionRAW(new std::vector<float>());
	std::auto_ptr < std::vector<float> > neutralEmEnergyFractionRAW(new std::vector<float>());
	std::auto_ptr < std::vector<float> > neutralHadronEnergyFractionRAW(new std::vector<float>());
	std::auto_ptr < std::vector<int> > passLooseID(new std::vector<int>());
	std::auto_ptr < std::vector<int> > passTightID(new std::vector<int>());
	//b-tagging information
	std::auto_ptr < std::vector<float> > trackCountingHighEffBTag(new std::vector<float>());
	std::auto_ptr < std::vector<float> > trackCountingHighPurBTag(new std::vector<float>());
	std::auto_ptr < std::vector<float> > simpleSecondaryVertexHighEffBTag(new std::vector<float>());
	std::auto_ptr < std::vector<float> > simpleSecondaryVertexHighPurBTag(new std::vector<float>());
	std::auto_ptr < std::vector<float> > jetProbabilityBTag(new std::vector<float>());
	std::auto_ptr < std::vector<float> > jetBProbabilityBTag(new std::vector<float>());

	std::auto_ptr < std::vector<float> > softMuonBJetTag(new std::vector<float>());
	std::auto_ptr < std::vector<float> > softMuonByIP3dBJetTags(new std::vector<float>());
	std::auto_ptr < std::vector<float> > softMuonByPtBJetTags(new std::vector<float>());

	std::auto_ptr < std::vector<float> > combinedSecondaryVertexBJetTags(new std::vector<float>());
	std::auto_ptr < std::vector<float> > combinedSecondaryVertexMVABJetTag(new std::vector<float>());

	//jet-vertex association
	std::auto_ptr < std::vector<float> > bestVertexTrackAssociationFactor(new std::vector<float>());
	std::auto_ptr < std::vector<int> > bestVertexTrackAssociationIndex(new std::vector<int>());
	std::auto_ptr < std::vector<float> > closestVertexWeighted3DSeparation(new std::vector<float>());
	std::auto_ptr < std::vector<float> > closestVertexWeightedXYSeparation(new std::vector<float>());
	std::auto_ptr < std::vector<float> > closestVertexWeightedZSeparation(new std::vector<float>());
	std::auto_ptr < std::vector<int> > closestVertex3DIndex(new std::vector<int>());
	std::auto_ptr < std::vector<int> > closestVertexXYIndex(new std::vector<int>());
	std::auto_ptr < std::vector<int> > closestVertexZIndex(new std::vector<int>());

	JetCorrectionUncertainty *jecUnc = 0;
	if (readJECuncertainty) {
		//(See https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/1075/1.html
		// and https://hypernews.cern.ch/HyperNews/CMS/get/physTools/2367/1.html)
		// handle the jet corrector parameters collection
		edm::ESHandle < JetCorrectorParametersCollection > JetCorParColl;
		// get the jet corrector parameters collection from the global tag
		iSetup.get<JetCorrectionsRecord>().get(jecUncPath, JetCorParColl);
		// get the uncertainty parameters from the collection
		JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
		// instantiate the jec uncertainty object
		jecUnc = new JetCorrectionUncertainty(JetCorPar);
	}

	PFJetIDSelectionFunctor pfjetIDLoose(PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE);
	PFJetIDSelectionFunctor pfjetIDTight(PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::TIGHT);

	pat::strbitset retpf = pfjetIDLoose.getBitTemplate();

	edm::Handle < std::vector<pat::Jet> > jets;
	iEvent.getByLabel(inputTag, jets);

	edm::Handle < reco::VertexCollection > primaryVertices; // DB
	iEvent.getByLabel(vtxInputTag, primaryVertices); // DB

	if (jets.isValid()) {
		edm::LogInfo("BristolNTuple_PFJetsInfo") << "Total # PFJets: " << jets->size();

		for (std::vector<pat::Jet>::const_iterator it = jets->begin(); it != jets->end(); ++it) {
			// exit from loop when you reach the required number of jets
			if (px->size() >= maxSize)
				break;

			retpf.set(false);
			int passjetLoose = 0;
			if (pfjetIDLoose(*it, retpf))
				passjetLoose = 1;

			retpf.set(false);
			int passjetTight = 0;
			if (pfjetIDTight(*it, retpf))
				passjetTight = 1;

			if (readJECuncertainty) {
				jecUnc->setJetEta(it->eta());
				jecUnc->setJetPt(it->pt()); // the uncertainty is a function of the corrected pt
			}

			if (!iEvent.isRealData()) {
				// Store generated jet resolutions for monte carlo
				float genjet_energy(0);
				float genjet_pt(0);
				float genjet_px(0);
				float genjet_py(0);
				float genjet_pz(0);
				float genjet_eta(0);
				float genjet_phi(0);
				// take only jets with corrected pt>10 according to: https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiTopRefSyst#Jet_energy_resolution
				if (it->pt() > 10) {
					if (it->genJet()) { //matching (stop segmentation fault due to jet having no associated generator jet)
						genjet_energy = it->genJet()->energy();
						genjet_pt = it->genJet()->pt();
						genjet_px = it->genJet()->px();
						genjet_py = it->genJet()->py();
						genjet_pz = it->genJet()->pz();
						genjet_eta = it->genJet()->eta();
						genjet_phi = it->genJet()->phi();
					}
				}
				//generated jet properties
				genJet_energy->push_back(genjet_energy);
				genJet_pt->push_back(genjet_pt);
				genJet_px->push_back(genjet_px);
				genJet_py->push_back(genjet_py);
				genJet_pz->push_back(genjet_pz);
				genJet_eta->push_back(genjet_eta);
				genJet_phi->push_back(genjet_phi);
			}

			// Vertex association

			int bestVtxIndex3Ddist = -1;
			int bestVtxIndexXYdist = -1;
			int bestVtxIndexZdist = -1;

			int bestVtxIndexSharedTracks = -1;

			float minVtxDist3D = 999999.;
			float minVtxDistXY = -99999.;
			float minVtxDistZ = -99999.;
			float maxTrackAssocRatio = -9999.;

			// Loop on primary Vertices and jets and perform associations

			if (primaryVertices.isValid()) {
				edm::LogInfo("BristolNTuple_PFJetsInfo") << "Total # Primary Vertices: " << primaryVertices->size();
				if (doVertexAssociation) {
					// Main Vertex Loop
					for (reco::VertexCollection::const_iterator v_it = primaryVertices->begin();
							v_it != primaryVertices->end(); ++v_it) {

						float sumweights = 0.0;
						float dist3Dweighted = 0.0;
						float distXYweighted = 0.0;
						float distZweighted = 0.0;
						float assocsumpttracks = 0.0;
						float trackassociationratio = 0.000001;

						// Loop on tracks in jet, calculate PT weighted 3D distance to vertex and PT weighted shared track ratio
						const reco::TrackRefVector &jtracks = it->associatedTracks();
						for (reco::TrackRefVector::const_iterator jtIt = jtracks.begin(); jtIt != jtracks.end();
								++jtIt) {
							if (jtIt->isNull())
								continue;
							const reco::Track *jtrack = jtIt->get();
							float trackptweight = jtrack->pt();
							sumweights += trackptweight;

							// Weighted Distance Calculation
							float distXY = jtrack->dxy(v_it->position());
							float distZ = jtrack->dz(v_it->position());
							dist3Dweighted = trackptweight * (sqrt(pow(distXY, 2) + pow(distZ, 2)));
							distXYweighted = trackptweight * distXY;
							distZweighted = trackptweight * distZ;

							// Loop on vertex tracks, find PT weighted shared tracks.
							for (reco::Vertex::trackRef_iterator vtIt = v_it->tracks_begin();
									vtIt != v_it->tracks_end(); ++vtIt) {
								if (vtIt->isNull())
									continue;
								const reco::Track *vtrack = vtIt->get();
								if (vtrack != jtrack)
									continue;
								assocsumpttracks += jtrack->pt();
								break;
							}

							trackassociationratio = assocsumpttracks / sumweights;

						}

						// Divide distances by sum of weights.
						dist3Dweighted = dist3Dweighted / sumweights;
						distXYweighted = distXYweighted / sumweights;
						distZweighted = distZweighted / sumweights;

						// Find vertex with minimum weighted distance.
						if (dist3Dweighted < minVtxDist3D) {
							minVtxDist3D = dist3Dweighted;
							bestVtxIndex3Ddist = int(std::distance(primaryVertices->begin(), v_it));

						}

						if (distXYweighted < minVtxDistXY) {
							minVtxDistXY = distXYweighted;
							bestVtxIndexXYdist = int(std::distance(primaryVertices->begin(), v_it));
						}

						if (distZweighted < minVtxDistZ) {
							minVtxDistZ = distZweighted;
							bestVtxIndexZdist = int(std::distance(primaryVertices->begin(), v_it));
						}

						// Find vertex with minimum weighted distance.
						if (trackassociationratio > maxTrackAssocRatio) {
							maxTrackAssocRatio = trackassociationratio;
							bestVtxIndexSharedTracks = int(std::distance(primaryVertices->begin(), v_it));
						}

						//std::cout<<dist3Dweighted<<"  "<<distXYweighted<<"  "<<distZweighted<<"  "<<trackassociationratio<<"  "<<int(std::distance(primaryVertices->begin(),v_it))<<std::endl;

					}
				}
				//std::cout<<"---------------------"<<std::endl;
			} else {
				edm::LogError("BristolNTuple_PFJetsError") << "Error! Can't get the product " << vtxInputTag;
			}

			// fill in all the vectors
			//kinematic variables
			px->push_back(it->px());
			py->push_back(it->py());
			pz->push_back(it->pz());
			energy->push_back(it->energy());
			//kinematic variables before corrections
			px_raw->push_back(it->correctedJet("Uncorrected").px());
			py_raw->push_back(it->correctedJet("Uncorrected").py());
			pz_raw->push_back(it->correctedJet("Uncorrected").pz());
			energy_raw->push_back(it->correctedJet("Uncorrected").energy());
			//extra properties
			charge->push_back(it->jetCharge());
			mass->push_back(it->mass());
			partonFlavour->push_back(it->partonFlavour());

			//jet energy correction and uncertainties
			if (readJECuncertainty)
				jecUnc_vec->push_back(jecUnc->getUncertainty(true));
			else
				jecUnc_vec->push_back(-999);
			l2l3resJEC_vec->push_back(it->pt() / it->correctedJet("L3Absolute").pt());
			l3absJEC_vec->push_back(it->correctedJet("L3Absolute").pt() / it->correctedJet("L2Relative").pt());
			l2relJEC_vec->push_back(it->correctedJet("L2Relative").pt() / it->correctedJet("L1FastJet").pt());
			l1offJEC_vec->push_back(it->correctedJet("L1FastJet").pt() / it->correctedJet("Uncorrected").pt());
			//TODO: add absolute JEC factor:
			/*// get a copy of the uncorrected p4
			 reco::Candidate::LorentzVector uncorrJet = ijet->correctedP4(0);
			 // Then get the correction (L1+L2+L3 [+L2L3 for data])
			 jec_->setJetEta( uncorrJet.eta() );
			 jec_->setJetPt ( uncorrJet.pt() );
			 jec_->setJetE  ( uncorrJet.energy() );
			 jec_->setJetA  ( ijet->jetArea() );
			 jec_->setRho   ( *(rhoHandle.product()) );
			 jec_->setNPV   ( pvHandle->size() );
			 float corr = jec_->getCorrection();
			 // Here will be the working variable for all the jet energy effects
			 reco::Candidate::LorentzVector scaledJetP4 = uncorrJet * corr; */

			//jet ID variables
			chargedEmEnergyFraction->push_back(it->chargedEmEnergyFraction());
			chargedHadronEnergyFraction->push_back(it->chargedHadronEnergyFraction());
			chargedMuEnergyFraction->push_back(it->chargedMuEnergyFraction());
			electronEnergyFraction->push_back(it->electronEnergy() / it->energy());
			muonEnergyFraction->push_back(it->muonEnergyFraction());
			neutralEmEnergyFraction->push_back(it->neutralEmEnergyFraction());
			neutralHadronEnergyFraction->push_back(it->neutralHadronEnergyFraction());
			photonEnergyFraction->push_back(it->photonEnergyFraction());
			chargedHadronMultiplicity->push_back(it->chargedHadronMultiplicity());
			chargedMultiplicity->push_back(it->chargedMultiplicity());
			electronMultiplicity->push_back(it->electronMultiplicity());
			muonMultiplicity->push_back(it->muonMultiplicity());
			neutralHadronMultiplicity->push_back(it->neutralHadronMultiplicity());
			neutralMultiplicity->push_back(it->neutralMultiplicity());
			photonMultiplicity->push_back(it->photonMultiplicity());
			nConstituents->push_back(it->numberOfDaughters());
			chargedEmEnergyFractionRAW->push_back(it->correctedJet("Uncorrected").chargedEmEnergyFraction());
			chargedHadronEnergyFractionRAW->push_back(it->correctedJet("Uncorrected").chargedHadronEnergyFraction());
			neutralEmEnergyFractionRAW->push_back(it->correctedJet("Uncorrected").neutralEmEnergyFraction());
			neutralHadronEnergyFractionRAW->push_back(it->correctedJet("Uncorrected").neutralHadronEnergyFraction());
			passLooseID->push_back(passjetLoose);
			passTightID->push_back(passjetTight);

			//b-tagging information
			//names are changing between major software releases
			trackCountingHighEffBTag->push_back(it->bDiscriminator("trackCountingHighEffBJetTags")); // checked 19.09.2011
			trackCountingHighPurBTag->push_back(it->bDiscriminator("trackCountingHighPurBJetTags")); // checked 19.09.2011
			simpleSecondaryVertexHighEffBTag->push_back(it->bDiscriminator("simpleSecondaryVertexHighEffBJetTags")); // checked 19.09.2011
			simpleSecondaryVertexHighPurBTag->push_back(it->bDiscriminator("simpleSecondaryVertexHighPurBJetTags")); // checked 19.09.2011
			jetProbabilityBTag->push_back(it->bDiscriminator("jetProbabilityBJetTags")); // checked 19.09.2011
			jetBProbabilityBTag->push_back(it->bDiscriminator("jetBProbabilityBJetTags")); // checked 19.09.2011

//			softElectronByIP3dBJetTags->push_back(it->bDiscriminator("softElectronByIP3dBJetTags")); // corrected 19.09.2011
//			softElectronByPtBJetTags->push_back(it->bDiscriminator("softElectronByPtBJetTags")); // introduced 19.09.2011

			softMuonBJetTag->push_back(it->bDiscriminator("softMuonBJetTags")); // checked 19.09.2011
			softMuonByIP3dBJetTags->push_back(it->bDiscriminator("softMuonByIP3dBJetTags")); // corrected 19.09.2011
			softMuonByPtBJetTags->push_back(it->bDiscriminator("softMuonByPtBJetTags")); // introduced 19.09.2011
			combinedSecondaryVertexBJetTags->push_back(it->bDiscriminator("combinedSecondaryVertexBJetTags")); // corrected 19.09.2011
			combinedSecondaryVertexMVABJetTag->push_back(it->bDiscriminator("combinedSecondaryVertexMVABJetTags")); // corrected 19.09.2011

			//jet-vertex association
			if (doVertexAssociation) {
				bestVertexTrackAssociationFactor->push_back(maxTrackAssocRatio);
				bestVertexTrackAssociationIndex->push_back(bestVtxIndexSharedTracks);
				closestVertexWeighted3DSeparation->push_back(minVtxDist3D);
				closestVertexWeightedXYSeparation->push_back(minVtxDistXY);
				closestVertexWeightedZSeparation->push_back(minVtxDistZ);
				closestVertex3DIndex->push_back(bestVtxIndex3Ddist);
				closestVertexXYIndex->push_back(bestVtxIndexXYdist);
				closestVertexZIndex->push_back(bestVtxIndexZdist);
			}
		}
	} else {
		edm::LogError("BristolNTuple_PFJetsError") << "Error! Can't get the product " << inputTag;
	}
	delete jecUnc;
	//-----------------------------------------------------------------
	// put vectors in the event
	//kinematic variables
	iEvent.put(px, prefix + "Px" + suffix);
	iEvent.put(py, prefix + "Py" + suffix);
	iEvent.put(pz, prefix + "Pz" + suffix);
	iEvent.put(energy, prefix + "Energy" + suffix);
	//kinematic variables before corrections
	iEvent.put(px_raw, prefix + "PxRAW" + suffix);
	iEvent.put(py_raw, prefix + "PyRAW" + suffix);
	iEvent.put(pz_raw, prefix + "PzRAW" + suffix);
	iEvent.put(energy_raw, prefix + "EnergyRAW" + suffix);
	//extra properties
	iEvent.put(charge, prefix + "Charge" + suffix);
	iEvent.put(mass, prefix + "Mass" + suffix);
	iEvent.put(partonFlavour, prefix + "PartonFlavour" + suffix);
	//generated jet properties
        if (!iEvent.isRealData()) {
		iEvent.put(genJet_energy, prefix + "GenJet.Energy" + suffix);
		iEvent.put(genJet_pt, prefix + "GenJet.Pt" + suffix);
		iEvent.put(genJet_px, prefix + "GenJet.Px" + suffix);
		iEvent.put(genJet_py, prefix + "GenJet.Py" + suffix);
		iEvent.put(genJet_pz, prefix + "GenJet.Pz" + suffix);
		iEvent.put(genJet_eta, prefix + "GenJet.Eta" + suffix);
		iEvent.put(genJet_phi, prefix + "GenJet.Phi" + suffix);
        }
	//jet energy correction and uncertainties
	iEvent.put(jecUnc_vec, prefix + "JECUnc" + suffix);
	iEvent.put(l2l3resJEC_vec, prefix + "L2L3ResJEC" + suffix);
	iEvent.put(l3absJEC_vec, prefix + "L3AbsJEC" + suffix);
	iEvent.put(l2relJEC_vec, prefix + "L2RelJEC" + suffix);
	iEvent.put(l1offJEC_vec, prefix + "L1OffJEC" + suffix);
	//jet ID variables
	iEvent.put(chargedEmEnergyFraction, prefix + "ChargedEmEnergyFraction" + suffix);
	iEvent.put(chargedHadronEnergyFraction, prefix + "ChargedHadronEnergyFraction" + suffix);
	iEvent.put(chargedMuEnergyFraction, prefix + "ChargedMuEnergyFraction" + suffix);
	iEvent.put(electronEnergyFraction, prefix + "ElectronEnergyFraction" + suffix);
	iEvent.put(muonEnergyFraction, prefix + "MuonEnergyFraction" + suffix);
	iEvent.put(neutralEmEnergyFraction, prefix + "NeutralEmEnergyFraction" + suffix);
	iEvent.put(neutralHadronEnergyFraction, prefix + "NeutralHadronEnergyFraction" + suffix);
	iEvent.put(photonEnergyFraction, prefix + "PhotonEnergyFraction" + suffix);
	iEvent.put(chargedHadronMultiplicity, prefix + "ChargedHadronMultiplicity" + suffix);
	iEvent.put(chargedMultiplicity, prefix + "ChargedMultiplicity" + suffix);
	iEvent.put(electronMultiplicity, prefix + "ElectronMultiplicity" + suffix);
	iEvent.put(muonMultiplicity, prefix + "MuonMultiplicity" + suffix);
	iEvent.put(neutralHadronMultiplicity, prefix + "NeutralHadronMultiplicity" + suffix);
	iEvent.put(neutralMultiplicity, prefix + "NeutralMultiplicity" + suffix);
	iEvent.put(photonMultiplicity, prefix + "PhotonMultiplicity" + suffix);
	iEvent.put(nConstituents, prefix + "NConstituents" + suffix);
	iEvent.put(chargedEmEnergyFractionRAW, prefix + "ChargedEmEnergyFractionRAW" + suffix);
	iEvent.put(chargedHadronEnergyFractionRAW, prefix + "ChargedHadronEnergyFractionRAW" + suffix);
	iEvent.put(neutralEmEnergyFractionRAW, prefix + "NeutralEmEnergyFractionRAW" + suffix);
	iEvent.put(neutralHadronEnergyFractionRAW, prefix + "NeutralHadronEnergyFractionRAW" + suffix);
	iEvent.put(passLooseID, prefix + "PassLooseID" + suffix);
	iEvent.put(passTightID, prefix + "PassTightID" + suffix);

	//b-tagging information
	iEvent.put(trackCountingHighEffBTag, prefix + "TrackCountingHighEffBTag" + suffix);
	iEvent.put(trackCountingHighPurBTag, prefix + "TrackCountingHighPurBTag" + suffix);
	iEvent.put(simpleSecondaryVertexHighEffBTag, prefix + "SimpleSecondaryVertexHighEffBTag" + suffix);
	iEvent.put(simpleSecondaryVertexHighPurBTag, prefix + "SimpleSecondaryVertexHighPurBTag" + suffix);
	iEvent.put(jetProbabilityBTag, prefix + "JetProbabilityBTag" + suffix);
	iEvent.put(jetBProbabilityBTag, prefix + "JetBProbabilityBTag" + suffix);
//	iEvent.put(softElectronByIP3dBJetTags, prefix + "SoftElectronByIP3dBJetTag" + suffix);
//	iEvent.put(softElectronByPtBJetTags, prefix + "SoftElectronByPtBJetTag" + suffix);
	iEvent.put(softMuonBJetTag, prefix + "SoftMuonBJetTag" + suffix);
	iEvent.put(softMuonByIP3dBJetTags, prefix + "SoftMuonByIP3dBJetTag" + suffix);
	iEvent.put(softMuonByPtBJetTags, prefix + "SoftMuonByPtBJetTag" + suffix);
	iEvent.put(combinedSecondaryVertexBJetTags, prefix + "CombinedSecondaryVertexBJetTag" + suffix);
	iEvent.put(combinedSecondaryVertexMVABJetTag, prefix + "CombinedSecondaryVertexMVABJetTag" + suffix);

	//jet-vertex association
	if (doVertexAssociation) {
		iEvent.put(bestVertexTrackAssociationFactor, prefix + "BestVertexTrackAssociationFactor" + suffix);
		iEvent.put(bestVertexTrackAssociationIndex, prefix + "BestVertexTrackAssociationIndex" + suffix);
		iEvent.put(closestVertexWeighted3DSeparation, prefix + "ClosestVertexWeighted3DSeparation" + suffix);
		iEvent.put(closestVertexWeightedXYSeparation, prefix + "ClosestVertexWeightedXYSeparation" + suffix);
		iEvent.put(closestVertexWeightedZSeparation, prefix + "ClosestVertexWeightedZSeparation" + suffix);
		iEvent.put(closestVertex3DIndex, prefix + "ClosestVertex3DIndex" + suffix);
		iEvent.put(closestVertexXYIndex, prefix + "ClosestVertexXYIndex" + suffix);
		iEvent.put(closestVertexZIndex, prefix + "ClosestVertexZIndex" + suffix);
	}

}
