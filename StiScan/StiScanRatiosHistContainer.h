#ifndef StiScanRatiosHistContainer_h
#define StiScanRatiosHistContainer_h

#include "StiRootIO/StiHistContainer.h"


class StiScanRatiosHistContainer : public tvx::StiHistContainer
{
public:

   StiScanRatiosHistContainer(const char* name, TDirectory* motherDir = 0, Option_t* option = "");

   virtual void FillDerivedHists() {}
   void CreateRatioHist(const TH1* hNumer, const TH1* hDenom);

protected:

   virtual void BookHists() {}
};

#endif
