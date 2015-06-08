#include "StiHify/StiHifyTreeMaker.h"

#include "TBranch.h"

#include "StEvent/StEnumerations.h"
#include "Sti/StiToolkit.h"
#include "Sti/StiHitContainer.h"
#include "Sti/StiTrackContainer.h"
#include "StiMaker/StiMaker.h"

#include "StiHify/StiHifyEvent.h"

ClassImp(StiHifyTreeMaker)


StiHifyTreeMaker::StiHifyTreeMaker(const std::string &name) : StiTreeMaker(name, "stihify")
{
}


void StiHifyTreeMaker::SetEventTree()
{
   // Pass detector Id to event to constrain type of track nodes to save in event
   fEvent = new StiHifyEvent(kIstId, true);
   fTree->Branch("e.", "StiHifyEvent", &fEvent, 64000, 99);
}


Int_t StiHifyTreeMaker::Make()
{
   // Fill event with information from Sti tracks
   StiMaker* stiMaker = (StiMaker*) GetMaker("Sti");
   assert(stiMaker);

   StiToolkit *stiToolkit = stiMaker->getToolkit();

   StiHitContainer *stiHitContainer = stiToolkit->getHitContainer();
   fEvent->Fill(*stiHitContainer);

   StiTrackContainer *stiTrackContainer = stiToolkit->getTrackContainer();
   fEvent->Fill(*stiTrackContainer);

   // Fill other quantities which depend on already filled track and hit containers
   fEvent->PostFill();

   fTree->Fill();

   return kStOk;
}
