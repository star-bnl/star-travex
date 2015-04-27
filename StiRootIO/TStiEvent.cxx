#include "StiRootIO/TStiEvent.h"

#include "St_base/Stypes.h"
#include "Sti/StiTrack.h"
#include "Sti/StiKalmanTrack.h"

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
 */
TStiEvent::TStiEvent(StDetectorId detGroupId, bool detActiveOnly) :
   TStiEvent()
{
   fgDetGroupId = detGroupId;
   fgDetActiveOnly = detActiveOnly;
}


EReturnCodes TStiEvent::Fill(const StiTrackContainer &stiTrackContainer)
{
   for (const StiTrack* stiTrack : stiTrackContainer)
   {
      const StiKalmanTrack& stiKTrack = static_cast<const StiKalmanTrack&>(*stiTrack);

      // This is where we'd like to apply the general QA on selected tracks
      if (stiKTrack.getFitPointCount(kTpcId) > 30)
         continue;

      if (fgDetGroupId == kMaxDetectorId)
      {
         // All tracks regardless of detector are accepted
         fTStiKalmanTracks.push_back( TStiKalmanTrack(stiKTrack, this) );
         continue;

      } else {
         // Accept only tracks having a node associated with the given detector
         for (const StiKalmanTrackNode& stiNode : stiKTrack)
         {
            StDetectorId stiNodeDetId = stiNode.getDetector() ?
               static_cast<StDetectorId>( stiNode.getDetector()->getGroupId() ) : kUnknownId;

            if (stiNodeDetId == fgDetGroupId) {
               fTStiKalmanTracks.push_back( TStiKalmanTrack(stiKTrack, this) );
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


EReturnCodes TStiEvent::PostFill()
{
   for (auto iTrack = fTStiKalmanTracks.begin(); iTrack != fTStiKalmanTracks.end(); ++iTrack)
   {
      iTrack->AssignClosestHits(fTStiHits);
      iTrack->FindAdjacentHits(fTStiHits);
   }

   return kStOk;
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
