#ifndef StiHifyRootFile_h
#define StiHifyRootFile_h

#include <map>
#include <set>
#include <string>

#include "travex/RootFile.h"
#include "stihify/StiHifyEvent.h"
#include "stihify/StiHifyAnalysisTreeMaker.h"

class StiHifyPrgOptions;


class StiHifyRootFile : public tvx::RootFile
{
public:

   StiHifyRootFile(StiHifyPrgOptions& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillHists(const StiHifyEvent &event);
   void Finalize();

private:

   StiHifyAnalysisTreeMaker* treeMaker;
};

#endif
