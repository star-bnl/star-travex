#include <algorithm>

#include "StiRootIO/StiVolumeFilter.h"
#include "StiHify/StiHifyEvent.h"
#include "Sti/StiKalmanTrack.h"

ClassImp(StiHifyEvent);


StiHifyEvent::StiHifyEvent() : TStiEvent(),
   fStiVolumeFilter(StiVolumeFilter::GetInstance())
{
}


StiHifyEvent::StiHifyEvent(const StiVolumeFilter& stiVolumeFilter) : TStiEvent(),
   fStiVolumeFilter(stiVolumeFilter)
{
}


/**
 * Constructs event in which track nodes will be constrained by the provided
 * detector Id.
 */
StiHifyEvent::StiHifyEvent(StDetectorId detGroupId, bool detActiveOnly) :
   TStiEvent(detGroupId, detActiveOnly),
   fStiVolumeFilter(StiVolumeFilter::GetInstance())
{
}


EReturnCodes StiHifyEvent::Fill(const StiTrackContainer &stiTrackContainer)
{
   auto acceptTrack = [] (const StiTrack* stiTrack) -> bool
   {
      const StiKalmanTrack& stiKTrack = static_cast<const StiKalmanTrack&>(*stiTrack);
      return (stiKTrack.getFitPointCount(kTpcId) > 30);
   };

   StiTrackContainer filtered(stiTrackContainer.getName(), stiTrackContainer.getDescription());
   filtered.resize(stiTrackContainer.size());

   auto new_end = std::copy_if(stiTrackContainer.begin(), stiTrackContainer.end(), filtered.begin(), acceptTrack);
   // Shrink container to new size
   filtered.resize(std::distance(filtered.begin(), new_end));

   return TStiEvent::Fill(filtered);
}
