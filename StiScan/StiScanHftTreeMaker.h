#ifndef StiScanHftTreeMaker_h
#define StiScanHftTreeMaker_h

#include "StHftPool/HftMatchedTree/HftMatchedTree.h"


class StiScanHftTreeMaker : public HftMatchedTree
{
public:

   StiScanHftTreeMaker(const Char_t *name = "StiScanHftTreeMaker");
   virtual Int_t Make();
   virtual void  Clear(Option_t *opt = "");
   virtual const char* GetCVS() const { static const char cvs[]="Tag $Name:  $ $Id: StiScanHftTreeMaker.h,v 1.3 2014/10/13 20:21:43 smirnovd Exp $ built " __DATE__ " " __TIME__; return cvs; }

private:

   virtual void SetTree();

   ClassDef(StiScanHftTreeMaker, 0)
};

#endif
