#include "BristolAnalysis/NTupleTools/interface/BristolNTuple_Photons.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


BristolNTuple_Photons::BristolNTuple_Photons(const edm::ParameterSet& iConfig) :
    inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
    prefix  (iConfig.getParameter<std::string>  ("Prefix")),
    suffix  (iConfig.getParameter<std::string>  ("Suffix")),
    maxSize (iConfig.getParameter<unsigned int> ("MaxSize"))
{
    produces<std::vector<float> > (prefix + "Px" + suffix);
    produces<std::vector<float> > (prefix + "Py" + suffix);
    produces<std::vector<float> > (prefix + "Pz" + suffix);
    produces<std::vector<float> > (prefix + "Energy" + suffix);
    produces<std::vector<float> > (prefix + "EcalIso" + suffix);
    produces<std::vector<float> > (prefix + "HcalIso" + suffix);
    produces<std::vector<float> > (prefix + "HoE" + suffix);
    produces<std::vector<float> > (prefix + "TrkIso" + suffix);
    produces<std::vector<float> > (prefix + "SigmaIEtaIEta" + suffix);
    produces<std::vector<bool> > (prefix + "TrkVeto" + suffix);
    produces<std::vector<float> > (prefix + "SCseedEnergy" + suffix);
    produces<std::vector<float> > (prefix + "SCenergy" + suffix);
    produces<std::vector<float> > (prefix + "SCeta" + suffix);
    produces<std::vector<float> > (prefix + "SCphi" + suffix);
    produces<std::vector<float> > (prefix + "E3x3" + suffix);
    produces<std::vector<float> > (prefix + "E5x5" + suffix);
}

void BristolNTuple_Photons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr < std::vector<float> > px(new std::vector<float>());
    std::auto_ptr < std::vector<float> > py(new std::vector<float>());
    std::auto_ptr < std::vector<float> > pz(new std::vector<float>());
    std::auto_ptr < std::vector<float> > energy(new std::vector<float>());
    std::auto_ptr < std::vector<float> > ecalIso(new std::vector<float>());
    std::auto_ptr < std::vector<float> > hcalIso(new std::vector<float>());
    std::auto_ptr < std::vector<float> > hoe(new std::vector<float>());
    std::auto_ptr < std::vector<float> > trkIso(new std::vector<float>());
    std::auto_ptr < std::vector<float> > sigmaIetaIeta(new std::vector<float>());
    std::auto_ptr < std::vector<bool> > trkVeto(new std::vector<bool>());
    std::auto_ptr < std::vector<float> > SCseedEnergy(new std::vector<float>());
    std::auto_ptr < std::vector<float> > SCenergy(new std::vector<float>());
    std::auto_ptr < std::vector<float> > SCeta(new std::vector<float>());
    std::auto_ptr < std::vector<float> > SCphi(new std::vector<float>());
    std::auto_ptr < std::vector<float> > E3x3(new std::vector<float>());
    std::auto_ptr < std::vector<float> > E5x5(new std::vector<float>());

    //-----------------------------------------------------------------

    edm::Handle < std::vector<pat::Photon> > photons;
    iEvent.getByLabel(inputTag, photons);

    if (photons.isValid()) {
        edm::LogInfo("BristolNTuple_PhotonsInfo") << "Total # Photons: " << photons->size();

        for (std::vector<pat::Photon>::const_iterator it = photons->begin(); it != photons->end(); ++it) {
            // exit from loop when you reach the required number of photons
            if (px->size() >= maxSize)
                break;

            px->push_back(it->px());
            py->push_back(it->py());
            pz->push_back(it->pz());
            energy->push_back(it->energy());
            ecalIso->push_back(it->ecalIso());
            hcalIso->push_back(it->hcalIso());
            hoe->push_back(it->hadronicOverEm());
            trkIso->push_back(it->trkSumPtHollowConeDR04());
            sigmaIetaIeta->push_back(it->sigmaIetaIeta());
            trkVeto->push_back(it->hasPixelSeed());
            SCseedEnergy->push_back(it->superCluster()->seed()->energy());
            SCenergy->push_back(it->superCluster()->energy());
            SCeta->push_back(it->superCluster()->eta());
            SCphi->push_back(it->superCluster()->phi());
            E3x3->push_back(it->e3x3());
            E5x5->push_back(it->e5x5());

        }
    } else {
        edm::LogError("BristolNTuple_PhotonsError") << "Error! Can't get the product " << inputTag;
    }

    //-----------------------------------------------------------------
    // put vectors in the event
    iEvent.put(px, prefix + "Px" + suffix);
    iEvent.put(py, prefix + "Py" + suffix);
    iEvent.put(pz, prefix + "Pz" + suffix);
    iEvent.put(energy, prefix + "Energy" + suffix);
    iEvent.put(ecalIso, prefix + "EcalIso" + suffix);
    iEvent.put(hcalIso, prefix + "HcalIso" + suffix);
    iEvent.put(hoe, prefix + "HoE" + suffix);
    iEvent.put(trkIso, prefix + "TrkIso" + suffix);
    iEvent.put(sigmaIetaIeta, prefix + "SigmaIEtaIEta" + suffix);
    iEvent.put(trkVeto, prefix + "TrkVeto" + suffix);
    iEvent.put(SCseedEnergy, prefix + "SCseedEnergy" + suffix);
    iEvent.put(SCenergy, prefix + "SCenergy" + suffix);
    iEvent.put(SCeta, prefix + "SCeta" + suffix);
    iEvent.put(SCphi, prefix + "SCphi" + suffix);
    iEvent.put(E3x3, prefix + "E3x3" + suffix);
    iEvent.put(E5x5, prefix + "E5x5" + suffix);
}
