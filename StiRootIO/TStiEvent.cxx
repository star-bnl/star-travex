#include "StiRootIO/TStiEvent.h"
#include "StiRootIO/StiHitFilters.h"
#include "StiRootIO/StiVolumeFilter.h"
#include "St_base/Stypes.h"
#include "Sti/StiHit.h"
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


bool TStiEvent::AcceptTrack(const StiKalmanTrack& track) const
{
   return fStiVolumeFilter.AcceptTrack(track);
}


bool TStiEvent::AcceptTrackNode(const StiKalmanTrackNode& node) const
{
   return fStiVolumeFilter.AcceptTrackNode(node);
}


EReturnCodes TStiEvent::Fill(const StiTrackContainer &stiTrackContainer)
{
   for (const StiTrack* stiTrack : stiTrackContainer)
   {
      const StiKalmanTrack& stiKTrack = static_cast<const StiKalmanTrack&>(*stiTrack);

      if ( AcceptTrack(stiKTrack) ) {
         fTStiKalmanTracks.push_back( TStiKalmanTrack(stiKTrack, this) );
      }
   }

   return kStOK;
}


EReturnCodes TStiEvent::Fill(StiHitContainer &stiHitContainer)
{
   StiHitFilterByVolumeName filter(fStiVolumeFilter.GetNamePatterns());
   std::vector<StiHit*>& selectedHits = stiHitContainer.getHits(filter);

   std::transform(selectedHits.begin(), selectedHits.end(),
      std::inserter(fTStiHits, fTStiHits.begin()), [](const StiHit* hit) {return TStiHit(*hit);} );

   return kStOK;
}


EReturnCodes TStiEvent::PostFill()
{
   for (auto& track : fTStiKalmanTracks)
   {
      // Find the closest hit among the all known (to be saved) hits in this event
      track.AssignClosestHits(fTStiHits);
      track.FindCandidateHits(fTStiHits);
   }

   return kStOk;
}


void TStiEvent::Print(Option_t *opt) const
{
   std::cout << "TStiEvent::Print(" << std::string(opt) << ")" << "\n"
             << "Num. of tracks: " << fTStiKalmanTracks.size() << "\n"
             << "Num. of hits:   " << fTStiHits.size()
             << std::endl;

   if (string(opt).find("all") == std::string::npos)
      return;

   for (const auto& track : fTStiKalmanTracks) {
      track.Print();
   }
}


void TStiEvent::Clear(Option_t *opt)
{
   fTStiKalmanTracks.clear();
   fTStiHits.clear();
}
