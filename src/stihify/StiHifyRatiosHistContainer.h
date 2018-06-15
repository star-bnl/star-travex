#ifndef StiHifyRatiosHistContainer_h
#define StiHifyRatiosHistContainer_h

#include "travex/HistContainer.h"


class StiHifyRatiosHistContainer : public tvx::HistContainer
{
public:

   StiHifyRatiosHistContainer(const char* name, TDirectory* motherDir = 0, Option_t* option = "");

   virtual void FillDerivedHists() {}
   void CreateRatioHist(const TH1* hNumer, const TH1* hDenom);

};

#endif
