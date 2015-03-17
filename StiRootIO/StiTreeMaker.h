#ifndef StiTreeMaker_h
#define StiTreeMaker_h

#include "TFile.h"
#include "TTree.h"

#include "StChain/StMaker.h"
#include "StiRootIO/TStiEvent.h"


class StiTreeMaker : public StMaker
{
public:

   StiTreeMaker(const Char_t *name = "StiTreeMaker");
   virtual ~StiTreeMaker();
   virtual Int_t Init();
   virtual Int_t Make() = 0;
   virtual void  Clear(Option_t *opt = "");
   virtual Int_t Finish();

protected:

   virtual void SetEventTree() = 0;

   TFile     *fFile;
   TTree     *fTree;
   TStiEvent *fEvent;

   ClassDef(StiTreeMaker, 0)
};

#endif
