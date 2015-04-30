#ifndef StiRootFile_h
#define StiRootFile_h

#include <map>
#include <string>

#include "TDirectoryFile.h"
#include "TFile.h"

#include "StiRootIO/PrgOptionProcessor.h"


typedef std::map<std::string, TDirectoryFile*>                   TDirMap;
typedef std::map<std::string, TDirectoryFile*>::iterator         TDirMapIter;
typedef std::map<std::string, TDirectoryFile*>::const_iterator   TDirMapConstIter;


class StiRootFile : public TFile
{
public:

   StiRootFile(PrgOptionProcessor& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillDerivedHists();

   virtual Int_t Write(const char* name = 0, Int_t opt = 0, Int_t bufsiz = 0);
   virtual Int_t Write(const char* name = 0, Int_t opt = 0, Int_t bufsiz = 0) const;
   virtual void Close(Option_t *option="");
   void SaveAllAs(std::string prefix="./");

protected:

   TDirMap mDirs;   ///< A string-to-TDirectoryFile map for convenient access to enclosed directories
   PrgOptionProcessor& fPrgOptions; ///< Command line arguments and options requested by the user
};

#endif
