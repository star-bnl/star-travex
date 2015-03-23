#ifndef TStiKalmanTrack_h
#define TStiKalmanTrack_h

#include <set>

#include "TObject.h"

#include "StEvent/StEnumerations.h"
#include "Sti/StiKalmanTrack.h"
#include "StiRootIO/TStiKalmanTrackNode.h"

class TStiEvent;


class TStiKalmanTrack : public TObject
{
public:

   TStiKalmanTrack();
   TStiKalmanTrack(TStiEvent* event, const StiKalmanTrack& stiKTrack);
   const TStiEvent* GetParentEvent() const { return fEvent; }
   std::pair<std::set<TStiHit>::iterator, bool> AddToParentEvent(const TStiHit& stiHit);
   const std::set<TStiKalmanTrackNode>& GetNodes() const { return fNodes; }
   void  AssignClosestHits(const std::set<TStiHit>& stiHits);
   const TStiKalmanTrackNode& GetDcaNode() const;
   double GetEnergyLosses() const;
   virtual void Print(Option_t *opt = "") const;

protected:

   TStiEvent  *fEvent;   //!< Pointer to mother event containing this track
   std::set<TStiKalmanTrackNode> fNodes;
   double   fEnergyLosses;      ///< Total track energy lost in all volumes

   ClassDef(TStiKalmanTrack, 2)
};

#endif
