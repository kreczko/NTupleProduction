#ifndef BristolNTuplePFJetsExtra
#define BristolNTuplePFJetsExtra

#include <string>
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

class BristolNTuple_PFJets : public edm::EDProducer {

 public:
  explicit BristolNTuple_PFJets(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::EDGetTokenT<std::vector<pat::Jet>> inputTag;
  const std::string     prefix,suffix;
  const unsigned int    maxSize;
  const double      minJetPtToStore;
  const bool readJEC;
  const bool doVertexAssociation;
  const bool isRealData;
};

#endif
