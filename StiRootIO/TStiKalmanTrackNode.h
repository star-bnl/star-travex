#ifndef TStiKalmanTrackNode_h
#define TStiKalmanTrackNode_h

#include <set>
#include <string>

#include "TObject.h"
#include "TVector3.h"

#include "Sti/StiKalmanTrackNode.h"
#include "StiRootIO/TStiHit.h"
#include "StiRootIO/TStiHitProxy.h"

class TStiKalmanTrack;

enum class StiNodeHitStatus {Any, Accepted, Rejected, Candidate};


class TStiKalmanTrackNode : public TObject
{
public:

   TStiKalmanTrackNode();
   TStiKalmanTrackNode(const StiKalmanTrackNode &stiKTN, TStiKalmanTrack* const parent=nullptr);
   bool  IsValid() const { return fValid; }
   bool  IsInsideVolume() const { return fIsInsideVolume; }
   const TVector3& GetTrackP() const { return fTrackP; }
   const TVector3& GetPosition() const { return fPosition; }
   const TVector3& GetPositionLocal() const { return fPositionLocal; }
   const TVector3& GetProjPositionLocal() const { return fProjPositionLocal; }
   const TVector3& GetProjError() const { return fProjError; }
   float GetEnergyLosses() const { return fabs(fEnergyLosses); }
   float GetNodeRadius() const { return fNodeRadius; }
   float GetNodeCenterRefAngle() const { return fNodeCenterRefAngle; }
   float GetNodeMaterialDensity() const { return fNodeMaterialDensity; }
   float GetNodeRelRadLength() const { return fNodeRelRadLength; }
   float GetNodeTrackLength() const { return fNodeTrackLength; }
   std::string GetVolumeName() const { return fVolumeName; }
   const TStiHit* GetHit() const { return fAcceptedStiHit; }
   const TStiHit* GetClosestHit() const { return fClosestStiHit; }
   std::set<const TStiHit*> GetCandidateHits() const;
   const std::set<TStiHitProxy>& GetCandidateProxyHits() const { return fCandidateStiHits; }
   void   FindClosestHit(const std::set<TStiHit>& stiHits) const;
   void   FindCandidateHits(const std::set<TStiHit>& stiHits) const;
   void   FindCandidateHitsByChi2(const std::set<TStiHit>& stiHits) const;
   void   AssignClosestCandidateHit() const { fAcceptedStiHit = fClosestStiHit; }
   double CalcDistanceToClosestHit() const { return fClosestStiHit ? (fClosestStiHit->GetPosition() - GetPosition()).Mag() : -1; }
   double CalcDistanceToHit() const { return fAcceptedStiHit ? (fAcceptedStiHit->GetPosition() - GetPosition()).Mag() : -1; }
   double CalcDistanceToHit(const TStiHit& hit) const { return (hit.GetPosition() - GetPosition()).Mag(); }
   TVector3 CalcPullToHit(const TStiHit& hit) const;
   TVector3 CalcPullClosestHit() const;
   double   CalcChi2(const TStiHit& hit) const;
   bool   MatchedVolName(const std::string & pattern) const;
   bool   MatchedVolName(const std::set<std::string> & patterns) const;
   virtual void Print(Option_t *opt = "") const;

   friend bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);

protected:

   const StiKalmanTrackNode* fStiTrackNode;  //!< Transient pointer to original StiKalmanTrackNode to access non-persistent info
   TStiKalmanTrack * const fTrack;    //!< Pointer to parent track containing this node
   float       fValid;                ///< A flag taken directly from StiKalmanTrackNode
   int         fIsInsideVolume;       ///< A flag taken directly from StiKalmanTrackNode
   TVector3    fPosition;             ///< Coordinates of the track state/node position
   TVector3    fPositionLocal;        ///< Local coordinates of the final (post refit) track node position
   TVector3    fProjPositionLocal;    ///< Coordinates of the track state/node position
   TVector3    fProjError;            ///< The projection error to the node before the fit
   TVector3    fTrackP;               ///< Track momentum vector in global CS
   float       fEnergyLosses;         ///< Energy lost in the volume
   float       fNodeRadius;           ///< The nominal radius of the Sti volume associated with this node
   float       fNodeCenterRefAngle;   ///< Angle to the center of the Sti volume associated with this node
   float       fNodeMaterialDensity;  ///< Density of the material of this node/volume
   float       fNodeTrackLength;      ///< Relative radiation length
   float       fNodeRelRadLength;     ///< Relative radiation length
   std::string fVolumeName;           ///< Name of Sti volume
   /// Pointer to the hit associated with this node by the reconstruction algorithm, if any
   mutable const TStiHit  *fAcceptedStiHit;
   mutable const TStiHit  *fClosestStiHit;    ///< Pointer to the hit closest to this node if any
   mutable std::set<TStiHitProxy> fCandidateStiHits;   ///< Collection of hits in some proximity of mean track projection

   ClassDef(TStiKalmanTrackNode, 10)
};


bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);


#endif
