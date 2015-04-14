
#include "StiRootIO/TStiKalmanTrack.h"

#include "StEvent/StEnumerations.h"
#include "Sti/StiKalmanTrackNode.h"
#include "StiRootIO/TStiEvent.h"


ClassImp(TStiKalmanTrack)


TStiKalmanTrack::TStiKalmanTrack() : TObject(),
   fEvent(nullptr), fNodes(), fEnergyLosses(0)
{
}


TStiKalmanTrack::TStiKalmanTrack(TStiEvent* event, const StiKalmanTrack& stiKTrack) :
   TObject(),
   fEvent(event), fNodes(), fEnergyLosses(0)
{
   // Loop over track nodes
   for (StiKTNIterator it = stiKTrack.begin(); it != stiKTrack.end(); ++it)
   {
      StiKalmanTrackNode *stiNode = &(*it);

      if ( !stiNode ) {
         Warning("TStiKalmanTrack", "Invalid kalman node. Skipping to next one...");
         continue;
      }

      // Silently skip DCA nodes
      if ( stiNode->isDca() )
         continue;

      const StiDetector* stiKTNDet = stiNode->getDetector();

      if (!stiKTNDet) {
         Warning("TStiKalmanTrack", "No detector found associated with non-DCA node. Skipping to next one...");
         continue;
      }

      StDetectorId stiNodeDetId = stiKTNDet ?
         static_cast<StDetectorId>( stiKTNDet->getGroupId() ) : kUnknownId;

      if ( ( TStiEvent::fgDetGroupId == stiNodeDetId || TStiEvent::fgDetGroupId == kMaxDetectorId ) &&
           ( (TStiEvent::fgDetActiveOnly && stiKTNDet->isActive()) || !TStiEvent::fgDetActiveOnly )
         )
      {
         fNodes.insert( TStiKalmanTrackNode(this, *stiNode) );
      }

      fEnergyLosses += stiNode->getEnergyLosses();
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

   std::set<TStiKalmanTrackNode>::const_iterator iTStiKTrackNode = fNodes.begin();

   for (int nodeIdx=0; iTStiKTrackNode != fNodes.end(); ++iTStiKTrackNode, ++nodeIdx) {
      Info("Print", "node index: %d\n", nodeIdx);
      iTStiKTrackNode->Print();
   }
}


/**
 * For each node of this track finds the hit closest to the mean track
 * projection.
 */
void TStiKalmanTrack::AssignClosestHits(const std::set<TStiHit>& stiHits)
{
   for (auto iNode = fNodes.begin(); iNode != fNodes.end(); ++iNode)
   {
      // The following cast is ugly but we want to update a member which is not
      // used for ordering of the set elements which is a safe thing to do
      TStiKalmanTrackNode& tmpNode = const_cast< TStiKalmanTrackNode& >(*iNode);
      tmpNode.AssignClosestHit(stiHits);
   }
}


/**
 * For each node of this track finds hits in some proximity to the mean track
 * projection.
 */
void TStiKalmanTrack::FindAdjacentHits(const std::set<TStiHit>& stiHits)
{
   for (auto iNode = fNodes.begin(); iNode != fNodes.end(); ++iNode)
   {
      // The following cast is ugly but we want to update a member which is not
      // used for ordering of the set elements which is a safe thing to do
      TStiKalmanTrackNode& tmpNode = const_cast< TStiKalmanTrackNode& >(*iNode);
      tmpNode.FindAdjacentHits(stiHits);
   }
}
