#include "StiScan/StiScanEvent.h"
#include "St_base/Stypes.h"

ClassImp(StiScanEvent);


StiScanEvent::StiScanEvent() : EventT(), fTStiKalmanTracks()
{
}


StiScanEvent::~StiScanEvent()
{
   fTStiKalmanTracks.clear();
}


Int_t StiScanEvent::Fill(StiTrackContainer &stiTrackContainer)
{
   vector<StiTrack*>::iterator trackIt = stiTrackContainer.begin();

   for ( ; trackIt != stiTrackContainer.end(); ++trackIt)
   {
      StiKalmanTrack* kalmanTrack = static_cast<StiKalmanTrack*>(*trackIt);

      if ( !kalmanTrack ) {
         Info("Fill", "Invalid kalman kTrack. Skipping to next one...");
         continue;
      }

      fTStiKalmanTracks.push_back( TStiKalmanTrack(*kalmanTrack) );
   }

   return kStOK;
}


void StiScanEvent::Print(Option_t *opt) const
{
   EventT::Print(opt);

   std::vector<TStiKalmanTrack>::const_iterator iTStiKTrack = fTStiKalmanTracks.begin();

   for ( ; iTStiKTrack != fTStiKalmanTracks.end(); ++iTStiKTrack) {
      iTStiKTrack->Print();
   }
}
