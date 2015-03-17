#ifndef TStiEvent_h
#define TStiEvent_h

#include <vector>
#include <set>
#include <utility>

#include "TObject.h"

#include "St_base/Stypes.h"
#include "StEvent/StEnumerations.h"
#include "Sti/StiTrackContainer.h"
#include "Sti/StiHitContainer.h"
#include "StiRootIO/TStiKalmanTrack.h"
#include "StiRootIO/TStiHit.h"


class TStiEvent : public TObject
{
public:

   TStiEvent();
   TStiEvent(StDetectorId detGroupId);

   virtual EReturnCodes  Fill(const StiTrackContainer &stiTrackContainer);
   virtual EReturnCodes  Fill(StiHitContainer &stiHitContainer);
   std::pair<std::set<TStiHit>::iterator, bool>   InsertStiHit(const TStiHit &stiHit) { return fTStiHits.insert(stiHit); }
   const std::set<TStiHit>& GetStiHits() const { return fTStiHits; }
   virtual void  Clear(Option_t *opt = "");
   virtual void  Print(Option_t *opt = "") const;
   const std::vector<TStiKalmanTrack>& GetTStiKalmanTracks() const { return fTStiKalmanTracks; }

protected:

   static StDetectorId           fgDetGroupId;        //!< Detector group id used in this study
   std::vector<TStiKalmanTrack>  fTStiKalmanTracks;   ///< A collection of all Sti tracks of interest in this event
   std::set<TStiHit>             fTStiHits;           ///< A collection of all Sti hits of interest in this event

   ClassDef(TStiEvent, 1)
};

#endif
