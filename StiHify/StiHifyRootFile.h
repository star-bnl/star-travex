#ifndef StiHifyRootFile_h
#define StiHifyRootFile_h

#include <map>
#include <set>
#include <string>

#include "travex/RootFile.h"
#include "StiHify/StiHifyEvent.h"
#include "StiHify/StiHifyAnalysisTreeMaker.h"

class StiHifyPrgOptions;


class StiHifyRootFile : public tvx::RootFile
{
public:

   StiHifyRootFile(StiHifyPrgOptions& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillHists(const StiHifyEvent &event);
   void FillDerivedHists();

private:

   StiHifyAnalysisTreeMaker* treeMaker;
};

#endif
