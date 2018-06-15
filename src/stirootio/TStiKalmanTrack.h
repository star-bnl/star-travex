#ifndef TStiKalmanTrack_h
#define TStiKalmanTrack_h

#include <set>

#include "TObject.h"

#include "StEvent/StEnumerations.h"
#include "Sti/StiKalmanTrack.h"
#include "stirootio/TStiKalmanTrackNode.h"

class TStiEvent;


class TStiKalmanTrack : public TObject
{
public:

   TStiKalmanTrack();
   explicit TStiKalmanTrack(TStiEvent* event);
   TStiKalmanTrack(const StiKalmanTrack& stiKTrack, TStiEvent* event=nullptr);

   const TStiEvent* GetParentEvent() const { return fEvent; }
   std::pair<std::set<TStiHit>::iterator, bool> AddToParentEvent(const TStiHit& stiHit);
   const std::set<TStiKalmanTrackNode>& GetNodes() const { return fNodes; }
   void  FindClosestHits(const std::set<TStiHit>& stiHits);
   void  FindCandidateHits(const std::set<TStiHit>& stiHits);
   void  AssignClosestCandidateHit();
   const TStiKalmanTrackNode& GetDcaNode() const;
   double GetEnergyLosses() const;
   virtual void Print(Option_t *opt = "") const;

protected:

   /// Pointer to mother event containing this track
   TStiEvent  *fEvent;

   std::set<TStiKalmanTrackNode> fNodes;

   /// Total track energy lost in all volumes
   double  fEnergyLosses;

   ClassDef(TStiKalmanTrack, 2)
};

#endif
