#include <iostream>
#include <fstream>

#include "TFile.h"

#include "travex/utils.h"
#include "StiScan/StiScanPrgOptions.h"


StiScanPrgOptions::StiScanPrgOptions() : StiScanPrgOptions(0, nullptr) { }


StiScanPrgOptions::StiScanPrgOptions(int argc, char **argv, const std::string& stiTreeName, const std::string& geantStepTreeName) :
   PrgOptionProcessor(argc, argv, stiTreeName), fDoGeantStepTree(false), fDoAutoHistRange(true),
   fGeantStepChain(new TChain(geantStepTreeName.c_str(), "READ")),
   fHistZMin(0), fHistZMax(0),
   fHistRMin(0), fHistRMax(0)
{
   fOptions.add_options()
      ("geant-step-tree,t",   "In addition to Sti tree process tree with info from geant steps")
      ("z-min", po::value<double>(&fHistZMin)->default_value(fHistZMin), "If provided the minimum limit along z will be overwritten by this value in the output histograms")
      ("z-max", po::value<double>(&fHistZMax)->default_value(fHistZMax), "If provided the maximum limit along z will be overwritten by this value in the output histograms")
      ("r-min", po::value<double>(&fHistRMin)->default_value(fHistRMin), "If provided the minimum limit in radial direction will be overwritten by this value in the output histograms")
      ("r-max", po::value<double>(&fHistRMax)->default_value(fHistRMax), "If provided the maximum limit in radial direction will be overwritten by this value in the output histograms")
   ;
}


StiScanPrgOptions::~StiScanPrgOptions()
{
   delete fGeantStepChain; fGeantStepChain = nullptr;
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
void StiScanPrgOptions::AddToInputChains(std::string stiTreeRootFileName)
{
   PrgOptionProcessor::AddToInputChains(stiTreeRootFileName);

   if (!fDoGeantStepTree) return;

   TString geantStepRootFileName(stiTreeRootFileName.c_str());
   geantStepRootFileName.ReplaceAll("stiscan.root", "track_history.root");

   TFile file( geantStepRootFileName.Data() );

   if ( file.IsZombie() )
      TVX_FATAL("Input file is not a valid root file: %s", geantStepRootFileName.Data());

   fGeantStepChain->AddFile( geantStepRootFileName.Data() );
   TVX_INFO("Found valid ROOT file with Geant info: %s", geantStepRootFileName.Data());
}
