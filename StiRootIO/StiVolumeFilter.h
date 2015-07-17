#ifndef StiVolumeFilter_h
#define StiVolumeFilter_h

#include <boost/regex.hpp>

#include "StEvent/StEnumerations.h"

class StiKalmanTrack;
class StiKalmanTrackNode;

/** 
 * A singleton to manage constraints on save Sti objects such as tracks and
 * track nodes.
 */
class StiVolumeFilter
{
public:

   static StiVolumeFilter& GetInstance();

   StDetectorId  GetDetGroupId() const { return fgDetGroupId; }

   bool AcceptTrack(const StiKalmanTrack& track) const;
   bool AcceptTrackNode(const StiKalmanTrackNode& node) const;
   bool HasAcceptedNode(const StiKalmanTrack& track) const;
   const std::set<boost::regex>& GetNamePatterns() const { return fgVolumeSelectPatterns; }

protected:

   StDetectorId           fgDetGroupId;        //!< Detector group id used in this study
   bool                   fgDetActiveOnly;     //!< Another selection criteria to save nodes
	/// A set of regex patterns to select and save track nodes passing through
	/// volumes with matching names
   std::set<boost::regex> fgVolumeSelectPatterns;

private:

   StiVolumeFilter();
   StiVolumeFilter(const StiVolumeFilter& ) = delete;
   void operator= (StiVolumeFilter const& ) = delete;
};

#endif
