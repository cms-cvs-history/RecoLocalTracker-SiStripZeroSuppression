#ifndef SiStripZeroSuppressionAlgorithm_h
#define SiStripZeroSuppressionAlgorithm_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"

class SiStripDigi;
class SiStripRawDigi;
class SiStripPedestalsSubtractor;
class SiStripFedZeroSuppression;
class SiStripCommonModeNoiseSubtractor;


class SiStripZeroSuppressionAlgorithm 
{
 public:
  
  SiStripZeroSuppressionAlgorithm(const edm::ParameterSet& conf);
  ~SiStripZeroSuppressionAlgorithm();
  void run(std::string RawDigiType, 
	   const edm::DetSetVector<SiStripRawDigi>& input,
	   std::vector< edm::DetSet<SiStripDigi> >& output,
	   const edm::EventSetup& es);

 private:

  SiStripFedZeroSuppression* SiStripZeroSuppressor_;
  SiStripCommonModeNoiseSubtractor* SiStripCommonModeNoiseSubtractor_;
  SiStripPedestalsSubtractor* SiStripPedestalsSubtractor_;

};
#endif
