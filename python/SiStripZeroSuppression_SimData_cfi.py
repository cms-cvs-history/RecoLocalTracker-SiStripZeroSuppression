import FWCore.ParameterSet.Config as cms

from RecoLocalTracker.SiStripZeroSuppression.DefaultAlgorithms_cff import *

siStripZeroSuppression = cms.EDProducer("SiStripZeroSuppression",
                                        
    Algorithms = DefaultAlgorithms,
    RawDigiProducersList = cms.VInputTag( cms.InputTag('simSiStripDigis','VirginRaw'), 
                                          cms.InputTag('simSiStripDigis','ProcessedRaw'),
                                          cms.InputTag('simSiStripDigis','ScopeMode')),
                                        
    storeCM = cms.bool(False), 
    fixCM= cms.bool(True),                # put -999 into CM collection for "inspected" APV
    produceRawDigis = cms.bool(False),    # if mergeCollection is True, produceRawDigi is not considered
    mergeCollections = cms.bool(False),
    doAPVRestore = cms.bool(False)
)



