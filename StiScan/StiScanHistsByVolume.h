#ifndef StiScanHistsByVolume_h
#define StiScanHistsByVolume_h

#include <map>
#include <set>
#include <string>

#include "StiScan/StiScanHistContainer.h"
#include "StiScan/StiScanPrgOptions.h"



class StiScanHistsByVolume : public StiScanHistContainer
{
public:

   StiScanHistsByVolume(StiScanPrgOptions& prgOpts);
   StiScanHistsByVolume(StiScanPrgOptions& prgOpts, const char* name, TDirectory* motherDir = 0, bool doProjection=false, Option_t* option = "");

protected:

   virtual void FillHists(const TStiKalmanTrack &kalmTrack, const std::set<std::string> *volumeList=0);
};

#endif
