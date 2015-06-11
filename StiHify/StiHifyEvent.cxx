#include <algorithm>

#include "StiHify/StiHifyEvent.h"
#include "Sti/StiKalmanTrack.h"

ClassImp(StiHifyEvent);


StiHifyEvent::StiHifyEvent() : TStiEvent()
{
}


/**
 * Constructs event in which track nodes will be constrained by the provided
 * detector Id.
 */
StiHifyEvent::StiHifyEvent(StDetectorId detGroupId, bool detActiveOnly) : TStiEvent(detGroupId, detActiveOnly)
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
   filtered.reserve(stiTrackContainer.size());

   std::copy_if(stiTrackContainer.begin(), stiTrackContainer.end(), filtered.begin(), acceptTrack);

   return TStiEvent::Fill(filtered);
}
