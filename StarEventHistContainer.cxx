#include "StarEventHistContainer.h"


StarEventHistContainer::StarEventHistContainer(const std::string name, TDirectory* motherDir, const std::string option) :
   tvx::HistContainer(name, motherDir, option)
{
   TH1* h;

   h = new TH1I("hNumVertices", "Number of Vertices", 500, 0, 500);
   fHs["hNumVertices"].reset(h);

   h = new TH1I("hNumTracksPerEvent", "Number of Tracks per Event", 100, 0, 5000);
   fHs["hNumTracksPerEvent"].reset(h);

   h = new TH1I("hNumTracksPerEventPrimary", "Number of Primary Tracks per Event", 100, 0, 100);
   fHs["hNumTracksPerEventPrimary"].reset(h);
}


void StarEventHistContainer::FillHists(const StMuDst &event)
{
   fHs["hNumVertices"]->Fill( event.primaryVertices()->GetEntriesFast() );
   fHs["hNumTracksPerEvent"]->Fill( event.globalTracks()->GetEntriesFast() );
   fHs["hNumTracksPerEventPrimary"]->Fill( event.primaryTracks()->GetEntriesFast() );
}
