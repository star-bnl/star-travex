#include "StiRootIO/TStiEvent.h"
#include "St_base/Stypes.h"

ClassImp(TStiEvent);


/// By default, we set no constraints on tracks w.r.t. their association with
/// a specific detector group
StDetectorId TStiEvent::fgDetGroupId = kMaxDetectorId;
bool TStiEvent::fgDetActiveOnly = false;


TStiEvent::TStiEvent() : TObject(), fTStiKalmanTracks(), fTStiHits()
{
}


/**
 * Constructs event in which track nodes will be constrained by the provided
 * detector Id.
 * */
TStiEvent::TStiEvent(StDetectorId detGroupId, bool detActiveOnly) :
   TObject(), fTStiKalmanTracks(), fTStiHits()
{
   fgDetGroupId = detGroupId;
   fgDetActiveOnly = detActiveOnly;
}


EReturnCodes TStiEvent::Fill(const StiTrackContainer &stiTrackContainer)
{
   for (auto trackIt = stiTrackContainer.begin(); trackIt != stiTrackContainer.end(); ++trackIt)
   {
      StiKalmanTrack* stiKTrack = static_cast<StiKalmanTrack*>(*trackIt);

      if ( !stiKTrack ) {
         Info("Fill", "Invalid kalman kTrack. Skipping to next one...");
         continue;
      }

      if (fgDetGroupId == kMaxDetectorId)
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

            if (stiNodeDetId == fgDetGroupId) {
               fTStiKalmanTracks.push_back( TStiKalmanTrack(this, *stiKTrack) );
               break;
            }
         }
      }
   }

   return kStOK;
}


EReturnCodes TStiEvent::Fill(StiHitContainer &stiHitContainer)
{
   std::vector<StiHit*>& selectedHits = stiHitContainer.getHits(fgDetGroupId);

   for (auto iStiHit = selectedHits.begin(); iStiHit != selectedHits.end(); ++iStiHit)
      InsertStiHit(**iStiHit);

   return kStOK;
}


void TStiEvent::Print(Option_t *opt) const
{
   for (auto iTStiKTrack = fTStiKalmanTracks.begin(); iTStiKTrack != fTStiKalmanTracks.end(); ++iTStiKTrack) {
      iTStiKTrack->Print();
   }
}


void TStiEvent::Clear(Option_t *opt)
{
   fTStiKalmanTracks.clear();
   fTStiHits.clear();
}
