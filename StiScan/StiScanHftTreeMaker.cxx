#include "StiScan/StiScanHftTreeMaker.h"

#include "TBranch.h"

#include "Sti/StiToolkit.h"
#include "Sti/StiTrackContainer.h"
#include "StiMaker/StiMaker.h"

#include "StiScan/StiScanEvent.h"

ClassImp(StiScanHftTreeMaker)


StiScanHftTreeMaker::StiScanHftTreeMaker(const Char_t *name) : StiTreeMaker(name)
{
}


void StiScanHftTreeMaker::SetEventTree()
{
   fEvent = new StiScanEvent();
   TBranch *branch = fTree->Branch("e.", "StiScanEvent", &fEvent, 64000, 99);
   branch->SetAutoDelete(kFALSE);
}


Int_t StiScanHftTreeMaker::Make()
{
   // Fill event with information from Sti tracks
   StiMaker* stiMaker = (StiMaker*) GetMaker("Sti");
   assert(stiMaker);

   StiToolkit *stiToolkit = stiMaker->getToolkit();
   StiTrackContainer *stiTrackContainer = stiToolkit->getTrackContainer();

   return fEvent->Fill(*stiTrackContainer);
}
