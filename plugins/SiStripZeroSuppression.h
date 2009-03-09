#ifndef SiStripZeroSuppression_h
#define SiStripZeroSuppression_h
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripZeroSuppressionAlgorithm.h"
class InputTag;

class SiStripZeroSuppression : public edm::EDProducer
{
  
 public:
  
  explicit SiStripZeroSuppression(const edm::ParameterSet&);  
  virtual ~SiStripZeroSuppression() {}
  virtual void produce(edm::Event& , const edm::EventSetup& );
  
 private:

  std::vector<edm::InputTag> inputTags;
  SiStripZeroSuppressionAlgorithm ZSalgorithm;

};
#endif
