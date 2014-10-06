#ifndef TStiKalmanTrack_h
#define TStiKalmanTrack_h

#include <string>
#include <set>

#include "TObject.h"
#include "Sti/StiKalmanTrack.h"
#include "StHftPool/EventT/TStiKalmanTrackNode.h"


class TStiKalmanTrack : public TObject
{
public:

   TStiKalmanTrack();
   TStiKalmanTrack(const StiKalmanTrack & stiKTrack);
   const std::set<TStiKalmanTrackNode>& GetNodes() const;
   const TStiKalmanTrackNode& GetDcaNode() const;
   double GetEnergyLosses() const;
   virtual void Print(Option_t *opt = "") const;

protected:

   std::set<TStiKalmanTrackNode> fNodes;
   double   fEnergyLosses;      ///< Total track energy lost in all volumes

   ClassDef(TStiKalmanTrack, 1)
};

#endif
