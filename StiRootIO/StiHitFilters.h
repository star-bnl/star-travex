#ifndef StiHitFilters_h
#define StiHitFilters_h

#include <regex>
#include <string>

#include "StEvent/StEnumerations.h"
#include "Sti/Base/Filter.h"
#include "Sti/StiHit.h"



class StiHitFilterByDetectorGroup : public Filter<StiHit>
{
public:

   StiHitFilterByDetectorGroup(StDetectorId detGroupId) : Filter(), fDetGroupId(detGroupId) {}

   virtual bool accept(const StiHit *hit) const {
      return (hit && hit->detector() && hit->detector()->getGroupId() == fDetGroupId);
   }

private:

   StDetectorId  fDetGroupId;   //!< Detector group id used in this study
};



class StiHitFilterByVolumeName : public Filter<StiHit>
{
public:

   StiHitFilterByVolumeName(const std::set<std::string>& patterns) : Filter(),
      fVolNamePattern(patterns) {}

   virtual bool accept(const StiHit *hit) const
   {
      for (const auto& volNamePattern : fVolNamePattern)
      {
         if ( hit && hit->detector() && std::regex_match(std::string(hit->detector()->getName()), std::regex(volNamePattern)) )
            return true;
      }
   }

private:

   const std::set<std::string>&  fVolNamePattern;   //!< Sti detector
};

#endif
