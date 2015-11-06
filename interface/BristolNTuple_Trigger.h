#ifndef BristolNTupleTrigger
#define BristolNTupleTrigger
#include <string>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
class BristolNTuple_Trigger : public edm::EDProducer {
 public:
  explicit BristolNTuple_Trigger(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  void beginRun( edm::Run &, const edm::EventSetup & );
  //unsigned int findTrigger(const std::string& triggerWildCard);

  const edm::InputTag   hltInputTag_, hltObjectsInputTag_;
  const std::string pathOfInterest_;

  const bool tightenTrigger_;
  std::string triggerFilterName_;
  unsigned minNumber_;
  StringCutObjectSelector<reco::Candidate> triggerObjectSelector_;

  const std::string     prefix_,suffix_;
};

#endif
