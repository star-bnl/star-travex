#include "StiRootIO/TStiHitProxy.h"
#include "StiRootIO/TStiKalmanTrackNode.h"


ClassImp(TStiHitProxy)


TStiHitProxy::TStiHitProxy() :
   TObject(),
   fTStiHit(nullptr),
   fTStiKTNode(nullptr),
   fChi2(-1),
   fDistanceToNode(-1)
{
}


TStiHitProxy::TStiHitProxy(const TStiHit& hit, const TStiKalmanTrackNode& node) :
   TObject(),
   fTStiHit(&hit),
   fTStiKTNode(&node),
   fChi2(node.CalcChi2(hit)),
   fDistanceToNode(node.CalcDistanceToHit(hit))
{
}


bool operator< (const TStiHitProxy& lhs, const TStiHitProxy& rhs)
{
   return (lhs.GetDistanceToNode() < rhs.GetDistanceToNode());
}
