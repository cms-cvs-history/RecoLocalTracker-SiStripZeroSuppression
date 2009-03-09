#include "RecoLocalTracker/SiStripZeroSuppression/plugins/SiStripZeroSuppression.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"

#include <memory>

SiStripZeroSuppression::SiStripZeroSuppression(edm::ParameterSet const& conf)
  : inputTags(conf.getParameter<std::vector<edm::InputTag> >("RawDigiProducersList")),
    ZSalgorithm(conf)
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
      ZSalgorithm.run(inputTag->label(),*input,vSiStripDigi,es);
    }
    std::auto_ptr< edm::DetSetVector<SiStripDigi> > output(new edm::DetSetVector<SiStripDigi>(vSiStripDigi) );
    
    e.put(output,inputTag->label());
  }
}   
