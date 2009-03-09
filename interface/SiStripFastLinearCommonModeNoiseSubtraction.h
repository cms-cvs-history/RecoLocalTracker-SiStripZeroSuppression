#ifndef RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPFASTLINEARCOMMONMODENOISESUBTRACTION_H
#define RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPFASTLINEARCOMMONMODENOISESUBTRACTION_H

#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripCommonModeNoiseSubtractor.h"


class SiStripFastLinearCommonModeNoiseSubtraction : public SiStripCommonModeNoiseSubtractor {
public:
  
  SiStripFastLinearCommonModeNoiseSubtraction(){};
  ~SiStripFastLinearCommonModeNoiseSubtraction(){};
  
  void init(const edm::EventSetup& es){};
  void subtract(const uint32_t&,std::vector<int16_t>&);
  
};
#endif
