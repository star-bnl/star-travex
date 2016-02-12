#include <algorithm>

#include "StiRootIO/StiVolumeFilter.h"
#include "StiHify/StiHifyEvent.h"
#include "Sti/StiKalmanTrack.h"

ClassImp(StiHifyEvent)


StiHifyEvent::StiHifyEvent() : TStiEvent()
{
}


StiHifyEvent::StiHifyEvent(const StiVolumeFilter& stiVolumeFilter) : TStiEvent(stiVolumeFilter)
{
}


/**
 * Fills internal event container with track states/nodes provided by Sti
 * tracking (some requirements on track quality may be applied at this point).
 */
EReturnCodes StiHifyEvent::Fill(const StiTrackContainer &stiTrackContainer)
{
   auto acceptTrack = [] (const StiTrack* stiTrack) -> bool
   {
      const StiKalmanTrack& stiKTrack = static_cast<const StiKalmanTrack&>(*stiTrack);
      return (stiKTrack.getFitPointCount(kTpcId) > 40);
   };

   // First, create an empty container to keep track states provided directly by Sti
   StiTrackContainer filtered(stiTrackContainer.getName(), stiTrackContainer.getDescription());
   filtered.resize(stiTrackContainer.size());

   // Second, copy in the new container those Sti tracks that satisfy certain
   // requirements (currently, the condition is simply hard coded above)
   auto new_end = std::copy_if(stiTrackContainer.begin(), stiTrackContainer.end(), filtered.begin(), acceptTrack);
   // Shrink container to new size
   filtered.resize(std::distance(filtered.begin(), new_end));

   return TStiEvent::Fill(filtered);
}


/**
 * Loops over the track container and calls the track method to override the
 * accepted hits in all nodes of that track.
 */
void StiHifyEvent::AssignClosestCandidateHit()
{
   for (auto& track : fTStiKalmanTracks)
   {
      track.AssignClosestCandidateHit();
   }
}
