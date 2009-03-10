#ifndef RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPCOMMONMODENOISESUBTRACTOR_H
#define RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPCOMMONMODENOISESUBTRACTOR_H

#include "FWCore/Framework/interface/EventSetup.h"
#include <vector>

class SiStripCommonModeNoiseSubtractor {

  friend class SiStripRawProcessingFactory;

 public:
  
  virtual ~SiStripCommonModeNoiseSubtractor() {};
  virtual void init(const edm::EventSetup& es) {};
  virtual void subtract(const uint32_t&,std::vector<int16_t>&)=0;
  
 protected:

  SiStripCommonModeNoiseSubtractor(){};
  
};
#endif
