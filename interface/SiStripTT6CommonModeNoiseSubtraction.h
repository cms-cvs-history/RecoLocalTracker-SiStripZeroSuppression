#ifndef RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPTT6COMMONMODENOISESUBTRACTION_H
#define RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPTT6COMMONMODENOISESUBTRACTION_H
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripCommonModeNoiseSubtractor.h"

#include "FWCore/Framework/interface/ESHandle.h"
class SiStripNoises;
class SiStripQuality;

class SiStripTT6CommonModeNoiseSubtraction : public SiStripCommonModeNoiseSubtractor {
  
  friend class SiStripRawProcessingFactory;
  
 public:
  
  void init(const edm::EventSetup& es);
  void subtract(const uint32_t&,std::vector<int16_t>&);
  void subtract(const uint32_t&,std::vector<float>&);
  
 private:

  template<typename T >void subtract_(const uint32_t&,std::vector<T>&);

  SiStripTT6CommonModeNoiseSubtraction(double in) : cut_to_avoid_signal_(in) {};
  double cut_to_avoid_signal_;
  edm::ESHandle<SiStripNoises> noiseHandle;
  edm::ESHandle<SiStripQuality> qualityHandle;
  
};
#endif
