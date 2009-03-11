#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripPedestalsSubtractor.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/SiStripPedestalsRcd.h"
#include "FWCore/Utilities/interface/Exception.h"

void SiStripPedestalsSubtractor::init(const edm::EventSetup& es){
  uint32_t p_cache_id = es.get<SiStripPedestalsRcd>().cacheIdentifier();
  if(p_cache_id != peds_cache_id) {
    es.get<SiStripPedestalsRcd>().get(pedestalsHandle);
    peds_cache_id = p_cache_id;
  }
}


void SiStripPedestalsSubtractor::subtract(const edm::DetSet<SiStripRawDigi>& input,std::vector<int16_t>& ssrd){

  edm::DetSet<SiStripRawDigi>::const_iterator iter=input.begin();
  std::vector<int16_t>::iterator              iout=ssrd.begin();
  SiStripPedestals::Range detPedRange = pedestalsHandle->getRange(input.id);
  
  peds_.resize(input.size());
  try{
      pedestalsHandle->allPeds(peds_, detPedRange);
  } catch(cms::Exception& e){
    //throw cms::Exception("CorruptedData")
    edm::LogError("SiStripPedestalsSubtractor")  
      << "[SiStripPedestalsSubtractor::subtract] DetId " << input.id << " propagating error from SiStripPedestal" << e.what();
    ssrd.clear();
  }

  std::vector<int>::const_iterator it_ped = peds_.begin();  
  for (;iter!=input.end(); ++iter, ++iout, ++it_ped) {

    int ped = *it_ped;
    //  uncomment to check bulk pedestal decoding
    //assert( ped == static_cast<int>(pedestalsHandle->getPed(it_ped - peds_.begin(), detPedRange)) );
    
    if(ped>895) ped -= 1024;
    *iout = iter->adc() - ped;

#ifdef DEBUG_SiStripZeroSuppression_
    if (edm::isDebugEnabled())
      LogDebug("SiStripPedestalsSubtractor") 
	<<"[SiStripPedestalsSubtractor::subtract]: DetID " << input.id << " strip " << (it_ped - peds.begin()) << " adc before sub= " 
	<< iter->adc() 
	<< "\t pedval= " << pedestalsHandle->getPed(it_ped - peds.begin(),detPedRange)
	<< "\t adc pedsub = " << ssrd[it_ped - peds.begin()]
	<< std::endl;
#endif
  }
}
