#ifndef StiHifyRootFile_h
#define StiHifyRootFile_h

#include <map>
#include <set>
#include <string>

#include "TH2S.h"
#include "TDirectoryFile.h"
#include "TFile.h"
#include "StMaker.h"

#include "StiRootIO/StiRootFile.h"
#include "StiHify/StiHifyEvent.h"

class StiScanPrgOptions;


class StiHifyRootFile : public StiRootFile
{
public:

   StiHifyRootFile(StiScanPrgOptions& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillHists(const StiHifyEvent &event, const std::set<std::string> *volumeList=0);
   void FillDerivedHists();

private:

   void BookHists();
};

#endif
