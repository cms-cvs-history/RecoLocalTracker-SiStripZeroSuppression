#include "RecoLocalTracker/SiStripZeroSuppression/plugins/SiStripZeroSuppression.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"

#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripRawProcessingFactory.h"

#include <memory>

SiStripZeroSuppression::SiStripZeroSuppression(edm::ParameterSet const& conf)
  : inputTags(conf.getParameter<std::vector<edm::InputTag> >("RawDigiProducersList")),
    suppressor(SiStripRawProcessingFactory::create_Suppressor(conf)),
    subtractorCMN(SiStripRawProcessingFactory::create_SubtractorCMN(conf)),
    subtractorPed(SiStripRawProcessingFactory::create_SubtractorPed(conf))
{
  for( std::vector<edm::InputTag>::const_iterator  
	 inputTag = inputTags.begin(); inputTag != inputTags.end(); ++inputTag )
    produces< edm::DetSetVector<SiStripDigi> > (inputTag->label());
}

void SiStripZeroSuppression::produce(edm::Event& e, const edm::EventSetup& es)
{
  edm::Handle< edm::DetSetVector<SiStripRawDigi> >  input;
  
  for( std::vector<edm::InputTag>::const_iterator 
	 inputTag = inputTags.begin(); inputTag != inputTags.end(); ++inputTag ) {
    
    e.getByLabel(*inputTag,input);
    std::vector< edm::DetSet<SiStripDigi> > vSiStripDigi; 
    if (input->size()) {

      vSiStripDigi.reserve(10000);
      subtractorPed->init(es);
      subtractorCMN->init(es);
      suppressor->init(es);

      for ( edm::DetSetVector<SiStripRawDigi>::const_iterator 
	      DSViter = input->begin(); DSViter != input->end(); DSViter++) {
	
	edm::DetSet<SiStripDigi> ssd(DSViter->id);
	if ( inputTag->label() == "VirginRaw" ) {
	  std::vector<int16_t> vssRd(DSViter->data.size());
	  subtractorPed->subtract(*DSViter,vssRd);
	  subtractorCMN->subtract(DSViter->id,vssRd);
	  suppressor->suppress(vssRd,ssd);
	} 
	else if ( inputTag->label() == "ProcessedRaw" ){
	  suppressor->suppress((*DSViter),ssd);	
	}
	else throw cms::Exception("Unknown RawDigitype") 
	  << "SiStripZeroZuppression can only process types \"VirginRaw\" and \"ProcessedRaw\"";
	
	if (ssd.data.size())  vSiStripDigi.push_back(ssd); 
      }
      
    }
    std::auto_ptr< edm::DetSetVector<SiStripDigi> > output(new edm::DetSetVector<SiStripDigi>(vSiStripDigi) );
    
    e.put(output,inputTag->label());
  }
}   
