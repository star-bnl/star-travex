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
   const TVector3& GetError() const { return fError; }
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
   void     FindClosestHit(const std::set<TStiHit>& stiHits) const;
   void     FindCandidateHits(const std::set<TStiHit>& stiHits) const;
   void     FindCandidateHitsByChi2(const std::set<TStiHit>& stiHits) const;
   void     AssignClosestCandidateHit() const { fAcceptedStiHit = fClosestStiHit; }
   double   CalcDiffProjToFitPositionWRTHit() const { return CalcDistanceProjToHit() - CalcDistanceToHit(); }
   TVector3 CalcDiffProjToFitError() const { return fProjError - fError; }
   double   CalcDistanceToClosestHit() const { return fClosestStiHit ? (fClosestStiHit->GetPosition() - GetPosition()).Mag() : -1; }
   double   CalcDistanceToHit() const { return fAcceptedStiHit ? (fAcceptedStiHit->GetPosition() - GetPosition()).Mag() : -1; }
   double   CalcDistanceProjToHit() const { return fAcceptedStiHit ? (fAcceptedStiHit->GetPositionLocal() - GetProjPositionLocal()).Mag() : -1; }
   double   CalcDistanceToHit(const TStiHit& hit) const { return (hit.GetPosition() - GetPosition()).Mag(); }
   TVector3 CalcPullToHit(const TStiHit& hit) const;
   TVector3 CalcPullClosestHit() const;
   double   CalcChi2(const TStiHit& hit) const;
   bool     MatchedVolName(const std::string & pattern) const;
   bool     MatchedVolName(const std::set<std::string> & patterns) const;
   virtual void Print(Option_t *opt = "") const;

   friend bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);

protected:

   /// Transient pointer to original StiKalmanTrackNode to access non-persistent info
   const StiKalmanTrackNode *fStiTrackNode; //! transient member

   /// Pointer to parent track containing this node
   TStiKalmanTrack *fTrack;                 //! transient member

   /// A flag taken directly from StiKalmanTrackNode
   float  fValid;

   /// A flag taken directly from StiKalmanTrackNode
   int  fIsInsideVolume;

   /// Global coordinates of the final (post re-fit) track node position
   TVector3  fPosition;

   /// Diagonal elements of error matrix after final fit
   TVector3  fError;

   /// Local coordinates of the final (post re-fit) track node position
   TVector3  fPositionLocal;

   /// Local coordinates of the projected (pre re-fit) track node position
   TVector3  fProjPositionLocal;

   /// The projection error to the node before the re-fit
   TVector3  fProjError;

   /// Track momentum vector in global CS
   TVector3  fTrackP;

   /// Energy lost in the volume
   float  fEnergyLosses;

   /// The nominal radius of the Sti volume associated with this node
   float  fNodeRadius;

   /// Angle to the center of the Sti volume associated with this node
   float  fNodeCenterRefAngle;

   /// Density of the material of this node/volume
   float  fNodeMaterialDensity;

   /// Relative radiation length
   float  fNodeTrackLength;

   /// Relative radiation length
   float  fNodeRelRadLength;

   /// Name of Sti volume
   std::string  fVolumeName;

   /// Pointer to the hit associated with this node by the reconstruction algorithm, if any
   mutable const TStiHit  *fAcceptedStiHit;

   /// Pointer to the hit closest to this node if any
   mutable const TStiHit  *fClosestStiHit;

   /// Collection of hits in some proximity of mean track projection
   mutable std::set<TStiHitProxy> fCandidateStiHits;

   ClassDef(TStiKalmanTrackNode, 11)
};


bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);


#endif
