#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/regex.hpp>

#include "TFile.h"
#include "TSystem.h"

#include "travex/utils.h"
#include "StiRootIO/PrgOptionProcessor.h"



PrgOptionProcessor::PrgOptionProcessor(int argc, char **argv, const std::string& stiTreeName) :
   tvx::ProgramOptions(argc, argv),
   fGeomFilePath(),
   fVolumeListFile(),
   fVolumePattern(),
   fVolumeList(),
   fStiTChain(new TChain(stiTreeName.c_str(), "READ"))
{
   // Declare supported options
   fOptions.add_options()

      ("geom-file",
       po::value<std::string>(&fGeomFilePath)->default_value("y2014a.root"),
       "Full path to a ROOT file with TGeo geometry")

      ("volume-pattern,p",
       po::value<std::string>(&fVolumePattern)->implicit_value("process_all_volumes"),
       "A regex pattern to match Sti/TGeo volume names. " \
       "If specified without a value all volumes will be matched")

      ("volume-pattern-flist,l",
       po::value<std::string>(&fVolumeListFile),
       "Full path to a text file with regex patterns to match Sti/TGeo volume names")
   ;

   // Set default values for Sti volume name patterns. These are used if the user does not specify any
   fVolumeList.insert("^.*IDSM_1/PXMO_1/PXLA_[\\d]+/LADR_\\d/PXSI_[\\d]+/PLAC_1.*$");
   fVolumeList.insert("^.*IDSM_1/IBMO_1/IBAM_[\\d]+/IBLM_\\d/IBSS_1.*$");
   fVolumeList.insert("^.*IDSM_1/SFMO_1/SFLM_[\\d]+/SFSW_[\\d]+/SFSL_1/SFSD_1.*$");
}


PrgOptionProcessor::~PrgOptionProcessor()
{
   delete fStiTChain; fStiTChain = nullptr;
}


/**
 * Takes the standard command line arguments and parses them with the boost
 * program_options utility. Additional checks are implemented to verify the
 * validity of the supplied arguments.
 */
void PrgOptionProcessor::ProcessOptions()
{
   po::store(po::parse_command_line(fArgc, fArgv, fOptions), fOptionsValues);
   po::notify(fOptionsValues);

   VerifyOptions();

   // Create chains based on the user input
   BuildInputChains();
}


void PrgOptionProcessor::VerifyOptions()
{
   TVX_INFO("User provided options:");

   tvx::ProgramOptions::VerifyOptions();

   if (fOptionsValues.count("geom-file"))
   {
      std::string pathToGeomFile = boost::any_cast<std::string>(fOptionsValues["geom-file"].value());
      std::cout << "File with TGeo geometry: " << pathToGeomFile << std::endl;

      if ( !std::ifstream(pathToGeomFile.c_str()).good() )
         TVX_FATAL("File \"%s\" does not exist", pathToGeomFile.c_str());
   } else
      TVX_FATAL("Geometry file not set");


   if (fOptionsValues.count("volume-pattern-flist"))
   {
      std::cout << "fVolumeListFile: " << fVolumeListFile << std::endl;
      std::ifstream volListFile(fVolumeListFile.c_str());

      if (!volListFile.good()) {
         TVX_FATAL("File \"%s\" does not exist", fVolumeListFile.c_str());
      }

      fVolumeList.clear();
      std::string pattern;

      while ( volListFile.good() )
      {
         volListFile >> pattern;

         try {
            boost::regex re(pattern);
         }
         catch (boost::regex_error& e) {
            TVX_FATAL("Provided regex \"%s\" is not valid", pattern.c_str());
         }

         if (volListFile.eof()) break;

         fVolumeList.insert(pattern);
      }

      TVX_INFO("User patterns (fVolumeList) are:");
      std::copy(fVolumeList.begin(), fVolumeList.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
   } else {
      TVX_INFO("Default patterns (fVolumeList) are:");
      std::copy(fVolumeList.begin(), fVolumeList.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
   }


   if (fOptionsValues.count("volume-pattern"))
   {
      // Process implicit value first
      if (fVolumePattern.compare("process_all_volumes") == 0) {
         fVolumeList.clear();
      }
      else {

         try {
            boost::regex re(fVolumePattern);
         }
         catch (boost::regex_error& e) {
            TVX_FATAL("Provided regex \"%s\" is not valid", fVolumePattern.c_str());
         }

         fVolumeList.clear();
         fVolumeList.insert(fVolumePattern);
      }

      TVX_INFO("User patterns (fVolumeList) are:");
      std::copy(fVolumeList.begin(), fVolumeList.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
   }
}


/**
 * Note that the function returns true when the internal list of regex'es formed
 * by the user specified options is empty.
 */
bool PrgOptionProcessor::MatchedVolName(const std::string & volName) const
{
   if (volName.empty() || fVolumeList.empty())
      return true;

   std::set<std::string>::const_iterator iPattern = fVolumeList.begin();

   for( ; iPattern != fVolumeList.end(); ++iPattern )
   {
      boost::regex r(*iPattern);

      if ( boost::regex_match(volName, r) )
         return true;
   }

   return false;
}


void PrgOptionProcessor::BuildInputChains()
{
   TFile file( fInFilePath.c_str() );

   if ( file.IsZombie() )
   {
      TVX_WARNING("Input file is not a root file: %s\nWill treat it as a file list", fInFilePath.c_str());

      std::ifstream treeListFile(fInFilePath.c_str());

      while ( treeListFile.good() )
      {
         std::string treeFileName;
         treeListFile >> treeFileName;
         if (treeListFile.eof()) break;

         AddToInputChains(treeFileName);
      }
   } else
   {
      TVX_INFO("Found root file: %s", fInFilePath.c_str());
      AddToInputChains(fInFilePath);
   }
}


/**
 * This private method takes a path to a valid ROOT file as input. No check is
 * done to make sure the input chains exist.
 *
 * XXX: Need to add a check to validate the tree in the file.
 */
void PrgOptionProcessor::AddToInputChains(std::string stiTreeRootFileName)
{
   TFile file( stiTreeRootFileName.c_str() );

   if ( file.IsZombie() )
      TVX_FATAL("Input file is not a valid root file: %s", stiTreeRootFileName.c_str());

   fStiTChain->AddFile( stiTreeRootFileName.c_str() );
   TVX_INFO("Found valid ROOT file with Sti tree: %s", stiTreeRootFileName.c_str());
}
