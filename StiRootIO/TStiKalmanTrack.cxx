
#include "StiRootIO/TStiKalmanTrack.h"

#include "StEvent/StEnumerations.h"
#include "Sti/StiKalmanTrackNode.h"
#include "StiRootIO/TStiEvent.h"


ClassImp(TStiKalmanTrack)


TStiKalmanTrack::TStiKalmanTrack() : TObject(),
   fEvent(nullptr), fNodes(), fEnergyLosses(0)
{
}


TStiKalmanTrack::TStiKalmanTrack(TStiEvent* event) : TObject(),
   fEvent(event), fNodes(), fEnergyLosses(0)
{
}


TStiKalmanTrack::TStiKalmanTrack(const StiKalmanTrack& stiKTrack, TStiEvent* event) :
   TStiKalmanTrack(event)
{
   // Loop over track nodes
   for (const StiKalmanTrackNode& stiNode : stiKTrack)
   {
      // Silently skip DCA nodes
      if ( stiNode.isDca() )
         continue;

      const StiDetector* stiKTNDet = stiNode.getDetector();

      if (!stiKTNDet) {
         Warning("TStiKalmanTrack", "No detector found associated with non-DCA node. Skipping to next one...");
         continue;
      }

      StDetectorId stiNodeDetId = static_cast<StDetectorId>( stiKTNDet->getGroupId() );

      if ( ( TStiEvent::fgDetGroupId == stiNodeDetId || TStiEvent::fgDetGroupId == kMaxDetectorId ) &&
           ( (TStiEvent::fgDetActiveOnly && stiKTNDet->isActive()) || !TStiEvent::fgDetActiveOnly )
         )
      {
         fNodes.insert( TStiKalmanTrackNode(stiNode, this) );
      }

      fEnergyLosses += stiNode.getEnergyLosses();
   }
}


std::pair<std::set<TStiHit>::iterator, bool> TStiKalmanTrack::AddToParentEvent(const TStiHit& stiHit)
{
   std::pair<std::set<TStiHit>::iterator, bool> dummy;
   dummy.second = false;

   return fEvent ? fEvent->InsertStiHit(stiHit) : dummy;
}


const TStiKalmanTrackNode& TStiKalmanTrack::GetDcaNode() const { return *fNodes.begin(); }

double TStiKalmanTrack::GetEnergyLosses() const { return fEnergyLosses; }


void TStiKalmanTrack::Print(Option_t *opt) const
{
   Info("Print", "fEnergyLosses: %f\n", fEnergyLosses);

   int nodeIdx = 0;

   for (const TStiKalmanTrackNode& iNode : fNodes)
   {
      Info("Print", "node index: %d\n", nodeIdx);
      iNode.Print();
      nodeIdx++;
   }
}


/**
 * For each node of this track finds the hit closest to the mean track
 * projection.
 */
void TStiKalmanTrack::AssignClosestHits(const std::set<TStiHit>& stiHits)
{
   for (const TStiKalmanTrackNode& iNode : fNodes)
   {
      // The following cast is ugly but we want to update a member which is not
      // used for ordering of the set elements which is a safe thing to do
      TStiKalmanTrackNode& tmpNode = const_cast< TStiKalmanTrackNode& >(iNode);
      tmpNode.AssignClosestHit(stiHits);
   }
}


/**
 * For each node of this track finds hits in some proximity to the mean track
 * projection.
 */
void TStiKalmanTrack::FindAdjacentHits(const std::set<TStiHit>& stiHits)
{
   for (const TStiKalmanTrackNode& iNode : fNodes)
   {
      // The following cast is ugly but we want to update a member which is not
      // used for ordering of the set elements which is a safe thing to do
      TStiKalmanTrackNode& tmpNode = const_cast< TStiKalmanTrackNode& >(iNode);
      tmpNode.FindAdjacentHits(stiHits);
   }
}
