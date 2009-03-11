#include "RecoLocalTracker/SiStripZeroSuppression/interface/MedianCMNSubtractor.h"

void MedianCMNSubtractor::subtract(const uint32_t& detId,std::vector<int16_t>& digis) {subtract_(detId,digis);}
void MedianCMNSubtractor::subtract(const uint32_t& detId,std::vector<float>& digis) {subtract_(detId,digis);}

template<typename T> 
inline
void MedianCMNSubtractor::
subtract_(const uint32_t& detId,std::vector<T>& digis){
  
  std::vector<T> tmp;  tmp.reserve(128);
  typename std::vector<T>::iterator  
    strip( digis.begin() ), 
    end(   digis.end()   ),
    endAPV;
  
  while( strip < end ) {
    endAPV = strip+128;
    const float offset = median(std::vector<int>(strip,endAPV));

    while (strip < endAPV) {
      *strip = static_cast<T>(*strip-offset);
      strip++;
    }

  }
}
