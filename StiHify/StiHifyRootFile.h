#ifndef StiHifyRootFile_h
#define StiHifyRootFile_h

#include <map>
#include <set>
#include <string>

#include "TH2S.h"
#include "TDirectoryFile.h"
#include "TFile.h"
#include "StMaker.h"

#include "StiHify/StiHifyEvent.h"

class StiScanPrgOptions;

typedef std::map<std::string, TDirectoryFile*>                   TDirMap;
typedef std::map<std::string, TDirectoryFile*>::iterator         TDirMapIter;
typedef std::map<std::string, TDirectoryFile*>::const_iterator   TDirMapConstIter;


class StiHifyRootFile : public TFile
{
public:

   StiHifyRootFile(StiScanPrgOptions& prgOpts);
   StiHifyRootFile(StiScanPrgOptions& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillHists(const StiHifyEvent &event, const std::set<std::string> *volumeList=0);
   void FillDerivedHists();
   virtual Int_t Write(const char* name = 0, Int_t opt = 0, Int_t bufsiz = 0);
   virtual Int_t Write(const char* name = 0, Int_t opt = 0, Int_t bufsiz = 0) const;
   virtual void Close(Option_t *option="");
   void SaveAllAs(std::string prefix="./");

private:

   void BookHists();

   TDirMap mDirs;   ///< A string-to-TDirectoryFile map for convenient access to enclosed directories
   StiScanPrgOptions& fPrgOptions; ///< Command line arguments and options requested by the user
};

#endif
