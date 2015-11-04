from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer import AutoFillTreeProducer


class EventProducer(AutoFillTreeProducer):

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(EventProducer, self).__init__(cfg_ana, cfg_comp, looperName)
