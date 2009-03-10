#include "RecoLocalTracker/SiStripZeroSuppression/interface/MedianCMNSubtractor.h"
#include <sstream>

void MedianCMNSubtractor::subtract(const uint32_t& detId,std::vector<int16_t>& digis) {subtract_(detId,digis);}
void MedianCMNSubtractor::subtract(const uint32_t& detId,std::vector<float>& digis) {subtract_(detId,digis);}

template <typename T> 
inline
void MedianCMNSubtractor::
subtract_(const uint32_t& detId,std::vector<T>& digis){
  
#ifdef DEBUG_SiStripZeroSuppression_
  LogDebug("SiStripZeroSuppression") << "[MedianCMNSubtractor::subtract] digis.size()= " << digis.size();
#endif
  std::vector<T> APVdigis;
  typename std::vector<T>::iterator fs;
  typename std::vector<T>::iterator ls;
  float CM;
  APVdigis.reserve(128);
  int nAPV = digis.size()/128;
  typename std::vector<T>::iterator mid = APVdigis.begin() + 64;
  
#ifdef DEBUG_SiStripZeroSuppression_
  if (edm::isDebugEnabled())
    LogDebug("SiStripZeroSuppression") << "[MedianCMNSubtractor::subtract] DetId " << detId << " number of apvs: nAPV= " << nAPV;
#endif
  for (int iAPV=0; iAPV<nAPV; iAPV++){
    APVdigis.clear(); //added verify
    fs = digis.begin()+iAPV*128;
    ls = digis.begin()+(iAPV+1)*128;
    APVdigis.insert(APVdigis.end(), fs, ls );

    //// slow way: sort all 128 digis to compute the median
    // std::sort(APVdigis.begin(),APVdigis.end());
    // CM = (APVdigis[63]+APVdigis[64])/2.;
    //// faster way, without sorting all the list
    ////   see http://www.sgi.com/tech/stl/nth_element.html
    ////   and/or numerical recipes in C/C++, section 8.5 (http://www.nrbook.com/a/bookcpdf/c8-5.pdf)
    std::nth_element(APVdigis.begin(), mid, APVdigis.end());
    typename std::vector<T>::iterator i63 = std::max_element(APVdigis.begin(), mid);
    CM = (*mid + *i63)/2.;

#ifdef DEBUG_SiStripZeroSuppression_
    std::stringstream ss;
    if (edm::isDebugEnabled())
      ss << "[MedianCMNSubtractor::subtract] DetId " << detId << " iApv= " <<iAPV << " CM= " << CM << std::endl;
#endif    

    while (fs < ls) {
      *fs = static_cast<T>(*fs-CM);
#ifdef DEBUG_SiStripZeroSuppression_
      if (edm::isDebugEnabled())
	ss << "[MedianCMNSubtractor::subtract] DetId " << detId << " strip " << fs-digis.begin() << " adc CM subtr " << *fs << std::endl;
#endif
      fs++;
    }
#ifdef DEBUG_SiStripZeroSuppression_
    LogDebug("SiStripZeroSuppression") << ss.str().c_str();
#endif
  }
}
