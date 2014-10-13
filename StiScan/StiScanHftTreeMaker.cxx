#include <stdlib.h>

#include "StiScan/StiScanHftTreeMaker.h"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "StEvent.h"
#include "StBFChain.h"

#include "Sti/StiToolkit.h"
#include "Sti/StiTrackContainer.h"
#include "Sti/StiKalmanTrack.h"
#include "StiMaker/StiMaker.h"

#include "StiScan/StiScanEvent.h"
#include "StIstDbMaker/StIstDb.h"
#include "StPxlDbMaker/StPxlDb.h"

ClassImp(StiScanHftTreeMaker)


StiScanHftTreeMaker::StiScanHftTreeMaker(const Char_t *name) : HftMatchedTree(name)
{
}


void StiScanHftTreeMaker::SetTree()
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

   ((StiScanEvent*) fEvent)->Fill(*stiTrackContainer);

   // Fill the rest of event with information from StEvent
   return HftMatchedTree::Make();
}


void StiScanHftTreeMaker::Clear(Option_t *opt)
{
   HftMatchedTree::Clear(opt);
}
