#include "BristolAnalysis/NTupleTools/interface/BristolNTuple_TriggerObjects.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "BristolAnalysis/NTupleTools/interface/PatUtilities.h"


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"


using namespace std;

BristolNTuple_TriggerObjects::BristolNTuple_TriggerObjects(const edm::ParameterSet& iConfig) :
		hltObjectsInputTag(iConfig.getParameter < edm::InputTag > ("HLTObjectsInputTag")), //
		hltObjectOfInterest(iConfig.getParameter < edm::InputTag > ("HLTObjectOfInterest")), //
		hltConfig(), //
		prefix(iConfig.getParameter < std::string > ("Prefix")), //
		suffix(iConfig.getParameter < std::string > ("Suffix")) {

	produces <std::vector<float> > ( prefix + "Px" + suffix );
	produces <std::vector<float> > ( prefix + "Py" + suffix );
	produces <std::vector<float> > ( prefix + "Pz" + suffix );
        produces <std::vector<float> > ( prefix + "Energy" + suffix );
}

void BristolNTuple_TriggerObjects::beginRun(edm::Run& iRun, const edm::EventSetup& iSetup) {

	bool changed = true;
	if (hltConfig.init(iRun, iSetup, hltObjectsInputTag.process(), changed)) {
		// if init returns TRUE, initialisation has succeeded!
		edm::LogInfo("BristolNTuple_TriggerInfo") << "HLT config with process name " << hltObjectsInputTag.process()
				<< " successfully extracted";
	} else {
		// if init returns FALSE, initialisation has NOT succeeded, which indicates a problem
		// with the file and/or code and needs to be investigated!
		edm::LogError("BristolNTuple_TriggerError") << "Error! HLT config extraction with process name "
				<< hltObjectsInputTag.process() << " failed";
		// In this case, all access methods will return empty values!
	}
}

void BristolNTuple_TriggerObjects::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

	std::auto_ptr < std::vector<float> > px(new std::vector<float>());
	std::auto_ptr < std::vector<float> > py(new std::vector<float>());
	std::auto_ptr < std::vector<float> > pz(new std::vector<float>());
	std::auto_ptr < std::vector<float> > E(new std::vector<float>());

	//-----------------------------------------------------------------
	// open the trigger summary to retrieve trigger objects of interest
	edm::Handle<trigger::TriggerEvent> triggerObjectsSummary;
	iEvent.getByLabel(hltObjectsInputTag, triggerObjectsSummary);
	trigger::TriggerObjectCollection selectedObjects;

	if (triggerObjectsSummary.isValid()) {
		edm::LogInfo("BristolNTuple_TriggerInfo") << "Successfully obtained " << hltObjectsInputTag;
		// trigger object we want to match
		size_t filterIndex = (*triggerObjectsSummary).filterIndex(hltObjectOfInterest);
		trigger::TriggerObjectCollection allTriggerObjects = triggerObjectsSummary->getObjects();
		if (filterIndex < (*triggerObjectsSummary).sizeFilters()) { //check if the trigger object is present
			const trigger::Keys &keys = (*triggerObjectsSummary).filterKeys(filterIndex);
			for (size_t j = 0; j < keys.size(); j++) {
				trigger::TriggerObject foundObject = (allTriggerObjects)[keys[j]];
				selectedObjects.push_back(foundObject);
			}
		}
	} else {
		edm::LogError("BristolNTuple_TriggerError") << "Error! Can't get the product " << hltObjectsInputTag;
	}

	for (size_t t = 0; t < selectedObjects.size() ; t++) {
		px->push_back(selectedObjects[t].px());
		py->push_back(selectedObjects[t].py());
		pz->push_back(selectedObjects[t].pz());
		E->push_back(selectedObjects[t].energy());
	}

	//-----------------------------------------------------------------
	// put vectors in the event
	iEvent.put(px, prefix + "Px" + suffix);
	iEvent.put(py, prefix + "Py" + suffix);
	iEvent.put(pz, prefix + "Pz" + suffix);
        iEvent.put(E, prefix + "Energy" + suffix);
}
