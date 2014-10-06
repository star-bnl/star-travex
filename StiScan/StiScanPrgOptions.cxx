#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/regex.hpp>

#include "TFile.h"
#include "TSystem.h"

#include "StHftPool/EventT/StiScanPrgOptions.h"


StiScanPrgOptions::StiScanPrgOptions() : PrgOptionProcessor(), fDoGeantStepTree(false), fDoAutoHistRange(true),
   fGeantStepChain(nullptr),
   fOutPrefix("./"),
   fHistZMin(0), fHistZMax(0),
   fHistRMin(0), fHistRMax(0)
{
   InitOptions();
}


StiScanPrgOptions::StiScanPrgOptions(int argc, char **argv, const std::string& hftTreeName, const std::string& geantStepTreeName) :
   PrgOptionProcessor(argc, argv, hftTreeName), fDoGeantStepTree(false), fDoAutoHistRange(true),
   fGeantStepChain(new TChain(geantStepTreeName.c_str(), "READ")),
   fOutPrefix("./"),
   fHistZMin(0), fHistZMax(0),
   fHistRMin(0), fHistRMax(0)
{
   InitOptions();
}


StiScanPrgOptions::~StiScanPrgOptions()
{
   delete fGeantStepChain; fGeantStepChain = nullptr;
}


/**
 * Declare supported options.
 */
void StiScanPrgOptions::InitOptions()
{
   fOptions.add_options()
      ("geant-step-tree,t",   "In addition to 'hftree' process tree with info from geant steps")
      ("out-prefix,o", po::value<std::string>(&fOutPrefix), "Absolute or relative path prefix for the output files")
      ("z-min", po::value<double>(&fHistZMin)->default_value(fHistZMin), "If provided the minimum limit along z will be overwritten by this value in the output histograms")
      ("z-max", po::value<double>(&fHistZMax)->default_value(fHistZMax), "If provided the maximum limit along z will be overwritten by this value in the output histograms")
      ("r-min", po::value<double>(&fHistRMin)->default_value(fHistRMin), "If provided the minimum limit in radial direction will be overwritten by this value in the output histograms")
      ("r-max", po::value<double>(&fHistRMax)->default_value(fHistRMax), "If provided the maximum limit in radial direction will be overwritten by this value in the output histograms")
   ;
}


void StiScanPrgOptions::VerifyOptions()
{
   PrgOptionProcessor::VerifyOptions();

   if (fOptionsValues.count("geant-step-tree") )
      fDoGeantStepTree = true;

   if (fHistZMin != fHistZMax || fHistRMin != fHistRMax)
      fDoAutoHistRange = false;
}


/*!
 * This private method takes a path to a valid ROOT file as input. No check is
 * done to make sure the input chains exist.
 *
 * XXX: Need to add a check to validate the tree in the file.
 */
void StiScanPrgOptions::AddToInputChains(std::string hftTreeRootFileName)
{
   PrgOptionProcessor::AddToInputChains(hftTreeRootFileName);

   if (!fDoGeantStepTree) return;

   TString geantStepRootFileName(hftTreeRootFileName.c_str());
   geantStepRootFileName.ReplaceAll("hftree.root", "track_history.root");

   TFile file( geantStepRootFileName.Data() );

   if ( file.IsZombie() )
      Fatal("AddToInputChains", "Input file is not a valid root file: %s", geantStepRootFileName.Data());

   fGeantStepChain->AddFile( geantStepRootFileName.Data() );
   Info("AddToInputChains", "Found valid ROOT file with Geant info: %s", geantStepRootFileName.Data());
}
