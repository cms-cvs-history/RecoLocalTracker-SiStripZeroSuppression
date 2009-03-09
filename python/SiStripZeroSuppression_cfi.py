import FWCore.ParameterSet.Config as cms

siStripZeroSuppression = cms.EDFilter("SiStripZeroSuppression",
                                      RawDigiProducersList = cms.VInputTag(
    cms.InputTag('siStripDigis','VirginRaw'), 
    cms.InputTag('siStripDigis','ProcessedRaw'),
    cms.InputTag('siStripDigis','ScopeMode')),
                                      FEDalgorithm = cms.uint32(4),
                                      ZeroSuppressionMode = cms.string('SiStripFedZeroSuppression'),
                                      CutToAvoidSignal = cms.double(3.0), ##
                                      CommonModeNoiseSubtractionMode = cms.string('Median') ##Supported modes: Median, TT6, FastLinear
)
