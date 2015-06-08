#ifndef StiScanTreeMaker_h
#define StiScanTreeMaker_h

#include "StiRootIO/StiTreeMaker.h"


class StiScanTreeMaker : public StiTreeMaker
{
public:

   explicit StiScanTreeMaker(const std::string &name="StiScanTreeMaker");
   virtual Int_t Make();

private:

   virtual void SetEventTree();

   ClassDef(StiScanTreeMaker, 0)
};

#endif
