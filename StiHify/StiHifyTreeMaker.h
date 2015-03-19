#ifndef StiHifyTreeMaker_h
#define StiHifyTreeMaker_h

#include <string>

#include "StiRootIO/StiTreeMaker.h"


class StiHifyTreeMaker : public StiTreeMaker
{
public:

   StiHifyTreeMaker(const std::string name = "StiHifyTreeMaker");
   virtual Int_t Make();

private:

   virtual void SetEventTree();

   ClassDef(StiHifyTreeMaker, 0)
};

#endif
