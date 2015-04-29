#ifndef StiScanRootFile_h
#define StiScanRootFile_h

#include <map>
#include <set>
#include <string>

#include "TH2S.h"
#include "TDirectoryFile.h"
#include "TFile.h"
#include "StMaker.h"

#include "StiRootIO/StiRootFile.h"
#include "StiScan/StiScanEvent.h"
#include "GeaRootIO/TGeaEvent.h"

class StiScanPrgOptions;


class StiScanRootFile : public StiRootFile
{
public:

   StiScanRootFile(StiScanPrgOptions& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillHists(const StiScanEvent &eventT, const std::set<std::string> *volumeList=0);
   void FillHists(const TGeaEvent &eventG, const std::set<std::string> *volumeList=0);
   void FillDerivedHists();

protected:

   StiScanPrgOptions& fPrgOptions; ///< Shadow base class reference

private:

   void FindAutoRange() const;
   void BookHists();
};

#endif
