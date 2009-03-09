#ifndef RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPMEDIANCOMMONMODENOISESUBTRACTION_H
#define RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPMEDIANCOMMONMODENOISESUBTRACTION_H

#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripCommonModeNoiseSubtractor.h"


class SiStripMedianCommonModeNoiseSubtraction : public SiStripCommonModeNoiseSubtractor {
public:
  
  SiStripMedianCommonModeNoiseSubtraction(){};
  ~SiStripMedianCommonModeNoiseSubtraction(){};
  
  void init(const edm::EventSetup& es){};
  void subtract(const uint32_t&,std::vector<int16_t>&);

};
#endif
