#ifndef StiHifyTreeMaker_h
#define StiHifyTreeMaker_h

#include <string>

#include "StiRootIO/StiTreeMaker.h"

class StiVolumeFilter;


class StiHifyTreeMaker : public StiTreeMaker
{
public:

   explicit StiHifyTreeMaker(const std::string &name="StiHifyTreeMaker");
   virtual Int_t Make();
   void SetOutputFileName(TString t){StiTreeMaker::setOutputFileName(t);};

private:

   virtual void SetEventTree();

   const StiVolumeFilter&  fStiVolumeFilter;   ///< Reference to singleton with selection criteria

   ClassDef(StiHifyTreeMaker, 0)
};

#endif
