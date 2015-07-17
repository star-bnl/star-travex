#include "StiRootIO/TStiEvent.h"

#include "StiRootIO/StiVolumeFilter.h"
#include "St_base/Stypes.h"
#include "Sti/StiTrack.h"
#include "Sti/StiKalmanTrack.h"

ClassImp(TStiEvent);


TStiEvent::TStiEvent() : TObject(),
   fStiVolumeFilter(StiVolumeFilter::GetInstance()),
   fTStiKalmanTracks(),
   fTStiHits()
{
}


TStiEvent::TStiEvent(const StiVolumeFilter& stiVolumeFilter) :
   fStiVolumeFilter(stiVolumeFilter),
   fTStiKalmanTracks(),
   fTStiHits()
{
}


EReturnCodes TStiEvent::Fill(const StiTrackContainer &stiTrackContainer)
{
   for (const StiTrack* stiTrack : stiTrackContainer)
   {
      const StiKalmanTrack& stiKTrack = static_cast<const StiKalmanTrack&>(*stiTrack);

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
   std::cout << "TStiEvent::Print(" << *opt << ")" << "\n"
             << "Num. of tracks: " << fTStiKalmanTracks.size() << "\n"
             << "Num. of hits:   " << fTStiHits.size()
             << std::endl;

   if (string(opt).find("all") == std::string::npos)
      return;

   for (auto iTStiKTrack = fTStiKalmanTracks.begin(); iTStiKTrack != fTStiKalmanTracks.end(); ++iTStiKTrack) {
      iTStiKTrack->Print();
   }
}


void TStiEvent::Clear(Option_t *opt)
{
   fTStiKalmanTracks.clear();
   fTStiHits.clear();
}
