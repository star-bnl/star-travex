#ifndef StiHifyRootFile_h
#define StiHifyRootFile_h

#include <map>
#include <set>
#include <string>

#include "StiRootIO/StiRootFile.h"
#include "StiHify/StiHifyEvent.h"

class StiHifyPrgOptions;


class StiHifyRootFile : public tvx::StiRootFile
{
public:

   StiHifyRootFile(StiHifyPrgOptions& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillHists(const StiHifyEvent &event, const std::set<std::string> *volumeList=0);
   void FillDerivedHists();

private:

   void BookHists(const StiHifyPrgOptions& prgOpts);
};

#endif
