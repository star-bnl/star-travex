#include "StarEventHistContainer.h"


StarEventHistContainer::StarEventHistContainer(const std::string name, TDirectory* motherDir, const std::string option) :
   tvx::HistContainer(name, motherDir, option)
{
   Add(new TH1I("hNumVertices", "; Number of Vertices; Counts; ", 10, 0, 10));
   Add(new TH1I("hNumTracksPerEvent", "; Number of Tracks per Event; Counts", 100, 0, 100));
   Add(new TH1I("hNumTracksPerEventPrimary", "; Number of Primary Tracks per Event; Counts", 100, 0, 100));
}


void StarEventHistContainer::FillHists(const StMuDst &event)
{
   fHs["hNumVertices"]->Fill( event.primaryVertices()->GetEntriesFast() );
   fHs["hNumTracksPerEvent"]->Fill( event.globalTracks()->GetEntriesFast() );
   fHs["hNumTracksPerEventPrimary"]->Fill( event.primaryTracks()->GetEntriesFast() );
}
