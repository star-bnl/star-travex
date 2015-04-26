#ifndef TStiHitProxy_h
#define TStiHitProxy_h

#include "TObject.h"

#include "StiRootIO/TStiHit.h"

class TStiKalmanTrackNode;


class TStiHitProxy : public TObject
{
public:

   TStiHitProxy();
   TStiHitProxy(const TStiHit& hit, const TStiKalmanTrackNode& node);

   static const TStiHit* GetBareStiHit(const TStiHitProxy& hitProxy) { return hitProxy.fTStiHit; }
   friend bool operator< (const TStiHitProxy& lhs, const TStiHitProxy& rhs);

private:

   const TStiHit* fTStiHit;   ///< Pointer to the hit in question
   const TStiKalmanTrackNode* fTStiKTNode;   //!< Pointer to Sti node
   double         fChi2;      ///< Chi2 for this hit and the track node it was associated with

   ClassDef(TStiHitProxy, 1)
};


bool operator< (const TStiHitProxy& lhs, const TStiHitProxy& rhs);

#endif
