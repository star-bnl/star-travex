#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/format.hpp>

#include "evedisp/EventReader.h"

#include "TDataSet.h"
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"
#include "TKey.h"
#include "TList.h"

#include "StarClassLibrary/StThreeVectorD.hh"
#include "St_base/StTree.h"
#include "StEvent/StEvent.h"
#include "StEvent/StTpcHitCollection.h"
#include "StEvent/StTpcSectorHitCollection.h"
#include "StEvent/StTpcPadrowHitCollection.h"
#include "StEvent/StHit.h"
#include "StEvent/StTpcHit.h"
#include "StEvent/StTrack.h"
#include "StEvent/StTrackDetectorInfo.h"
#include "StEvent/StTrackNode.h"
#include "StEvent/StGlobalTrack.h"
#include "StEvent/StDcaGeometry.h"



EventReader::EventReader(std::string file_name, TEvePointSet &hits, TEveTrackList &tracks,
                         TEvePointSetArray &trackHits) :
   fFile(new TFile(file_name.c_str(), "READ")),
   fHits(hits),
   fTracks(tracks),
   fTrackHits(trackHits)
{
}



void StEventReader::ReadNext()
{
   //StIOEvent* stIOEvent = static_cast<StIOEvent*>(fFile->Get("bfcTree.0000002000"));

   ////StIOEvent* stIOEvent = static_cast<StIOEvent*>(fFile->Get("eventBranch.0018069061.0001494262"));

   //StTree* stTree = static_cast<StTree*>(stIOEvent->fObj);
   ////StTree* stTree = new StTree("bfcTree");

   //stTree->SetTFile(fFile);
   //stTree->SetIOMode("1");
   //stTree->NextEvent();
   //stTree->Print();
   //stTree->Dump();

   TList* keys = fFile->GetListOfKeys();
   TIter nextKey(keys);

   int iEvent = 0;

   TKey* key = (TKey*) keys->First();
   //while (TKey* key = (TKey*) nextKey())
   //{
   //if (iEvent > 1) break;

   std::cout << "\n\nEvent: " << iEvent << std::endl;

   key->Print();

   StIOEvent* stIOEvent = static_cast<StIOEvent*>(key->ReadObj());

   TDataSet* stEventDS = static_cast<TDataSet*>(stIOEvent->fObj);
   //stEventDS->Print();
   //stEventDS->Dump();

   StEvent* stEvent = static_cast<StEvent*>(stEventDS->FindObject("StEvent"));
   //std::cout << "xxx: " << stEvent->mContent.size() << std::endl;
   stEvent->Print();
   stEvent->Dump();
   stEvent->statistics();

   //iEvent++;
   //}


   //TDataSetIter next(stTree);
   //StBranch *br=0;

   ////int kase = 0; StBranch *br0=0;
   //while (br = (StBranch*)next() )
   //{
   //  //br->NextEvent();
   //  //br->ReadEvent(br0->GetUKey());
   //  br->Print();
   //  br->Dump();
   //}


   //stTree->SetIOMode("1");
   //stTree->NextEvent();
   //stTree->GetEvent(1);

   ////stTree->GetUKey().GetKey();

   //std::cout << "\n\nkey: " << stTree->GetUKey().GetKey() << std::endl;

   ////stTree->Print();
   ////stTree->Dump();

   //StBranch* stEventBranch = static_cast<StBranch*>(stTree->FindObject("eventBranch"));
   //stEventBranch->Print();
   //stEventBranch->Dump();

   //TList* events = stEventBranch->GetList();

   //for(const auto&& obj: *events)
   //  obj->Print();



   //return;

   //TKey* key2 = (TKey*)keys->First();

   //StIOEvent* stIOEvent = static_cast<StIOEvent*>(key2->ReadObj());
   //if (strcmp("StIOEvent",ky->GetClassName()))       continue;

   //StEvent* stEvent = static_cast<StEvent*>(stIOEvent->FindObject("StEvent"));
   //stEvent->Print();
   //stEvent->Dump();
   //stEvent->statistics();

   //StBranch* stEventBranch = static_cast<StBranch*>(stTree->FindObject("eventBranch"));

   //StEvent* stEvent = static_cast<StEvent*>(stEventBranch->FindObject("StEvent"));
   //std::cout << "xxx: " << stEvent->mContent.size() << std::endl;

   StTpcHitCollection *tpcHitCollection = stEvent->tpcHitCollection();

   for (int iSector = 0; iSector < tpcHitCollection->numberOfSectors(); iSector++) {
      StTpcSectorHitCollection *tpcSectorHitCollection = tpcHitCollection->sector(iSector);

      for (int iPadrow = 0; iPadrow < tpcSectorHitCollection->numberOfPadrows(); iPadrow++) {
         StTpcPadrowHitCollection *tpcPadrowHitCollection = tpcSectorHitCollection->padrow(iPadrow);
         StSPtrVecTpcHit &tpcHits = tpcPadrowHitCollection->hits();

         //std::cout << "sector, padrow, size: " << iSector << ", " << iPadrow << ", " << tpcHits.size() << std::endl;

         for (int i_hit = 0; i_hit < tpcHits.size(); i_hit++) {
            //StHit* hit = reinterpret_cast<StHit*>(tpcHits[i_hit]);
            StTpcHit *hit = tpcHits[i_hit];

            if (!hit) continue;

            fHits.SetNextPoint(hit->position().x(), hit->position().y(), hit->position().z());
         }
      }
   }


   TEveTrackPropagator *trkProp = fTracks.GetPropagator();
   StSPtrVecTrackNode  &stTracks = stEvent->trackNodes();

   fTrackHits.InitBins("track_bins", stTracks.size(), 1, stTracks.size() + 1);

   std::ostringstream oss("");

   for (int i_track = 0; i_track <= stTracks.size() + 1; ++i_track) {
      oss.str("");
      oss << boost::format("gth_%|04|") % i_track;
      fTrackHits.GetBin(i_track)->SetRnrState(false);
      fTrackHits.GetBin(i_track)->SetName(oss.str().c_str());
   }


   for (StSPtrVecTrackNodeConstIterator iter = stTracks.begin(); iter != stTracks.end(); iter++) {
      StTrackNode *stTrackNode = *iter;

      const StGlobalTrack *globalStTrack = dynamic_cast<const StGlobalTrack *>(stTrackNode->track(global));
      const StDcaGeometry *dcaGeometry = globalStTrack ? globalStTrack->dcaGeometry() : nullptr;

      if (!dcaGeometry) continue;

      int i_track = globalStTrack->key();

      StThreeVectorD dcaMomentum = dcaGeometry->momentum();
      StThreeVectorD dcaPosition{
         -dcaGeometry->impact() *std::sin( dcaGeometry->psi() ),
            dcaGeometry->impact() *std::cos( dcaGeometry->psi() ),
            dcaGeometry->z()
         };

      TEveVectorT<double> momentum( dcaMomentum.x(), dcaMomentum.y(), dcaMomentum.z() );
      TEveVectorT<double> origin( dcaPosition.x(), dcaPosition.y(), dcaPosition.z() );
      int charge = (dcaGeometry->curvature() > 0) ? +1 : -1;

      TEveRecTrackT<double> eveRecTrack;
      eveRecTrack.fIndex = i_track; //++;
      eveRecTrack.fP = momentum;
      eveRecTrack.fV = origin;
      eveRecTrack.fSign = charge;

      // Access detector info
      const StTrackDetectorInfo *detectorInfo = globalStTrack->detectorInfo();

      unsigned short n_ref_points = detectorInfo->numberOfReferencedPoints();
      const StPtrVecHit &hits = detectorInfo->hits();

      for (auto &hit : hits) {
         fTrackHits.Fill(hit->position().x(), hit->position().y(), hit->position().z(), i_track);
      }


      // Read topologyMap
      const StTrackTopologyMap &topologyMap = globalStTrack->topologyMap();

      unsigned n_hits_Tpc = topologyMap.numberOfHits(kTpcId);
      unsigned n_hits_iTpc = topologyMap.numberOfHits(kiTpcId);

      oss.str("");
      oss << boost::format("gt_%|04|_%|02|_%|02|_%|02|") % i_track % n_hits_Tpc % n_hits_iTpc % n_ref_points;

      TEveTrack *eveTrack = new TEveTrack(&eveRecTrack, trkProp);
      eveTrack->SetName( oss.str().c_str() );
      eveTrack->SetAttLineAttMarker(&fTracks);
      eveTrack->SetRnrState(false);
      eveTrack->AddElement(fTrackHits.GetBin(i_track));

      fTracks.AddElement(eveTrack);
   }

   fTracks.MakeTracks();
   fTracks.SetRnrState(true);
   fTrackHits.SetRnrState(true);
}
