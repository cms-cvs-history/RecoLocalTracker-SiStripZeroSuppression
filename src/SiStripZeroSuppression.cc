// File: SiStripZeroSuppression.cc
// Description:  see SiStripZeroSuppression.h
// Author:  D. Giordano
//
//--------------------------------------------
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripZeroSuppression.h"


namespace cms
{
  SiStripZeroSuppression::SiStripZeroSuppression(edm::ParameterSet const& conf): 
    conf_(conf),
    SiStripZeroSuppressionAlgorithm_(conf),
    SiStripPedestalsService_(conf),
    SiStripNoiseService_(conf),
    RawDigiProducersList(conf_.getParameter<Parameters>("RawDigiProducersList")){

    edm::LogInfo("SiStripZeroSuppression") << "[SiStripZeroSuppression::SiStripZeroSuppression] Constructing object...";


    Parameters::iterator itRawDigiProducersList = RawDigiProducersList.begin();
    for(; itRawDigiProducersList != RawDigiProducersList.end(); ++itRawDigiProducersList ) {
      std::string rawdigiLabel = itRawDigiProducersList->getParameter<std::string>("RawDigiLabel");
      produces< edm::DetSetVector<SiStripDigi> > (rawdigiLabel);
    }
  }
  
  // Virtual destructor needed.
  SiStripZeroSuppression::~SiStripZeroSuppression() { 
    edm::LogInfo("SiStripZeroSuppression") << "[SiStripZeroSuppression::~SiStripZeroSuppression] Destructing object...";
  }  
  
  //Get at the beginning Calibration data (pedestals)
  void SiStripZeroSuppression::beginJob( const edm::EventSetup& es ) {
    edm::LogInfo("SiStripZeroSuppression") << "[SiStripZeroSuppression::beginJob]";
    
    SiStripZeroSuppressionAlgorithm_.configure(&SiStripPedestalsService_,&SiStripNoiseService_);
  }

  // Functions that gets called by framework every event
  void SiStripZeroSuppression::produce(edm::Event& e, const edm::EventSetup& es)
  {
    edm::LogInfo("SiStripZeroSuppression") << "[SiStripZeroSuppression::produce] Analysing " << e.id();
   
    edm::Handle< edm::DetSetVector<SiStripRawDigi> >  input;

    Parameters::iterator itRawDigiProducersList = RawDigiProducersList.begin();
    for(; itRawDigiProducersList != RawDigiProducersList.end(); ++itRawDigiProducersList ) {

      // Step A: Get Inputs   
      std::string rawdigiProducer = itRawDigiProducersList->getParameter<std::string>("RawDigiProducer");
      std::string rawdigiLabel = itRawDigiProducersList->getParameter<std::string>("RawDigiLabel");
      e.getByLabel(rawdigiProducer,rawdigiLabel,input);  //FIXME: fix this label	
  
      // Step B: produce output product
      std::vector< edm::DetSet<SiStripDigi> > vSiStripDigi;
      vSiStripDigi.reserve(10000);
      if (input->size())
	SiStripZeroSuppressionAlgorithm_.run(rawdigiLabel,*input,vSiStripDigi);
    
      // Step C: create and fill output collection
      std::auto_ptr< edm::DetSetVector<SiStripDigi> > output(new edm::DetSetVector<SiStripDigi>(vSiStripDigi) );

      // Step E: write output to file
      e.put(output,rawdigiLabel);
    }
  }   
}
