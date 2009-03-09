#ifndef RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPTT6COMMONMODENOISESUBTRACTION_H
#define RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPTT6COMMONMODENOISESUBTRACTION_H

#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripCommonModeNoiseSubtractor.h"


class SiStripTT6CommonModeNoiseSubtraction : public SiStripCommonModeNoiseSubtractor {
public:
  
  SiStripTT6CommonModeNoiseSubtraction(double in):SiStripCommonModeNoiseSubtractor(in){};
  ~SiStripTT6CommonModeNoiseSubtraction(){};
  
  void init(const edm::EventSetup& es);
  void subtract(const uint32_t&,std::vector<int16_t>&);

};
#endif
