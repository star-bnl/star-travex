#ifndef TStiKalmanTrackNode_h
#define TStiKalmanTrackNode_h

#include <set>
#include <string>

#include "TObject.h"
#include "TVector3.h"

#include "Sti/StiKalmanTrackNode.h"
#include "StiRootIO/TStiHit.h"

class TStiKalmanTrack;

enum class StiNodeHitStatus {Any, Accepted, Rejected, Candidate};


class TStiKalmanTrackNode : public TObject
{
public:

   TStiKalmanTrackNode();
   TStiKalmanTrackNode(TStiKalmanTrack* const track, const StiKalmanTrackNode &stiKTN);
   bool  IsValid() const { return fValid; }
   bool  IsInsideVolume() const { return fIsInsideVolume; }
   const TVector3& GetTrackP() const { return fTrackP; }
   const TVector3& GetPosition() const { return fPosition; }
   const TVector3& GetPositionLocal() const { return fPositionLocal; }
   const TVector3& GetTrackProjErr() const { return fTrackProjErr; }
   float GetEnergyLosses() const { return fabs(fEnergyLosses); }
   float GetNodeRadius() const { return fNodeRadius; }
   float GetNodeCenterRefAngle() const { return fNodeCenterRefAngle; }
   float GetNodeMaterialDensity() const { return fNodeMaterialDensity; }
   float GetNodeRelRadLength() const { return fNodeRelRadLength; }
   float GetNodeTrackLength() const { return fNodeTrackLength; }
   std::string GetVolumeName() const { return fVolumeName; }
   const TStiHit* GetHit() const { return fStiHit; }
   const TStiHit* GetClosestHit() const { return fClosestStiHit; }
   const std::set<const TStiHit*>& GetAdjacentHits() const { return fAdjacentStiHits; }
   void   AssignClosestHit(const std::set<TStiHit>& stiHits);
   void   FindAdjacentHits(const std::set<TStiHit>& stiHits);
   double CalcDistanceToClosestHit() const { return fClosestStiHit ? (fClosestStiHit->GetPosition() - GetPosition()).Mag() : -1; }
   double CalcDistanceToHit() const { return fStiHit ? (fStiHit->GetPosition() - GetPosition()).Mag() : -1; }
   TVector3 CalcPullToHit(const TStiHit& hit) const;
   TVector3 CalcPullClosestHit() const;
   bool   MatchedVolName(const std::string & pattern) const;
   bool   MatchedVolName(const std::set<std::string> & patterns) const;
   virtual void Print(Option_t *opt = "") const;

   friend bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);

protected:

   TStiKalmanTrack * const fTrack;    //!< Pointer to parent track containing this node
   float       fValid;                ///< A flag taken directly from StiKalmanTrackNode
   int         fIsInsideVolume;       ///< A flag taken directly from StiKalmanTrackNode
   TVector3    fPosition;             ///< Coordinates of the track state/node position
   TVector3    fPositionLocal;        ///< Coordinates of the track state/node position
   TVector3    fTrackP;               ///< Track momentum vector in global CS
   float       fEnergyLosses;         ///< Energy lost in the volume
   float       fNodeRadius;           ///< The nominal radius of the Sti volume associated with this node
   float       fNodeCenterRefAngle;   ///< Angle to the center of the Sti volume associated with this node
   float       fNodeMaterialDensity;  ///< Density of the material of this node/volume
   float       fNodeTrackLength;      ///< Relative radiation length
   float       fNodeRelRadLength;     ///< Relative radiation length
   std::string fVolumeName;           ///< Name of Sti volume
   const TStiHit  *fStiHit;           ///< Pointer to the hit associated with this node if any
   const TStiHit  *fClosestStiHit;    ///< Pointer to the hit closest to this node if any
   std::set<const TStiHit*> fAdjacentStiHits;   ///< Collection of hits in some proximity of mean track projection
   TVector3    fTrackProjErr;         ///< The projection error to the node before the fit

   ClassDef(TStiKalmanTrackNode, 9)
};


bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);


#endif
