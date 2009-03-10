#ifndef SiStripZeroSuppressionAlgorithm_h
#define SiStripZeroSuppressionAlgorithm_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripFedZeroSuppression.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripPedestalsSubtractor.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripCommonModeNoiseSubtractor.h"
#include <memory>

class SiStripDigi;
class SiStripRawDigi;

class SiStripZeroSuppressionAlgorithm 
{
 public:
  
  SiStripZeroSuppressionAlgorithm(const edm::ParameterSet& conf);
  ~SiStripZeroSuppressionAlgorithm() {}
  void run(std::string RawDigiType, 
	   const edm::DetSetVector<SiStripRawDigi>& input,
	   std::vector< edm::DetSet<SiStripDigi> >& output,
	   const edm::EventSetup& es);

 private:

  std::auto_ptr<SiStripFedZeroSuppression> suppressor;
  std::auto_ptr<SiStripCommonModeNoiseSubtractor> subtractorCMN;
  std::auto_ptr<SiStripPedestalsSubtractor> subtractorPed;

};
#endif
