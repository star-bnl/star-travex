#ifndef StiRootFile_h
#define StiRootFile_h

#include <map>
#include <string>

#include "TFile.h"

#include "StiRootIO/PrgOptionProcessor.h"
#include "StiRootIO/StiHistContainer.h"


namespace tvx {

typedef std::map<std::string, StiHistContainer*>   StiHistContainers;


/**
 * A basic extention of the ROOT's TFile class to manage a set of user defined
 * histogram containers by arranging them in subdirectories.
 */
class StiRootFile : public TFile
{
public:

   StiRootFile(PrgOptionProcessor& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillDerivedHists();

   virtual void Close(Option_t *option="");
   void SaveAllAs(std::string prefix="./");

protected:

   /// A string-to-StiHistContainer map for convenient access to enclosed directories
   StiHistContainers mDirs;

   /// Command line arguments and options requested by the user
   PrgOptionProcessor& fPrgOptions;
};

}

#endif
