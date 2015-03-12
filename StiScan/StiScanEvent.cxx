#include "StiScan/StiScanEvent.h"
#include "St_base/Stypes.h"

ClassImp(StiScanEvent);


/// By default, we set no constraints on tracks w.r.t. their association with
/// a specific detector group
StDetectorId StiScanEvent::fDetGroupId = kMaxDetectorId;


StiScanEvent::StiScanEvent() : EventT(), fTStiKalmanTracks(), fTStiHits()
{
}


StiScanEvent::StiScanEvent(StDetectorId detGroupId) : EventT(),
   fTStiKalmanTracks(), fTStiHits()
{
   fDetGroupId = detGroupId;
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

      if (fDetGroupId == kMaxDetectorId)
      {
         // All tracks regardless of detector are accepted
         fTStiKalmanTracks.push_back( TStiKalmanTrack(this, *stiKTrack) );
         continue;

      } else {
         // Accept only tracks having a node associated with the given detector
         for (StiKTNIterator it = stiKTrack->begin(); it != stiKTrack->end(); ++it)
         {
            StiKalmanTrackNode *stiNode = &(*it);
            if ( !stiNode ) continue;

            StDetectorId stiNodeDetId = stiNode->getDetector() ?
               static_cast<StDetectorId>( stiNode->getDetector()->getGroupId() ) : kUnknownId;

            if (stiNodeDetId == fDetGroupId) {
               fTStiKalmanTracks.push_back( TStiKalmanTrack(this, *stiKTrack) );
               break;
            }
         }
      }
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


void StiScanEvent::Clear(Option_t *opt)
{
   EventT::Clear(opt);

   fTStiKalmanTracks.clear();
   fTStiHits.clear();
}
