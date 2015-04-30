#ifndef StiHifyRatiosHistContainer_h
#define StiHifyRatiosHistContainer_h

#include <map>
#include <set>
#include <string>

#include "TProfile2D.h"
#include "TProfile3D.h"

#include "StiHify/StiHifyEvent.h"
#include "StiRootIO/Profile2D.h"
#include "StiRootIO/Profile3D.h"
#include "StiRootIO/TStiKalmanTrack.h"
#include "StiRootIO/TStiKalmanTrackNode.h"
#include "StiRootIO/StiHistContainer.h"


class StiHifyRatiosHistContainer : public StiHistContainer
{
public:

   StiHifyRatiosHistContainer(const char* name, TDirectory* motherDir = 0, Option_t* option = "");

   virtual void FillDerivedHists() {}
   void CreateRatioHist(const TH1* hNumer, const TH1* hDenom);

protected:

   virtual void BookHists() {}
};

#endif
