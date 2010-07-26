#include "RecoLocalTracker/SiStripZeroSuppression/plugins/SiStripZeroSuppression.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripRawProcessingFactory.h"
#include <memory>

SiStripZeroSuppression::
SiStripZeroSuppression(edm::ParameterSet const& conf)
  : inputTags(conf.getParameter<std::vector<edm::InputTag> >("RawDigiProducersList")),
    algorithms(SiStripRawProcessingFactory::create(conf.getParameter<edm::ParameterSet>("Algorithms"))),
    storeCM(conf.getParameter<bool>("storeCM")){
  
  for(tag_iterator_t inputTag = inputTags.begin(); inputTag != inputTags.end(); ++inputTag )
    produces< edm::DetSetVector<SiStripDigi> > (inputTag->instance());

  if(storeCM)
    produces< edm::DetSetVector<SiStripDigi> > ("APVCM");
}

void SiStripZeroSuppression::
produce(edm::Event& e, const edm::EventSetup& es) {

  algorithms->initialize(es);

  for(tag_iterator_t inputTag = inputTags.begin(); inputTag != inputTags.end(); ++inputTag ) {

    std::vector<edm::DetSet<SiStripDigi> > output_base; 
    edm::Handle< edm::DetSetVector<SiStripRawDigi> > input;
    e.getByLabel(*inputTag,input);

    if (input->size()) 
      processRaw(*inputTag, *input, output_base);

    std::auto_ptr< edm::DetSetVector<SiStripDigi> > output(new edm::DetSetVector<SiStripDigi>(output_base) );
    e.put( output, inputTag->instance() );    
  }

  if(storeCM){
    std::auto_ptr< edm::DetSetVector<SiStripDigi> > outputAPVCM(new edm::DetSetVector<SiStripDigi>(output_apvcm) );
    e.put( outputAPVCM,"APVCM");
  }
  
}


inline
void SiStripZeroSuppression::
processRaw(const edm::InputTag& inputTag, const edm::DetSetVector<SiStripRawDigi>& input, std::vector<edm::DetSet<SiStripDigi> >& output) {

  if(storeCM){
    output_apvcm.clear();
    output_apvcm.reserve(16000);
  }
  
  output.reserve(10000);    
  for ( edm::DetSetVector<SiStripRawDigi>::const_iterator 
	  rawDigis = input.begin(); rawDigis != input.end(); rawDigis++) {
    
    edm::DetSet<SiStripDigi> suppressedDigis(rawDigis->id);

    if ( "ProcessedRaw" == inputTag.instance()) {
      std::vector<int16_t> processedRawDigis;
      transform(rawDigis->begin(), rawDigis->end(), back_inserter(processedRawDigis), boost::bind(&SiStripRawDigi::adc , _1));
      algorithms->subtractorCMN->subtract( rawDigis->id, processedRawDigis);
      algorithms->suppressor->suppress( processedRawDigis, suppressedDigis );
    } else

    if ( "VirginRaw" == inputTag.instance()) {
      std::vector<int16_t> processedRawDigis(rawDigis->size());
      algorithms->subtractorPed->subtract( *rawDigis, processedRawDigis);
      algorithms->subtractorCMN->subtract( rawDigis->id, processedRawDigis);
      algorithms->suppressor->suppress( processedRawDigis, suppressedDigis );

      if(storeCM){
	const std::vector< std::pair<short,float> >& vmedians = algorithms->subtractorCMN->getAPVsCM();
	edm::DetSet<SiStripDigi> apvDetSet(rawDigis->id);
	for(size_t i=0;i<vmedians.size();++i){
	  apvDetSet.push_back(SiStripDigi(vmedians[i].first,(vmedians[i].second+512)*10));
	  // std::cout << "CM patch in VR " << rawDigis->id << " " << vmedians[i].first << " " << vmedians[i].second << std::endl;
	}
	if(apvDetSet.size())
	  output_apvcm.push_back(apvDetSet);
      }
    } else 
      
    throw cms::Exception("Unknown input type") 
      << inputTag.instance() << " unknown.  SiStripZeroZuppression can only process types \"VirginRaw\" and \"ProcessedRaw\" ";
    

    if (suppressedDigis.size()) 
      output.push_back(suppressedDigis); 

  }
}
