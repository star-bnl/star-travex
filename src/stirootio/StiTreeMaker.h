#ifndef StiTreeMaker_h
#define StiTreeMaker_h

#include <string>

#include "TFile.h"
#include "TTree.h"

#include "StChain/StMaker.h"
#include "stirootio/TStiEvent.h"


class StiTreeMaker : public StMaker
{
public:

   StiTreeMaker(const std::string &name="StiTreeMaker", const std::string &suffix="stitree");
   virtual ~StiTreeMaker();
   virtual Int_t Init();
   virtual Int_t Make() = 0;
   virtual void  Clear(Option_t *opt = "");
   virtual Int_t Finish();

protected:

   virtual void SetEventTree() = 0;

   TFile     *fFile;
   std::string  fSuffix;  ///< A string to append to the output file name
   TTree     *fTree;
   TStiEvent *fEvent;

   ClassDef(StiTreeMaker, 0)
};

#endif
