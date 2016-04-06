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
      if (fEvent && fEvent->AcceptTrackNode(stiNode) ) {
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
 * projection, i.e. the nominal position of the track node.
 */
void TStiKalmanTrack::FindClosestHits(const std::set<TStiHit>& stiHits)
{
   for (const auto& node : fNodes)
   {
      node.FindClosestHit(stiHits);
   }
}


/**
 * For each node of this track finds hits in some proximity to the mean track
 * projection.
 */
void TStiKalmanTrack::FindCandidateHits(const std::set<TStiHit>& stiHits)
{
   for (const auto& node : fNodes)
   {
      node.FindCandidateHits(stiHits);
   }
}


void TStiKalmanTrack::AssignClosestCandidateHit()
{
   for (const auto& node : fNodes)
   {
      node.AssignClosestCandidateHit();
   }
}
