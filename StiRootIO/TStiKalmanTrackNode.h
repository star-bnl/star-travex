#ifndef TStiKalmanTrackNode_h
#define TStiKalmanTrackNode_h

#include <set>
#include <string>

#include "TObject.h"
#include "TVector3.h"

#include "Sti/StiKalmanTrackNode.h"
#include "StiRootIO/TStiHit.h"

class TStiKalmanTrack;


class TStiKalmanTrackNode : public TObject
{
public:

   TStiKalmanTrackNode();
   TStiKalmanTrackNode(TStiKalmanTrack* const track, const StiKalmanTrackNode &stiKTN);
   bool  IsValid() const { return fValid; }
   const TVector3& GetTrackP() const { return fTrackP; }
   const TVector3& GetPosition() const { return fPosition; }
   float GetEnergyLosses() const { return fabs(fEnergyLosses); }
   float GetNodeRadius() const { return fNodeRadius; }
   float GetNodeCenterRefAngle() const { return fNodeCenterRefAngle; }
   float GetNodeMaterialDensity() const { return fNodeMaterialDensity; }
   float GetNodeRelRadLength() const { return fNodeRelRadLength; }
   float GetNodeTrackLength() const { return fNodeTrackLength; }
   std::string GetVolumeName() const { return fVolumeName; }
   void   AssignClosestHit(const std::set<TStiHit>& stiHits);
	double CalcDistanceToClosestHit() { return (fClosestStiHit->GetPosition() - GetPosition()).Mag(); }
   bool   MatchedVolName(const std::string & pattern) const;
   bool   MatchedVolName(const std::set<std::string> & patterns) const;
   virtual void Print(Option_t *opt = "") const;

   friend bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);

protected:

   TStiKalmanTrack * const fTrack;   //!< Pointer to parent track containing this node
   float       fValid;             ///< A flag taken directly from StiKalmanTrackNode
   TVector3    fPosition;          ///< Coordinates of the track state/node position
   TVector3    fTrackP;            ///< Track momentum vector in global CS
   float       fEnergyLosses;      ///< Energy lost in the volume
   float       fNodeRadius;        ///< The nominal radius of the Sti volume associated with this node
   float       fNodeCenterRefAngle;   ///< Angle to the center of the Sti volume associated with this node
   float       fNodeMaterialDensity;  ///< Density of the material of this node/volume
   float       fNodeTrackLength;     ///< Relative radiation length
   float       fNodeRelRadLength;     ///< Relative radiation length
   std::string fVolumeName;           ///< Name of Sti volume
   const TStiHit  *fStiHit;               ///< A pointer to the hit associated with this node if any
   const TStiHit  *fClosestStiHit;    ///< A pointer to the hit associated with this node if any
   double      fTrackProjErr;         ///< The projection error to the node before the fit

   ClassDef(TStiKalmanTrackNode, 8)
};


bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);


#endif
