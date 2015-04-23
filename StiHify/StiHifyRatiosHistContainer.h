#ifndef StiHifyRatiosHistContainer_h
#define StiHifyRatiosHistContainer_h

#include <map>
#include <set>
#include <string>

#include "TDirectoryFile.h"
#include "TProfile2D.h"
#include "TProfile3D.h"

#include "StiHify/StiHifyEvent.h"
#include "StiRootIO/Profile2D.h"
#include "StiRootIO/Profile3D.h"
#include "StiRootIO/TStiKalmanTrack.h"
#include "StiRootIO/TStiKalmanTrackNode.h"

typedef std::map<std::string, TH1*>                    HistMap;
typedef std::map<std::string, TH1*>::iterator          HistMapIter;
typedef std::map<std::string, TH1*>::const_iterator    HistMapConstIter;


class StiHifyRatiosHistContainer : public TDirectoryFile
{
public:

   StiHifyRatiosHistContainer();
   StiHifyRatiosHistContainer(const char* name, TDirectory* motherDir = 0, Option_t* option = "");
   ~StiHifyRatiosHistContainer();

   void CreateRatioHist(const TH1* hNumer, const TH1* hDenom);
   void SaveAllAs(std::string prefix="./");

private:

   HistMap mHs;
};

#endif
