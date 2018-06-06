#ifndef PrgOptionProcessor_h
#define PrgOptionProcessor_h

#include <set>
#include <string>

#include "TChain.h"

#include "travex/ProgramOptions.h"


/**
 * Processes and controls user options provided in the command line.
 */
class PrgOptionProcessor : public tvx::ProgramOptions
{
public:

   PrgOptionProcessor(int argc, char **argv, const std::string& stiTreeName="t");
   ~PrgOptionProcessor();

   std::string  PathToStiTreeFile() const { return fInFilePath; }
   std::string  PathToGeometryFile() const { return fGeomFilePath; }
   const std::set<std::string>&  GetVolumeList() const { return fVolumeList; }
   TChain* GetStiTChain() { return fStiTChain; }

   void ProcessOptions();
   bool MatchedVolName(const std::string & volName) const;

protected:

   void BuildInputChains();
   virtual void VerifyOptions();
   virtual void AddToInputChains(std::string stiTreeRootFileName);

   /// Full path to a ROOT file with TGeo geometry
   std::string  fGeomFilePath;

   /// Full path to a text file with Sti/TGeo volume names
   std::string  fVolumeListFile;

   /// Regex pattern provided by the user in the command line
   std::string  fVolumePattern;

   /// A list of volume names to consider
   std::set<std::string>  fVolumeList;

   /// A (chained) TTree from the input file
   TChain *fStiTChain;
};

#endif
