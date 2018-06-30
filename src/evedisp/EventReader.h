#include <string>

#include "TChain.h"
#include "TEvePointSet.h"
#include "TEveTrack.h"
#include "TFile.h"

#include "tml/event.hpp"


enum class EventType { kStEvent, kTmlEvent };


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



class TmlEventReader : public EventReader
{
public:

   TmlEventReader(std::string file_name, TEvePointSet &hits, TEveTrackList &tracks,
                     TEvePointSetArray &trackHits) :
      EventReader(file_name, hits, tracks, trackHits),
      fTmlTree(new TChain("tree", "READ")),
      fTmlEvent(new tml::Event())
   {
       fTmlTree->AddFile(file_name.c_str());
       fTmlTree->SetBranchAddress<tml::Event>("event", &fTmlEvent);
   }

   virtual ~TmlEventReader()
   {
       delete fTmlTree; fTmlTree = nullptr;
       delete fTmlEvent; fTmlEvent = nullptr;
   }

   virtual void ReadNext();

private:

   TChain*     fTmlTree;
   tml::Event* fTmlEvent;

};
