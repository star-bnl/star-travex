#ifndef StiScanHftTreeMaker_h
#define StiScanHftTreeMaker_h

#include "StHftPool/HftMatchedTree/HftMatchedTree.h"


class StiScanHftTreeMaker : public HftMatchedTree
{
public:

   StiScanHftTreeMaker(const Char_t *name = "StiScanHftTreeMaker");
   virtual Int_t Make();

private:

   virtual void SetTree();

   ClassDef(StiScanHftTreeMaker, 0)
};

#endif
