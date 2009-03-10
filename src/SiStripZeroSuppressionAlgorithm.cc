#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripZeroSuppressionAlgorithm.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"

#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripRawProcessingFactory.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripFedZeroSuppression.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripPedestalsSubtractor.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripCommonModeNoiseSubtractor.h"

SiStripZeroSuppressionAlgorithm::
SiStripZeroSuppressionAlgorithm(const edm::ParameterSet& conf) 
  : suppressor(SiStripRawProcessingFactory::create_Suppressor(conf)),
    subtractorCMN(SiStripRawProcessingFactory::create_SubtractorCMN(conf)),
    subtractorPed(SiStripRawProcessingFactory::create_SubtractorPed(conf))
{}

void SiStripZeroSuppressionAlgorithm::
run(std::string RawDigiType, 
    const edm::DetSetVector<SiStripRawDigi>& input,
    std::vector< edm::DetSet<SiStripDigi> >& output,
    const edm::EventSetup& es)
{
  subtractorPed->init(es);
  subtractorCMN->init(es);
  suppressor->init(es);
  
  for ( edm::DetSetVector<SiStripRawDigi>::const_iterator 
	  DSViter = input.begin(); DSViter != input.end(); DSViter++) {
    
    edm::DetSet<SiStripDigi> ssd(DSViter->id);
    
    if ( RawDigiType == "VirginRaw" ) {
      std::vector<int16_t> vssRd(DSViter->data.size());
      subtractorPed->subtract(*DSViter,vssRd);
      subtractorCMN->subtract(DSViter->id,vssRd);
      suppressor->suppress(vssRd,ssd);
    } 
    else if ( RawDigiType == "ProcessedRaw" ){
      suppressor->suppress((*DSViter),ssd);	
    }
    else throw cms::Exception("Unknown RawDigitype") << "SiStripZeroZuppression can only process types \"VirginRaw\" and \"ProcessedRaw\"";
    
    if (ssd.data.size())  output.push_back(ssd); 
  }
}
