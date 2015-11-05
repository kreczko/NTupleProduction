from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer


class ElectronProducer(Analyzer):
    """
        Mostly taken from https://github.com/cbernet/cmssw/blob/heppy_7_4_12/PhysicsTools/Heppy/python/analyzers/objects/LeptonAnalyzer.py
        The electron producer puts additional variables for the electron
        collections into the event
    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ElectronProducer, self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(ElectronProducer, self).declareHandles()
        self.handles['electrons'] = AutoHandle(
            self.cfg_ana.electrons, "std::vector<pat::Electron>")
        self.handles['rhoEle'] = AutoHandle(self.cfg_ana.rhoElectron, 'double')

    def beginLoop(self, setup):
        super(ElectronProducer, self).beginLoop(setup)
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')

    def makeAllElectrons(self, event):
        """
               make a list of all electrons, and apply basic corrections to them
        """
        allelectrons = map(Electron, self.handles['electrons'].product())

        # Duplicate removal for fast sim (to be checked if still necessary in
        # latest greatest 5.3.X releases)
        allelenodup = []
        for e in allelectrons:
            dup = False
            for e2 in allelenodup:
                if abs(e.pt() - e2.pt()) < 1e-6 and abs(e.eta() - e2.eta()) < 1e-6 and abs(e.phi() - e2.phi()) < 1e-6 and e.charge() == e2.charge():
                    dup = True
                    break
            if not dup:
                allelenodup.append(e)
        allelectrons = allelenodup

        # fill EA for rho-corrected isolation
        for ele in allelectrons:
            ele.rho = float(self.handles['rhoEle'].product()[0])
            if self.eleEffectiveArea == "Data2012":
                # https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaEARhoCorrection?rev=14
                SCEta = abs(ele.superCluster().eta())
                if SCEta < 1.0:
                    ele.EffectiveArea03 = 0.13  # 0.130;
                elif SCEta < 1.479:
                    ele.EffectiveArea03 = 0.14  # 0.137;
                elif SCEta < 2.0:
                    ele.EffectiveArea03 = 0.07  # 0.067;
                elif SCEta < 2.2:
                    ele.EffectiveArea03 = 0.09  # 0.089;
                elif SCEta < 2.3:
                    ele.EffectiveArea03 = 0.11  # 0.107;
                elif SCEta < 2.4:
                    ele.EffectiveArea03 = 0.11  # 0.110;
                else:
                    ele.EffectiveArea03 = 0.14  # 0.138;
                if SCEta < 1.0:
                    ele.EffectiveArea04 = 0.208
                elif SCEta < 1.479:
                    ele.EffectiveArea04 = 0.209
                elif SCEta < 2.0:
                    ele.EffectiveArea04 = 0.115
                elif SCEta < 2.2:
                    ele.EffectiveArea04 = 0.143
                elif SCEta < 2.3:
                    ele.EffectiveArea04 = 0.183
                elif SCEta < 2.4:
                    ele.EffectiveArea04 = 0.194
                else:
                    ele.EffectiveArea04 = 0.261
            elif self.eleEffectiveArea == "Phys14_25ns_v1":
                aeta = abs(ele.eta())
                if aeta < 0.800:
                    ele.EffectiveArea03 = 0.1013
                elif aeta < 1.300:
                    ele.EffectiveArea03 = 0.0988
                elif aeta < 2.000:
                    ele.EffectiveArea03 = 0.0572
                elif aeta < 2.200:
                    ele.EffectiveArea03 = 0.0842
                else:
                    ele.EffectiveArea03 = 0.1530
                if aeta < 0.800:
                    ele.EffectiveArea04 = 0.1830
                elif aeta < 1.300:
                    ele.EffectiveArea04 = 0.1734
                elif aeta < 2.000:
                    ele.EffectiveArea04 = 0.1077
                elif aeta < 2.200:
                    ele.EffectiveArea04 = 0.1565
                else:
                    ele.EffectiveArea04 = 0.2680
            elif self.eleEffectiveArea == "Spring15_50ns_v1":
                aeta = abs(ele.eta())
                # ----- https://github.com/ikrav/cmssw/blob/egm_id_747_v2/RecoEgamma/ElectronIdentification/data/Spring15/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_50ns.txt
                if aeta < 0.800:
                    ele.EffectiveArea03 = 0.0973
                elif aeta < 1.300:
                    ele.EffectiveArea03 = 0.0954
                elif aeta < 2.000:
                    ele.EffectiveArea03 = 0.0632
                elif aeta < 2.200:
                    ele.EffectiveArea03 = 0.0727
                else:
                    ele.EffectiveArea03 = 0.1337
                # warning: EAs not computed for cone DR=0.4 yet. Do not correct
                ele.EffectiveArea04 = 0.0
            elif self.eleEffectiveArea == "Spring15_25ns_v1":
                aeta = abs(ele.eta())
                # ----- https://github.com/ikrav/cmssw/blob/egm_id_747_v2/RecoEgamma/ElectronIdentification/data/Spring15/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_25ns.txt
                if aeta < 1.000:
                    ele.EffectiveArea03 = 0.1752
                elif aeta < 1.479:
                    ele.EffectiveArea03 = 0.1862
                elif aeta < 2.000:
                    ele.EffectiveArea03 = 0.1411
                elif aeta < 2.200:
                    ele.EffectiveArea03 = 0.1534
                elif aeta < 2.300:
                    ele.EffectiveArea03 = 0.1903
                elif aeta < 2.400:
                    ele.EffectiveArea03 = 0.2243
                else:
                    ele.EffectiveArea03 = 0.2687
                # warning: EAs not computed for cone DR=0.4 yet. Do not correct
                ele.EffectiveArea04 = 0.0
            else:
                raise RuntimeError,  "Unsupported value for ele_effectiveAreas: can only use Data2012 (rho: ?) and Phys14_v1 (rho: fixedGridRhoFastjetAll)"

        # Electron scale calibrations
        if self.cfg_ana.doElectronScaleCorrections:
            for ele in allelectrons:
                self.electronEnergyCalibrator.correct(ele, event.run)

        # Attach the vertex
        for ele in allelectrons:
            ele.associatedVertex = event.goodVertices[0] if len(
                event.goodVertices) > 0 else event.vertices[0]

        # Compute relIso with R=0.3 and R=0.4 cones
        for ele in allelectrons:
            if self.cfg_ana.ele_isoCorr == "rhoArea":
                ele.absIso03 = (ele.chargedHadronIsoR(
                    0.3) + max(ele.neutralHadronIsoR(0.3) + ele.photonIsoR(0.3) - ele.rho * ele.EffectiveArea03, 0))
                ele.absIso04 = (ele.chargedHadronIsoR(
                    0.4) + max(ele.neutralHadronIsoR(0.4) + ele.photonIsoR(0.4) - ele.rho * ele.EffectiveArea04, 0))
            elif self.cfg_ana.ele_isoCorr == "deltaBeta":
                ele.absIso03 = (ele.chargedHadronIsoR(
                    0.3) + max(ele.neutralHadronIsoR(0.3) + ele.photonIsoR(0.3) - ele.puChargedHadronIsoR(0.3) / 2, 0.0))
                ele.absIso04 = (ele.chargedHadronIsoR(
                    0.4) + max(ele.neutralHadronIsoR(0.4) + ele.photonIsoR(0.4) - ele.puChargedHadronIsoR(0.4) / 2, 0.0))
            else:
                raise RuntimeError, "Unsupported ele_isoCorr name '" + \
                    str(self.cfg_ana.ele_isoCorr) + \
                    "'! For now only 'rhoArea' and 'deltaBeta' are supported."
            ele.relIso03 = ele.absIso03 / ele.pt()
            ele.relIso04 = ele.absIso04 / ele.pt()

        # Set tight MVA id
        for ele in allelectrons:
            if self.cfg_ana.ele_tightId == "MVA":
                ele.tightIdResult = ele.electronID("POG_MVA_ID_Trig_full5x5")
            elif self.cfg_ana.ele_tightId == "Cuts_2012":
                ele.tightIdResult = -1 + 1 * ele.electronID("POG_Cuts_ID_2012_Veto_full5x5") + 1 * ele.electronID(
                    "POG_Cuts_ID_2012_Loose_full5x5") + 1 * ele.electronID("POG_Cuts_ID_2012_Medium_full5x5") + 1 * ele.electronID("POG_Cuts_ID_2012_Tight_full5x5")
            elif self.cfg_ana.ele_tightId == "Cuts_PHYS14_25ns_v1_ConvVetoDxyDz":
                ele.tightIdResult = -1 + 1 * ele.electronID("POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Veto_full5x5") + 1 * ele.electronID("POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Loose_full5x5") + 1 * ele.electronID(
                    "POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Medium_full5x5") + 1 * ele.electronID("POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Tight_full5x5")

            else:
                try:
                    ele.tightIdResult = ele.electronID(
                        self.cfg_ana.ele_tightId)
                except RuntimeError:
                    raise RuntimeError, "Unsupported ele_tightId name '" + \
                        str(self.cfg_ana.ele_tightId) + \
                        "'! For now only 'MVA' and 'Cuts_2012' are supported, in addition to what provided in Electron.py."

        return allelectrons

    def isFromB(self, particle, bid=5, done={}):
        for i in xrange(particle.numberOfMothers()):
            mom = particle.mother(i)
            momid = abs(mom.pdgId())
            if momid / 1000 == bid or momid / 100 == bid or momid == bid:
                return True
            elif mom.status() == 2 and self.isFromB(mom, done=done):
                return True
        return False

    def process(self, event):
        self.readCollections(event.input)
        self.counters.counter('events').inc('all events')

        # call the leptons functions
        self.makeAllElectrons(event)

        return True
    
#A default config
setattr(ElectronProducer,"defaultConfig",cfg.Analyzer(
    verbose=False,
    class_object=ElectronProducer,
    # input collections
    electrons='slimmedElectrons',
    rhoElectron = 'fixedGridRhoFastjetAll',
    # energy scale corrections (off by default)
    doElectronScaleCorrections=False, # "embedded" in 5.18 for regression
    doSegmentBasedMuonCleaning=False,
    # inclusive very loose electron selection
    inclusive_electron_id  = "",
    inclusive_electron_pt  = 5,
    inclusive_electron_eta = 2.5,
    inclusive_electron_dxy = 0.5,
    inclusive_electron_dz  = 1.0,
    inclusive_electron_lostHits = 1.0,
    # loose electron selection
    loose_electron_id     = "", #POG_MVA_ID_NonTrig_full5x5",
    loose_electron_pt     = 7,
    loose_electron_eta    = 2.4,
    loose_electron_dxy    = 0.05,
    loose_electron_dz     = 0.2,
    loose_electron_relIso = 0.4,
    # loose_electron_isoCut = lambda electron : electron.miniRelIso < 0.1
    loose_electron_lostHits = 1.0,
    # electron isolation correction method (can be "rhoArea" or "deltaBeta")
    ele_isoCorr = "rhoArea" ,
    ele_effectiveAreas = "Spring15_25ns_v1" , #(can be 'Data2012' or 'Phys14_25ns_v1', or 'Spring15_50ns_v1' or 'Spring15_25ns_v1')
    ele_tightId = "Cuts_2012" ,
    # Mini-isolation, with pT dependent cone: will fill in the miniRelIso, miniRelIsoCharged, miniRelIsoNeutral variables of the leptons (see https://indico.cern.ch/event/368826/ )
    packedCandidates = 'packedPFCandidates',
    miniIsolationPUCorr = 'rhoArea', # Allowed options: 'rhoArea' (EAs for 03 cone scaled by R^2), 'deltaBeta', 'raw' (uncorrected), 'weights' (delta beta weights; not validated)
                                     # Choose None to just use the individual object's PU correction
    miniIsolationVetoLeptons = None, # use 'inclusive' to veto inclusive leptons and their footprint in all isolation cones
    # do MC matching 
    do_mc_match = True, # note: it will in any case try it only on MC, not on data
    match_inclusiveLeptons = False, # match to all inclusive leptons
    )
)
