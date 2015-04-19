#ifndef StiHifyHistContainer_h
#define StiHifyHistContainer_h

#include <map>
#include <set>
#include <string>

#include "TDirectoryFile.h"
#include "TProfile2D.h"
#include "TProfile3D.h"

#include "StiHify/StiHifyEvent.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiRootIO/Profile2D.h"
#include "StiRootIO/Profile3D.h"
#include "StiRootIO/TStiKalmanTrack.h"

typedef std::map<std::string, TH1*>                    HistMap;
typedef std::map<std::string, TH1*>::iterator          HistMapIter;
typedef std::map<std::string, TH1*>::const_iterator    HistMapConstIter;


class StiHifyHistContainer : public TDirectoryFile
{
public:

   StiHifyHistContainer(StiScanPrgOptions& prgOpts);
   StiHifyHistContainer(StiScanPrgOptions& prgOpts, const char* name, TDirectory* motherDir = 0, bool doProjection=true, Option_t* option = "");
   ~StiHifyHistContainer();

   void FillHists(const StiHifyEvent &event, const std::set<std::string> *volumeList=0);
   void FillDerivedHists();
   void SaveAllAs(std::string prefix="./");

protected:

   void BookHists();
   virtual void FillHists(const TStiKalmanTrack &kalmTrack, const std::set<std::string> *volumeList=0);

   HistMap mHs;

};

#endif
