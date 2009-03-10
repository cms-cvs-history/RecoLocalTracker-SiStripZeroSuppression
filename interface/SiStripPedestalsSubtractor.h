#ifndef RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPPEDESTALSSUBTRACTOR_H
#define RECOLOCALTRACKER_SISTRIPZEROSUPPRESSION_SISTRIPPEDESTALSSUBTRACTOR_H

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"
#include "CondFormats/SiStripObjects/interface/SiStripPedestals.h"
#include <vector>

class SiStripPedestalsSubtractor {

  friend class SiStripRawProcessingFactory;

 public:
  
  ~SiStripPedestalsSubtractor(){};
  void subtract(const edm::DetSet<SiStripRawDigi>&, std::vector<int16_t>&);
  void init(const edm::EventSetup& );
  
 private:
  
  SiStripPedestalsSubtractor(){};
  edm::ESHandle<SiStripPedestals> pedestalsHandle;
  std::vector<int> peds_;
  
};
#endif
