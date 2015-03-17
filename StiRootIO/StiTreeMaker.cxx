#include "StiRootIO/StiTreeMaker.h"

#include "TBranch.h"
#include "TSystem.h"

#include "StBFChain/StBFChain.h"
#include "StEvent/StEnumerations.h"
#include "StEvent/StEnumerations.h"

ClassImp(StiTreeMaker)


StiTreeMaker::StiTreeMaker(const Char_t *name) : StMaker(name),
   fFile(nullptr),
   fTree(new TTree("t", "TTree with HFT hits and tracks")),
   fEvent(nullptr)
{
}


StiTreeMaker::~StiTreeMaker()
{
   if (fFile) fFile->Close();

   delete fFile; fFile = nullptr;
   delete fTree; fTree = nullptr;
   delete fEvent; fEvent = nullptr;
}


Int_t StiTreeMaker::Init()
{
   StBFChain *bfChain = (StBFChain *) StMaker::GetChain();

   if (!bfChain) return kStFatal;

   // Authorize Trees up to 2 Terabytes (if the system can do it)
   TTree::SetMaxTreeSize(1000 * Long64_t(2000000000));

   TString fileName( gSystem->BaseName(bfChain->GetFileIn().Data()) );

   fileName.ReplaceAll(".event", "");
   fileName.ReplaceAll(".daq", "");
   fileName.ReplaceAll(".fz", "");

   fileName += ".hftree.root";

   fFile = new TFile(fileName, "RECREATE", "TTree with HFT hits and tracks");
   fFile->SetCompressionLevel(1); // Set file compression level

   SetEventTree();

   fTree->SetDirectory(fFile);

   return kStOK;
}


Int_t StiTreeMaker::Finish()
{
   fFile = fTree->GetCurrentFile(); //just in case we switched to a new file
   fFile->Write();

   if (GetDebug() >= 1)
      fTree->Print();

   return kStOK;
}


/** Clear the container for the next event. */
void StiTreeMaker::Clear(Option_t *opt)
{
   fEvent->Clear();
}
