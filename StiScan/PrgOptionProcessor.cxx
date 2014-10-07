#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/regex.hpp>

#include "TFile.h"
#include "TSystem.h"

#include "StiScan/PrgOptionProcessor.h"


PrgOptionProcessor::PrgOptionProcessor() : TObject(),
   fArgc(0), fArgv(),
   fOptions("Program options", 120), fOptionsValues(), fHftreeFile(), fVolumeListFile(),
   fVolumePattern(),
   fVolumeList(), fMaxEventsUser(0), fSparsity(1), fSaveGraphics(false),
   fEnvVars(), fHftChain(0)
{
   InitOptions();
   InitEnvVars();
}


PrgOptionProcessor::PrgOptionProcessor(int argc, char **argv, const std::string& hftTreeName) : TObject(),
   fArgc(argc), fArgv(argv),
   fOptions("Program options", 120), fOptionsValues(), fHftreeFile(), fVolumeListFile(),
   fVolumePattern(),
   fVolumeList(), fMaxEventsUser(0), fSparsity(1), fSaveGraphics(false),
   fEnvVars(), fHftChain(new TChain(hftTreeName.c_str(), "READ"))
{
   InitOptions();
   InitEnvVars();
}


void PrgOptionProcessor::InitOptions()
{
   // Declare supported options
   fOptions.add_options()
      ("help,h",              "Print help message")
      ("hftree-file,f",       po::value<std::string>(&fHftreeFile), "Full path to a ROOT file containing a 'hftree' TTree " \
                              "OR a text file with a list of such ROOT files")
      ("volume-pattern,p",    po::value<std::string>(&fVolumePattern)->implicit_value("process_all_volumes"),
                              "A regex pattern to match Sti/TGeo volume names. If specified without a value all volumes will be matched")
      ("volume-pattern-flist,l",   po::value<std::string>(&fVolumeListFile), "Full path to a text file with regex patterns to match Sti/TGeo volume names")
      ("max-events,n",        po::value<unsigned int>(&fMaxEventsUser)->default_value(0), "Maximum number of events to process")
      ("sparsity,s",          po::value<float>(&fSparsity)->default_value(1), "Approximate fraction of events to read and process")
      ("save-graph,g",        "Use this option to save plots in png format")
   ;

   // Set default values for Sti volume name patterns. These are used if the user does not specify any
   fVolumeList.insert("^.*IDSM_1/IBMO_1/IBAM_[\\d]+/ILSB.*$");
   fVolumeList.insert("^.*IDSM_1/PXMO_1/PXLA_[\\d]+/LADR_\\d/PXSI_[\\d]+/PLAC.*$");
}


void PrgOptionProcessor::InitEnvVars()
{
   const char* tmpEnv = getenv("OFFLINE_HFT_DIR");

   if (tmpEnv) fEnvVars["OFFLINE_HFT_DIR"] = tmpEnv;
   else        fEnvVars["OFFLINE_HFT_DIR"] = ".";
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
   if (fOptionsValues.count("help"))
   {
      std::cout << fOptions << std::endl;
      exit(EXIT_FAILURE);
   }

   Info("VerifyOptions", "User provided options:");


   if (fOptionsValues.count("hftree-file"))
   {
      std::string hftreeFile = boost::any_cast<std::string>(fOptionsValues["hftree-file"].value());

      std::cout << "fHftreeFile: " << hftreeFile << std::endl;
      std::ifstream tmpFileCheck(hftreeFile.c_str());
      if (!tmpFileCheck.good()) {
         Fatal("VerifyOptions", "File \"%s\" does not exist", hftreeFile.c_str());
      }
   } else {
      Fatal("VerifyOptions", "Input file not set");
   }


   if (fOptionsValues.count("volume-pattern-flist"))
   {
      std::cout << "fVolumeListFile: " << fVolumeListFile << std::endl;
      std::ifstream volListFile(fVolumeListFile.c_str());

      if (!volListFile.good()) {
         Fatal("VerifyOptions", "File \"%s\" does not exist", fVolumeListFile.c_str());
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
            Fatal("VerifyOptions", "Provided regex \"%s\" is not valid", pattern.c_str());
         }

         if (volListFile.eof()) break;

         fVolumeList.insert(pattern);
      }

      Info("VerifyOptions", "User patterns (fVolumeList) are:");
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
            Fatal("VerifyOptions", "Provided regex \"%s\" is not valid", fVolumePattern.c_str());
         }

         fVolumeList.clear();
         fVolumeList.insert(fVolumePattern);
      }

      Info("VerifyOptions", "User patterns (fVolumeList) are:");
      std::copy(fVolumeList.begin(), fVolumeList.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
   }


   if (fOptionsValues.count("max-events"))
   {
      std::cout << "max-events: " << fMaxEventsUser << std::endl;
   }

   if (fOptionsValues.count("sparsity"))
   {
      if (fSparsity > 1 || fSparsity <= 0) {
         Warning("VerifyOptions", "Sparsity specified value outside allowed limits. Set to 1");
         fSparsity = 1;
      }
      std::cout << "sparsity: " << fSparsity << std::endl;
   }

   if (fOptionsValues.count("save-graph") )
      fSaveGraphics = true;
}


bool PrgOptionProcessor::MatchedVolName(std::string & volName) const
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
   TFile file( fHftreeFile.c_str() );

   if ( file.IsZombie() )
   {
      Warning("BuildInputChains", "Input file is not a root file: %s\nWill treat it as a file list", fHftreeFile.c_str());

      std::ifstream hftreeListFile(fHftreeFile.c_str());

      while ( hftreeListFile.good() )
      {
         std::string hftTreeFileName;
         hftreeListFile >> hftTreeFileName;
         if (hftreeListFile.eof()) break;

         AddToInputChains(hftTreeFileName);
      }
   } else
   {
      Info("BuildInputChains", "Found root file: %s", fHftreeFile.c_str());
      AddToInputChains(fHftreeFile);
   }
}


/*!
 * This private method takes a path to a valid ROOT file as input. No check is
 * done to make sure the input chains exist.
 *
 * XXX: Need to add a check to validate the tree in the file.
 */
void PrgOptionProcessor::AddToInputChains(std::string hftTreeRootFileName)
{
   TFile file( hftTreeRootFileName.c_str() );

   if ( file.IsZombie() )
      Fatal("AddToInputChains", "Input file is not a valid root file: %s", hftTreeRootFileName.c_str());

   fHftChain->AddFile( hftTreeRootFileName.c_str() );
   Info("AddToInputChains", "Found valid ROOT file with HFT tree: %s", hftTreeRootFileName.c_str());
}
