#ifndef TStiKalmanTrack_h
#define TStiKalmanTrack_h

#include <string>
#include <set>

#include "TObject.h"
#include "Sti/StiKalmanTrack.h"
#include "StiScan/TStiKalmanTrackNode.h"

class StiScanEvent;


class TStiKalmanTrack : public TObject
{
public:

   TStiKalmanTrack();
   TStiKalmanTrack(StiScanEvent* event, const StiKalmanTrack & stiKTrack);
   const StiScanEvent* GetParentEvent() const { return fEvent; }
   const std::set<TStiKalmanTrackNode>& GetNodes() const;
   const TStiKalmanTrackNode& GetDcaNode() const;
   double GetEnergyLosses() const;
   virtual void Print(Option_t *opt = "") const;

protected:

   StiScanEvent  *fEvent;   //!< Pointer to mother event containing this track
   std::set<TStiKalmanTrackNode> fNodes;
   double   fEnergyLosses;      ///< Total track energy lost in all volumes

   ClassDef(TStiKalmanTrack, 2)
};

#endif
