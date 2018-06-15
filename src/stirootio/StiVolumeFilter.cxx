#include "stirootio/StiVolumeFilter.h"
#include "TError.h"
#include "Sti/StiKalmanTrack.h"
#include "Sti/StiKalmanTrackNode.h"


/**
 * By default, we set no constraints on tracks w.r.t. their association with
 * a specific detector group
 */
StiVolumeFilter::StiVolumeFilter() : fgDetGroupId(kMaxDetectorId), fgDetActiveOnly(false),
   fgVolumeSelectPatterns()
{
   std::string   patternFileName("save_sti_detectors.txt");
   std::ifstream volumePatternFile(patternFileName);
   std::string   pattern;

   while ( std::getline(volumePatternFile, pattern) )
   {
      try {
         std::regex volNameRegex(pattern);
         fgVolumeSelectPatterns.insert(pattern);
      }
      catch (std::regex_error& e) {
         Warning(__FUNCTION__, "Regex [%s] found in [%s] is not valid", pattern.c_str(), patternFileName.c_str());
      }
   }
}


StiVolumeFilter& StiVolumeFilter::GetInstance()
{
   static StiVolumeFilter instance;
   return instance;
}


bool StiVolumeFilter::AcceptTrack(const StiKalmanTrack& track) const
{
   return HasAcceptedNode(track);
}


bool StiVolumeFilter::AcceptTrackNode(const StiKalmanTrackNode& node) const
{
   // Always accept DCA nodes
   if ( node.isDca() ) return true;

   // Continue with a non-DCA node
   // Always reject nodes without a proper detector
   if ( !node.getDetector() ) return false;

   // Non-DCA node with detector defined
   std::string volumeName( node.getDetector()->getName() );

   // Always reject nodes without a proper detector name
   if ( volumeName.empty() ) return false;

   // If no patterns defined check detectorGroupId and active det is not used but should be at some
   // point...
   if (fgVolumeSelectPatterns.empty())
   {
      StDetectorId stiNodeDetId = static_cast<StDetectorId>( node.getDetector()->getGroupId() );

      if ( ( fgDetGroupId == stiNodeDetId || fgDetGroupId == kMaxDetectorId ) &&
           ( (fgDetActiveOnly && node.getDetector()->isActive()) || !fgDetActiveOnly )
         )
      {
         return true;
      }

      return false;
   }

   for (const auto& volNamePattern : fgVolumeSelectPatterns)
   {
      if ( std::regex_match(volumeName, std::regex(volNamePattern)) ) return true;
   }

   return false;
}


bool StiVolumeFilter::HasAcceptedNode(const StiKalmanTrack& track) const
{
   // By convention return true if no patterns defined
   if (fgVolumeSelectPatterns.empty()) return true;

   // Loop over track nodes
   for (const StiKalmanTrackNode& node : track)
   {
      // The decision to accept does not take into account DCA nodes
      if ( node.isDca() ) continue;

      if ( AcceptTrackNode(node) ) return true;
   }

   return false;
}
