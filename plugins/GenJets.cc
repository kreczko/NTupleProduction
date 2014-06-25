#include "BristolAnalysis/NTupleTools/interface/BristolNTuple_GenJets.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"


BristolNTuple_GenJets::BristolNTuple_GenJets(const edm::ParameterSet& iConfig) :
    inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
    prefix  (iConfig.getParameter<std::string>  ("Prefix")),
    suffix  (iConfig.getParameter<std::string>  ("Suffix")),
    minPt (iConfig.getParameter<float> ("minPt")),
    maxAbsoluteEta (iConfig.getParameter<float> ("maxAbsoluteEta")),
    maxSize (iConfig.getParameter<unsigned int> ("MaxSize"))
{
	produces < std::vector<float> > (prefix + "Px" + suffix);
	produces < std::vector<float> > (prefix + "Py" + suffix);
	produces < std::vector<float> > (prefix + "Pz" + suffix);
	produces < std::vector<float> > (prefix + "Energy" + suffix);
	produces < std::vector<float> > (prefix + "Charge" + suffix);
	produces < std::vector<float> > (prefix + "Mass" + suffix);

	produces < std::vector<float> > (prefix + "EMF" + suffix);
	produces < std::vector<float> > (prefix + "HADF" + suffix);
}

void BristolNTuple_GenJets::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

	std::auto_ptr < std::vector<float> > px(new std::vector<float>());
	std::auto_ptr < std::vector<float> > py(new std::vector<float>());
	std::auto_ptr < std::vector<float> > pz(new std::vector<float>());
	std::auto_ptr < std::vector<float> > energy(new std::vector<float>());
	std::auto_ptr < std::vector<float> > charge(new std::vector<float>());
	std::auto_ptr < std::vector<float> > mass(new std::vector<float>());

	std::auto_ptr < std::vector<float> > emf(new std::vector<float>());
	std::auto_ptr < std::vector<float> > hadf(new std::vector<float>());

	//-----------------------------------------------------------------
	if (!iEvent.isRealData()) {
		edm::Handle < reco::GenJetCollection > genJets;
		iEvent.getByLabel(inputTag, genJets);

		if (genJets.isValid()) {
			edm::LogInfo("BristolNTuple_GenJetsExtraInfo") << "Total # GenJets: " << genJets->size();

			for (reco::GenJetCollection::const_iterator it = genJets->begin(); it != genJets->end(); ++it) {
				// exit from loop when you reach the required number of GenJets
				if (px->size() >= maxSize)
					break;

				if (it->pt() < minPt || fabs(it->eta()) > maxAbsoluteEta)
					continue;

				// fill in all the vectors
				px->push_back(it->px());
				py->push_back(it->py());
				pz->push_back(it->pz());
				energy->push_back(it->energy());
				charge->push_back(it->charge());
				mass->push_back(it->mass());

				emf->push_back(it->emEnergy() / it->energy());
				hadf->push_back(it->hadEnergy() / it->energy());
			}
		} else {
			edm::LogError("BristolNTuple_GenJetsExtraError") << "Error! Can't get the product " << inputTag;
		}
	}

	//-----------------------------------------------------------------
	// put vectors in the event
	iEvent.put(px, prefix + "Px" + suffix);
	iEvent.put(py, prefix + "Py" + suffix);
	iEvent.put(pz, prefix + "Pz" + suffix);
	iEvent.put(energy, prefix + "Energy" + suffix);
	iEvent.put(charge, prefix + "Charge" + suffix);
	iEvent.put(mass, prefix + "Mass" + suffix);

	iEvent.put(emf, prefix + "EMF" + suffix);
	iEvent.put(hadf, prefix + "HADF" + suffix);
}
