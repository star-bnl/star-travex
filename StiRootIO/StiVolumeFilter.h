#ifndef StiVolumeFilter_h
#define StiVolumeFilter_h

#include <boost/regex.hpp>

#include "StEvent/StEnumerations.h"
#include "Sti/StiKalmanTrack.h"


/** 
 * A singleton to manage constraints on save Sti objects such as tracks and
 * track nodes.
 */
class StiVolumeFilter
{
public:

   static StiVolumeFilter& GetInstance();

   bool HasNodeAt(const StiKalmanTrack& stiKTrack, const boost::regex& volNamePattern);
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
