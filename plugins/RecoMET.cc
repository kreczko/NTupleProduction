#include "BristolAnalysis/NTupleTools/interface/BristolNTuple_RecoMET.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

BristolNTuple_RecoMET::BristolNTuple_RecoMET(const edm::ParameterSet& iConfig) :
	inputTag(iConfig.getParameter < edm::InputTag > ("InputTag")), //
	prefix(iConfig.getParameter < std::string > ("Prefix")), //
	suffix(iConfig.getParameter < std::string > ("Suffix"))
{
	produces<float>(prefix + "Ex" + suffix);
	produces<float>(prefix + "Ey" + suffix);
	produces<float>(prefix + "ET" + suffix);
	produces<float>(prefix + "Phi" + suffix);
	produces<float>(prefix + "Significance" + suffix);
}

void BristolNTuple_RecoMET::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    edm::Handle < std::vector<reco::PFMET> > recomets;
    iEvent.getByLabel(inputTag, recomets);

    if (!recomets.isValid())
      edm::LogError("BristolNTuple_RecoMETExtraError") << "Error! Can't get the product " << inputTag;

    const reco::PFMET recoPFMET(recomets->at(0));

    std::auto_ptr<float> px(new float(recoPFMET.px()));
    std::auto_ptr<float> py(new float(recoPFMET.py()));
    std::auto_ptr<float> met(new float(recoPFMET.pt()));
    std::auto_ptr<float> phi(new float(recoPFMET.phi()));
    std::auto_ptr<float> significance(new float(recoPFMET.significance()));

	iEvent.put(px, prefix + "Ex" + suffix);
	iEvent.put(py, prefix + "Ey" + suffix);
	iEvent.put(met, prefix + "ET" + suffix);
	iEvent.put(phi, prefix + "Phi" + suffix);
	iEvent.put(significance, prefix + "Significance" + suffix);
}
