#include "RecoLocalTracker/SiStripZeroSuppression/interface/TT6CMNSubtractor.h"

#include "CondFormats/SiStripObjects/interface/SiStripNoises.h"
#include "CalibFormats/SiStripObjects/interface/SiStripQuality.h"
#include "CondFormats/DataRecord/interface/SiStripNoisesRcd.h"
#include "CalibTracker/Records/interface/SiStripQualityRcd.h"

void TT6CMNSubtractor::init(const edm::EventSetup& es){
  es.get<SiStripNoisesRcd>().get(noiseHandle);
  es.get<SiStripQualityRcd>().get(qualityHandle);
}

void TT6CMNSubtractor::subtract(const uint32_t& detId,std::vector<int16_t>& digis){ subtract_(detId,digis);}
void TT6CMNSubtractor::subtract(const uint32_t& detId,std::vector<float>& digis){ subtract_(detId,digis);}

template<typename T>
inline
void TT6CMNSubtractor::
subtract_(const uint32_t& detId,std::vector<T>& digis){

  short FixedBias=128;
  SiStripNoises::Range detNoiseRange = noiseHandle->getRange(detId);
  SiStripQuality::Range detQualityRange = qualityHandle->getRange(detId);

  typename std::vector<T>::iterator fs,ls;
  double sumVal = 0.0;
  double sumWt =  0.0;
  
  for (uint16_t istrip=0; istrip<digis.size(); ++istrip){

    if ( !qualityHandle->IsStripBad(detQualityRange,istrip) ) {
      float stripNoise=noiseHandle->getNoise(istrip,detNoiseRange);

      if( fabs(digis[istrip]-FixedBias) < cut_to_avoid_signal_*stripNoise ) { 
	double nWeight = 1/(stripNoise*stripNoise);
	sumVal += (digis[istrip]-FixedBias)*nWeight;
	sumWt += nWeight;
      }
    }

    if (istrip%128 == 127){
      double CM = (sumWt) ? sumVal/sumWt :0.0;
      
      fs = digis.begin()+istrip-127;
      ls = digis.begin()+istrip+1;
      
      while (fs < ls)
	*fs++ = static_cast<T>(*fs-FixedBias-CM);

      sumVal = 0.0;
      sumWt =  0.0;
    }
  }
}

