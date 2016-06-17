#ifndef StiScanRootFile_h
#define StiScanRootFile_h

#include <map>
#include <set>
#include <string>

#include "travex/RootFile.h"

#include "GeaRootIO/TGeaEvent.h"
#include "StiScan/StiScanEvent.h"

class StiScanPrgOptions;


class StiScanRootFile : public tvx::RootFile
{
public:

   StiScanRootFile(StiScanPrgOptions& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillHists(const StiScanEvent &stiEvent, const std::set<std::string> *volumeList=0);
   void FillHists(const TGeaEvent &geaEvent, const std::set<std::string> *volumeList=0);
   void FillDerivedHists();

protected:

   StiScanPrgOptions& fPrgOptions; ///< Shadow base class reference

private:

   void FindAutoRange() const;
};

#endif
