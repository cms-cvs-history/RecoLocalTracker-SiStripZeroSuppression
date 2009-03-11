#include "RecoLocalTracker/SiStripZeroSuppression/interface/MedianCMNSubtractor.h"

void MedianCMNSubtractor::subtract(const uint32_t& detId,std::vector<int16_t>& digis) {subtract_(detId,digis);}
void MedianCMNSubtractor::subtract(const uint32_t& detId,std::vector<float>& digis) {subtract_(detId,digis);}

template<typename T> 
inline
void MedianCMNSubtractor::
subtract_(const uint32_t& detId,std::vector<T>& digis){
  
  std::vector<T> tmp;  tmp.reserve(128);
  typename std::vector<T>::iterator strip(digis.begin()), endAPV, high, low;
  
  for (unsigned iAPV=0; iAPV<digis.size()/128; iAPV++){

    endAPV = strip+128;   tmp.clear(); 
    tmp.insert(tmp.end(), strip, endAPV );
    const float offset = median(tmp);

    while (strip < endAPV) 
      *strip++ = static_cast<T>(*strip-offset);

  }
}
