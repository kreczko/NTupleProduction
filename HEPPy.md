# heppy - A lightweight python event processing framework for high-energy physics

## Material
http://indico.cern.ch/event/346174/contribution/1/attachments/682467/937528/heppy_FCC.pdf
HEPPY for FFC: https://github.com/HEP-FCC/heppy_fcc
HEPPY fork:  https://github.com/HEP-FCC/heppy
HEPPY: https://github.com/cbernet/heppy
Apparently https://twiki.cern.ch/twiki/bin/viewauth/FCC/FCCSoftwareHeppy, but does not exist
CMSSW https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHeppy

## Create HEPPy NTuples
```
python BristolAnalysis/NTupleTools/Configuration/create_heppy_ntuple.py
```

## Adding event content
In Python FWLite one can simply add event content by using `setattr(event, ...)`
as shown in https://github.com/cbernet/cmssw/blob/heppy_7_4_12/PhysicsTools/Heppy/python/analyzers/objects/JetAnalyzer.py#L272