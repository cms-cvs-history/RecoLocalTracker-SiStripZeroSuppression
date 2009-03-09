// File: SiStripFastLinearCommonModeNoiseSubtraction.cc
// Description:  A fast linear common mode subtraction algorithm by D. Abbaneo to be implemented on FED level
// Author:  Alexander Dierlamm

#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripFastLinearCommonModeNoiseSubtraction.h"
#include <sstream>

void SiStripFastLinearCommonModeNoiseSubtraction::subtract(const uint32_t& detId,std::vector<int16_t>& digis){
#ifdef DEBUG_SiStripZeroSuppression_
  LogDebug("SiStripZeroSuppression") << "[SiStripFastLinearCommonModeNoiseSubtraction::subtract] digis.size()= " << digis.size();
#endif

  std::vector<int16_t> APVdiff;
  std::vector<int16_t> APVdigis;
  std::vector<int16_t>::iterator fs;
  std::vector<int16_t>::iterator ls;
  APVdiff.reserve(64);
  APVdigis.reserve(128);
  int nAPV = digis.size()/128;
  double slope;
  double offset;

#ifdef DEBUG_SiStripZeroSuppression_
  LogDebug("SiStripZeroSuppression") << "[SiStripFastLinearCommonModeNoiseSubtraction::subtract] number of apvs: nAPV= " << nAPV;
#endif

  for (int iAPV=0; iAPV<nAPV; iAPV++){
    APVdigis.clear();
    fs = digis.begin()+iAPV*128;
    ls = digis.begin()+(iAPV+1)*128;
    APVdigis.insert(APVdigis.end(), fs, ls );
    std::sort(APVdigis.begin(),APVdigis.end());
    offset = (APVdigis[63]+APVdigis[64])/2.;

    // Here we get the slope according to a proposal by D. Abbaneo
    // Details on http://abbaneo.web.cern.ch/abbaneo/cmode/cm.html
    APVdiff.clear();
    for(int s=0;s<64;s++)
      APVdiff.push_back(digis[s+64+iAPV*128]-digis[s+iAPV*128]);
    std::sort(APVdiff.begin(),APVdiff.end());
    slope = (APVdiff[31]+APVdiff[32])/2./64.;

#ifdef DEBUG_SiStripZeroSuppression_
    std::stringstream ss;
    if (edm::isDebugEnabled())
      ss << "[SiStripFastLinearCommonModeNoiseSubtraction::subtract] iApv= " <<iAPV << " slope= " << slope << " offset= " << offset << std::endl;
#endif

    int strip=1;
    while (fs < ls) {
#ifdef DEBUG_SiStripZeroSuppression_
      if (edm::isDebugEnabled())
	ss << "[SiStripFastLinearCommonModeNoiseSubtraction::subtract] initial strip "<<strip<<": " << *fs << " - " << (offset+(strip-64)*slope);
#endif
      *fs = (int16_t) (*fs-(offset+(strip-64)*slope));
#ifdef DEBUG_SiStripZeroSuppression_
      ss << " ==>>   adc CM subtr " << *fs << std::endl;
#endif
      fs++;
      strip++;
    }
#ifdef DEBUG_SiStripZeroSuppression_
    LogDebug("SiStripZeroSuppression") << ss.str().c_str();
#endif
  }// end APV loop

}

