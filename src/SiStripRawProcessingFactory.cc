#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripRawProcessingFactory.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripFedZeroSuppression.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripPedestalsSubtractor.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripMedianCommonModeNoiseSubtraction.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripFastLinearCommonModeNoiseSubtraction.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripTT6CommonModeNoiseSubtraction.h"

std::auto_ptr<SiStripPedestalsSubtractor> SiStripRawProcessingFactory::
create_SubtractorPed(const edm::ParameterSet& conf) {
  return std::auto_ptr<SiStripPedestalsSubtractor>( new SiStripPedestalsSubtractor() );
}

std::auto_ptr<SiStripFedZeroSuppression> SiStripRawProcessingFactory::
create_Suppressor(const edm::ParameterSet& conf) {
  uint32_t mode = conf.getParameter<uint32_t>("SiStripFedZeroSuppressionMode");
  switch(mode) {
  case 1: case 2: case 3:  case 4:
    return std::auto_ptr<SiStripFedZeroSuppression>( new SiStripFedZeroSuppression(mode));
  default:
    throw cms::Exception("Unregistered mode") << "SiStripFedZeroSuppression has modes 1,2,3,4.";
  }
}

std::auto_ptr<SiStripCommonModeNoiseSubtractor> SiStripRawProcessingFactory::
create_SubtractorCMN(const edm::ParameterSet& conf) {
  std::string mode = conf.getParameter<std::string>("CommonModeNoiseSubtractionMode");

  if ( mode == "Median")
    return std::auto_ptr<SiStripCommonModeNoiseSubtractor>( new SiStripMedianCommonModeNoiseSubtraction() );

  if ( mode == "FastLinear")
    return std::auto_ptr<SiStripCommonModeNoiseSubtractor>( new SiStripFastLinearCommonModeNoiseSubtraction() );

  if ( mode == "TT6") {
    double cutToAvoidSignal = conf.getParameter<double>("CutToAvoidSignal");
    return std::auto_ptr<SiStripCommonModeNoiseSubtractor>( new SiStripTT6CommonModeNoiseSubtraction(cutToAvoidSignal) );
  }
  
  throw cms::Exception("Unregistered Algorithm") 
    << "SiStripCommonModeNoiseSubtractor possibilities: (Median, FastLinear, TT6)";
}
