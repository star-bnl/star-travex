#ifndef StiHifyRatiosHistContainer_h
#define StiHifyRatiosHistContainer_h

#include "StiRootIO/StiHistContainer.h"


class StiHifyRatiosHistContainer : public tvx::StiHistContainer
{
public:

   StiHifyRatiosHistContainer(const char* name, TDirectory* motherDir = 0, Option_t* option = "");

   virtual void FillDerivedHists() {}
   void CreateRatioHist(const TH1* hNumer, const TH1* hDenom);

protected:

   virtual void BookHists() {}
};

#endif
