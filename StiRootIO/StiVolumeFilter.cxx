#include "StiRootIO/StiVolumeFilter.h"
#include "TError.h"


/**
 * By default, we set no constraints on tracks w.r.t. their association with
 * a specific detector group
 */
StiVolumeFilter::StiVolumeFilter() : fgDetGroupId(kMaxDetectorId), fgDetActiveOnly(false),
   fgVolumeSelectPatterns()
{
   std::string   patternFileName("deactivate_sti_detectors.txt");
   std::ifstream volumePatternFile(patternFileName);
   std::string   pattern;

   while ( std::getline(volumePatternFile, pattern) )
   {
      try {
         boost::regex volNameRegex(pattern);
         fgVolumeSelectPatterns.insert(volNameRegex);
      }
      catch (boost::regex_error& e) {
         Warning(__FUNCTION__, "Regex [%s] found in [%s] is not valid", pattern.c_str(), patternFileName.c_str());
      }
   }
}


StiVolumeFilter& StiVolumeFilter::GetInstance()
{
   static StiVolumeFilter instance;
   return instance;
}


bool StiVolumeFilter::HasNodeAt(const StiKalmanTrack& stiKTrack, const boost::regex& volNamePattern)
{
   if (volNamePattern.empty()) return true;

   // Loop over track nodes
   for (const StiKalmanTrackNode& stiNode : stiKTrack)
   {
      if ( stiNode.isDca() || !stiNode.getDetector() ) continue;

      std::string volumeName( stiNode.getDetector()->getName() );

      if ( volumeName.empty() ) continue;

      if ( boost::regex_match(volumeName, volNamePattern) ) return true;
   }

   return false;
}
