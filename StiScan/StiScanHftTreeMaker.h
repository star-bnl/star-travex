#ifndef StiScanHftTreeMaker_h
#define StiScanHftTreeMaker_h

#include "StHftPool/HftMatchedTree/HftMatchedTree.h"


class StiScanHftTreeMaker : public HftMatchedTree
{
public:

   StiScanHftTreeMaker(const Char_t *name = "StiScanHftTreeMaker");
   virtual Int_t Make();
   virtual const char* GetCVS() const { static const char cvs[]="Tag $Name:  $ $Id: StiScanHftTreeMaker.h,v 1.2 2014/10/07 18:02:11 smirnovd Exp $ built " __DATE__ " " __TIME__; return cvs; }

private:

   virtual void SetTree();

   ClassDef(StiScanHftTreeMaker, 0)
};

#endif
