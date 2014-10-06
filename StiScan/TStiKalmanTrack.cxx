
#include "StHftPool/EventT/TStiKalmanTrack.h"

#include "Sti/StiKalmanTrackNode.h"


ClassImp(TStiKalmanTrack)


TStiKalmanTrack::TStiKalmanTrack() : TObject(),
   fNodes(), fEnergyLosses(0)
{
}


TStiKalmanTrack::TStiKalmanTrack(const StiKalmanTrack & stiKTrack) : TObject(),
   fNodes(), fEnergyLosses(0)
{
   // Loop over track nodes
   for (StiKTNIterator it = stiKTrack.begin(); it != stiKTrack.end(); ++it)
   {
      StiKalmanTrackNode *stiNode = &(*it);

      if ( !stiNode ) {
         Warning("TStiKalmanTrack", "Invalid kalman node. Skipping to next one...");
         continue;
      }

      fNodes.insert( TStiKalmanTrackNode(*stiNode) );
      fEnergyLosses += stiNode->getEnergyLosses();
   }
}


const std::set<TStiKalmanTrackNode>& TStiKalmanTrack::GetNodes() const { return fNodes; }

const TStiKalmanTrackNode& TStiKalmanTrack::GetDcaNode() const { return *fNodes.begin(); }

double TStiKalmanTrack::GetEnergyLosses() const { return fEnergyLosses; }


void TStiKalmanTrack::Print(Option_t *opt) const
{
   printf("fEnergyLosses: %f\n", fEnergyLosses);

   std::set<TStiKalmanTrackNode>::const_iterator iTStiKTrackNode = fNodes.begin();

   for (int nodeIdx=0; iTStiKTrackNode != fNodes.end(); ++iTStiKTrackNode, ++nodeIdx) {
      printf("node index: %d\n", nodeIdx);
      iTStiKTrackNode->Print();
   }
}
