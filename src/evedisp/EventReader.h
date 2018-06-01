#include <string>

#include "TChain.h"
#include "TEvePointSet.h"
#include "TEveTrack.h"
#include "TFile.h"

#include "tml/event.hpp"

enum class EventType { kStEvent };


class EventReader
{
public:

   EventReader(std::string file_name, TEvePointSet &hits, TEveTrackList &tracks,
               TEvePointSetArray &trackHits);

   virtual ~EventReader() { delete fFile; fFile = nullptr; }

   virtual void ReadNext() = 0;

protected:

   TFile *fFile;
   TEvePointSet &fHits;
   TEveTrackList &fTracks;
   TEvePointSetArray &fTrackHits;
};



class StEventReader : public EventReader
{
public:

   StEventReader(std::string file_name, TEvePointSet &hits, TEveTrackList &tracks,
                 TEvePointSetArray &trackHits) :
      EventReader(file_name, hits, tracks, trackHits) {}

   virtual void ReadNext();
};



