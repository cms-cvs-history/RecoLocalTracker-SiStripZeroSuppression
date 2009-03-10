#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripZeroSuppressionAlgorithm.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"

#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripPedestalsSubtractor.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripFedZeroSuppression.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripMedianCommonModeNoiseSubtraction.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripFastLinearCommonModeNoiseSubtraction.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripTT6CommonModeNoiseSubtraction.h"

SiStripZeroSuppressionAlgorithm::
SiStripZeroSuppressionAlgorithm(const edm::ParameterSet& conf) 
{
  std::string ZeroSuppressionMode_(conf.getParameter<std::string>("ZeroSuppressionMode"));
  std::string CMNSubtractionMode_(conf.getParameter<std::string>("CommonModeNoiseSubtractionMode"));

  SiStripPedestalsSubtractor_ = new SiStripPedestalsSubtractor();

   //------------------------
  if ( ZeroSuppressionMode_ == "SiStripFedZeroSuppression" ) {
    SiStripZeroSuppressor_ = new SiStripFedZeroSuppression(conf.getParameter<uint32_t>("FEDalgorithm")); 
  } 
  else {
    edm::LogError("Unregistered Algorithm") << "No valid strip ZeroSuppressor selected, possible ZeroSuppressor: SiStripFedZeroSuppression" << std::endl;
    throw cms::Exception("Wrong Parameter Configuraiton")
      << "[SiStripZeroSuppressionAlgorithm::run] invalid ZeroSuppression algorithm " << ZeroSuppressionMode_ << "\n Please set in the SiStripZeroSuppression configuration file the string ZeroSuppressionMode = SiStripFedZeroSuppression";
  }
  
   //------------------------
  if ( CMNSubtractionMode_ == "Median") { 
    SiStripCommonModeNoiseSubtractor_ = new SiStripMedianCommonModeNoiseSubtraction();
  }
  else if ( CMNSubtractionMode_ == "TT6") { 
    SiStripCommonModeNoiseSubtractor_ = new SiStripTT6CommonModeNoiseSubtraction(conf.getParameter<double>("CutToAvoidSignal"));
  }
  else if ( CMNSubtractionMode_ == "FastLinear") { 
    SiStripCommonModeNoiseSubtractor_ = new SiStripFastLinearCommonModeNoiseSubtraction();
  }
  else {
    edm::LogError("Unregistered Algorithm") << "Possible CommonModeNoiseSubtraction modes: (Median, FastLinear, TT6)" << std::endl;
    throw cms::Exception("Wrong Parameter Configuraiton")
      << "Invalid CMNSubtraction algorithm " << CMNSubtractionMode_ 
      << "\n Please set in the SiStripZeroSuppression configuration file the string CommonModeNoiseSubtractionMode to a supported mode: Median, TT6, FastLinearone";
  }
}

SiStripZeroSuppressionAlgorithm::
~SiStripZeroSuppressionAlgorithm() {
  if ( SiStripZeroSuppressor_ != 0 ) 
    delete SiStripZeroSuppressor_;
  if ( SiStripCommonModeNoiseSubtractor_ != 0 ) 
    delete SiStripCommonModeNoiseSubtractor_;
  if ( SiStripPedestalsSubtractor_ != 0 ) 
    delete SiStripPedestalsSubtractor_;
}

void SiStripZeroSuppressionAlgorithm::
run(std::string RawDigiType, 
    const edm::DetSetVector<SiStripRawDigi>& input,
    std::vector< edm::DetSet<SiStripDigi> >& output,
    const edm::EventSetup& es)
{
  SiStripPedestalsSubtractor_->init(es);
  SiStripCommonModeNoiseSubtractor_->init(es);
  SiStripZeroSuppressor_->init(es);
  
  for ( edm::DetSetVector<SiStripRawDigi>::const_iterator 
	  DSViter = input.begin(); DSViter != input.end(); DSViter++) {
    
    edm::DetSet<SiStripDigi> ssd(DSViter->id);
    
    if ( RawDigiType == "VirginRaw" ) {
      std::vector<int16_t> vssRd(DSViter->data.size());
      SiStripPedestalsSubtractor_->subtract(*DSViter,vssRd);
      SiStripCommonModeNoiseSubtractor_->subtract(DSViter->id,vssRd);
      SiStripZeroSuppressor_->suppress(vssRd,ssd);
    } 
    else if ( RawDigiType == "ProcessedRaw" ){
      SiStripZeroSuppressor_->suppress((*DSViter),ssd);	
    }
    else throw cms::Exception("Unknown RawDigitype") << "SiStripZeroZuppression can only process types \"VirginRaw\" and \"ProcessedRaw\"";
    
    if (ssd.data.size())  output.push_back(ssd); 
  }
}
