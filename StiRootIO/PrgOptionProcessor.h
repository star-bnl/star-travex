#ifndef PrgOptionProcessor_h
#define PrgOptionProcessor_h

#include <set>
#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include "TChain.h"

#include "travex/ProgramOptions.h"


namespace {
   namespace po = boost::program_options;
}


/**
 * Processes and controls user options provided in the command line.
 */
class PrgOptionProcessor : public tvx::ProgramOptions
{
public:

   PrgOptionProcessor();
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

   std::string             fGeomFilePath;    ///< Full path to a ROOT file with TGeo geometry
   std::string             fVolumeListFile;  ///< Full path to a text file with Sti/TGeo volume names
   std::string             fVolumePattern;   ///< Regex pattern provided by the user in the command line
   std::set<std::string>   fVolumeList;      ///< A list of volume names to consider
   TChain *fStiTChain;

private:

   void InitOptions();
};

#endif
