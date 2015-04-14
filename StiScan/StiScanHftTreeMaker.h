#ifndef StiScanHftTreeMaker_h
#define StiScanHftTreeMaker_h

#include "StiRootIO/StiTreeMaker.h"


class StiScanHftTreeMaker : public StiTreeMaker
{
public:

   StiScanHftTreeMaker(const std::string name="StiScanHftTreeMaker");
   virtual Int_t Make();

private:

   virtual void SetEventTree();

   ClassDef(StiScanHftTreeMaker, 0)
};

#endif
