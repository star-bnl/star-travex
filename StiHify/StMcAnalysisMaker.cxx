#include "TFile.h"
#include "TH3F.h"
#include "TNtuple.h"
#include "TSystem.h"

#include "StarClassLibrary/StParticleDefinition.hh"
#include "StEvent/StTrack.h"
#include "StEvent/StPrimaryTrack.h"
#include "StEvent/StTrackGeometry.h"
#include "StMcEvent/StMcEventTypes.hh"
#include "StMcEvent/StMcContainers.hh"
#include "StarClassLibrary/SystemOfUnits.h"
#include "StEvent/StEvent.h"
#include "StEvent/StTrackNode.h"
#include "StEvent/StGlobalTrack.h"
#include "StEvent/StTpcHit.h"
#include "StEvent/StTrackTopologyMap.h"

#include "StBFChain/StBFChain.h"
#include "StMcEvent/StMcEvent.hh"
#include "StAssociationMaker/StAssociationMaker.h"
#include "StAssociationMaker/StTrackPairInfo.hh"
#include "StMcEvent/StMcTpcHit.hh"
#include "StMcAnalysisMaker.h"
#include "StMcEvent/StMcTpcHitCollection.hh"

#include "StEvent/StEventTypes.h"
#include "StEventUtilities/StuRefMult.hh"
#include "StEvent/StEventSummary.h"
#include "StEvent/StBTofCollection.h"
#include "StEvent/StBTofHeader.h"
#include "StEvent/StEnumerations.h"
#include "StEvent/StTpcDedxPidAlgorithm.h"
#include "StMcEvent/StMcVertex.hh"
#include "StarClassLibrary/StParticleTypes.hh"

#include "StDetectorDbMaker/StDetectorDbTriggerID.h"


StMcAnalysisMaker::StMcAnalysisMaker(const char *name, const char *title): StMaker(name), mFile(NULL),
   mIsNtuple(true) , mNtuple(NULL), mTpcNtuple(NULL)
{
   cout << "StMcAnalysisMaker::StMcAnalysisMaker - DONE" << endl;
}
//__________________________________
int StMcAnalysisMaker::Init()
{
  StBFChain *bfChain = (StBFChain *) StMaker::GetChain();

   /*if (!bfChain) return kStFatal;
   TString fileName( gSystem->BaseName(bfChain->GetFileOut().Data()) );
   fileName = fileName.ReplaceAll(".event.root","");
   */

   if(!mOutFileName.Length()) mOutFileName = "mcAnalysis";
   mOutFileName = mOutFileName.ReplaceAll(".root","");

   mFile = new TFile(Form("%s.tpcRes.root",mOutFileName.Data()), "recreate");
   assert(mFile && !mFile->IsZombie());

   hTpcHitsDiffXVsPadrowVsSector = new TH3F("hTpcHitsDiffXVsPadrowVsSector", "hTpcHitsDiffXVsPadrowVsSector", 50, 0, 50, 26, 0, 26, 200, -2, 2);
   hTpcHitsDiffYVsPadrowVsSector = new TH3F("hTpcHitsDiffYVsPadrowVsSector", "hTpcHitsDiffYVsPadrowVsSector", 50, 0, 50, 26, 0, 26, 200, -2, 2);
   hTpcHitsDiffZVsPadrowVsSector = new TH3F("hTpcHitsDiffZVsPadrowVsSector", "hTpcHitsDiffZVsPadrowVsSector", 50, 0, 50, 26, 0, 26, 200, -2, 2);

   mAssoc = (StAssociationMaker*)GetMaker("StAssociationMaker");

   if (!mAssoc)
   {
      cout << " empty StAssociateMaker, stop!!" << endl;
      exit(1);
   }

   if (mIsNtuple)
   {
      mNtuple = new TNtuple("nt", "nt", "pt:phi:eta:geantId:svx:svy:svz:nCommon:nFit:nMax:gPt:gPhi:gEta:dca:dcaXY:dcaZ:hftTopo:isTrueHft");
      mTpcNtuple = new TNtuple("tpc","tpc","pt:nCommon:nFit:nMax:hOx:hOy:hOz:eta:phi:mcX:mcY:mcZ:mcTb:rcX:rcY:rcZ:rcTb:sector:padrow:pad:dE:adc:mcHitToRcTrackX:mcHitToRcTrackY:mcHitToRcTrackZ");
   }

   cout << "StMcAnalysisMaker::Init - DONE" << endl;
   return StMaker::Init();
}

//__________________________________
int StMcAnalysisMaker::Make()
{
   cout << "StMcAnalysisMaker::Make() - call" << endl;
   StMcEvent* mcEvent = (StMcEvent*)GetDataSet("StMcEvent");

   if (!mcEvent)
   {
      LOG_WARN << "No StMcEvent" << endm;
      return kStWarn;
   }

   StEvent* event = (StEvent*)GetDataSet("StEvent");
   if (!event)
   {
      LOG_WARN << "No StEvent" << endm;
      return kStWarn;
   }

   cout << "StMcAnalysisMaker::Make() : event: " << event->id() << endl;

   return fillTracks(mcEvent,event);
}
//____________________________________
int StMcAnalysisMaker::fillTracks(StMcEvent* mcEvent,StEvent* event)
{
   StSPtrVecMcTrack& trks = mcEvent->tracks();
   cout << "Filling " << trks.size() << " mcTracks..." << endl;

   if (!mAssoc->rcTpcHitMap())
   {
      cout << "!!!There is no rcTpcHitMap in the association maker!!!!" << endl;
      return 1;
   }

   for (unsigned int i = 0;  i < trks.size(); i++)
   {
      StMcTrack* mcTrack = trks[i];

      float pTrkSvx, pTrkSvy;
      if(mcTrack->startVertex()) 
      {
        pTrkSvx = mcTrack->startVertex()->position().x();
        pTrkSvy = mcTrack->startVertex()->position().y();
      }
      else continue;

      if(sqrt(pTrkSvx*pTrkSvx + pTrkSvy*pTrkSvy)>3.) continue;
      
      int ncommonhits = 0;
      const StTrack* rcTrack = findPartner(mcTrack, ncommonhits);

      bool istTruth = true;
      bool pxlTruth1 = true;
      bool pxlTruth2 = true;

      if (rcTrack)
      {
         StPtrVecHit rcIstHits = rcTrack->detectorInfo()->hits(kIstId);
         StPtrVecHit rcPxlHits = rcTrack->detectorInfo()->hits(kPxlId);

         int nRcIstHits = (int)rcIstHits.size();
         int nRcPxlHits = (int)rcPxlHits.size();


         for (int iHit = 0; iHit < nRcIstHits; ++iHit)
         {
            if (rcIstHits[iHit]->idTruth() != mcTrack->key())
            {
               istTruth = false;
               break;
            }
         }

         for (int iHit = 0; iHit < nRcPxlHits; ++iHit)
         {
            if (rcPxlHits[iHit]->idTruth() != mcTrack->key())
            {
               StThreeVectorF pos = rcPxlHits[iHit]->position();

               float const R = pow(pos.x(), 2.0) + pow(pos.y(), 2.0);
               if (R > 3.5 * 3.5) pxlTruth2 = false;
               else pxlTruth1 = false;
            }
         }
      }

      float dca = -999.;
      float dcaXY = -999.;
      float dcaZ = -999.;

      if(rcTrack)
      {
        StPhysicalHelixD helix = rcTrack->geometry()->helix();
        dca = helix.distance(event->primaryVertex()->position());
        dcaXY = helix.geometricSignedDistance(event->primaryVertex()->position().x(),event->primaryVertex()->position().y());

        StThreeVectorF dcaPoint = helix.at(helix.pathLength(event->primaryVertex()->position()));
        dcaZ = dcaPoint.z() - event->primaryVertex()->position().z();
      }

      float arr[50];
      int iArr = 0;
      arr[iArr++] = mcTrack->pt(); 
      arr[iArr++] = mcTrack->momentum().phi(); 
      arr[iArr++] = mcTrack->pseudoRapidity(); 
      arr[iArr++] = mcTrack->geantId();
      arr[iArr++] = mcTrack->startVertex()->position().x(); 
      arr[iArr++] = mcTrack->startVertex()->position().y(); 
      arr[iArr++] = mcTrack->startVertex()->position().z();
      arr[iArr++] = rcTrack ? ncommonhits : -999;
      arr[iArr++] = rcTrack ? rcTrack->fitTraits().numberOfFitPoints(kTpcId) : -999;
      arr[iArr++] = rcTrack ? rcTrack->numberOfPossiblePoints() : -999;
      arr[iArr++] = rcTrack ? rcTrack->geometry()->momentum().perp() : -999;
      arr[iArr++] = rcTrack ? rcTrack->geometry()->momentum().phi() : -999;
      arr[iArr++] = rcTrack ? rcTrack->geometry()->momentum().pseudoRapidity() : -999;
      arr[iArr++] = dca;
      arr[iArr++] = dcaXY;
      arr[iArr++] = dcaZ;
      arr[iArr++] = rcTrack ? (rcTrack->topologyMap().data(0)) >> 1 & 0x7F: 0;
      arr[iArr++] = static_cast<float>(istTruth && pxlTruth1 && pxlTruth2);
      mNtuple->Fill(arr);

      if (!rcTrack) continue;

      StPtrVecHit rcTpcHits = rcTrack->detectorInfo()->hits(kTpcId);
      // const StPtrVecMcHit* mcTpcHits = mcTrack->Hits(kTpcId);

      StPhysicalHelixD helix = rcTrack->geometry()->helix();

      for (size_t ih = 0; ih < rcTpcHits.size(); ih++)
      {
         StTpcHit* rcHit = dynamic_cast<StTpcHit*>(rcTpcHits[ih]);
         if (!rcHit) continue;

         pair<rcTpcHitMapIter, rcTpcHitMapIter>  bounds = mAssoc->rcTpcHitMap()->equal_range(rcHit);

         // loop over all mcHits associated with this rcHit
         bool found = false;
         for (rcTpcHitMapIter iter = bounds.first; iter != bounds.second; iter++)
         {
            const StMcTpcHit* mcHit = (*iter).second;

            // fill histograms if this mcHit belongs to this mcTrack
            if (mcHit->parentTrack()->key() == mcTrack->key())
            {
               StThreeVectorF mcHitTotRcTrack = helix.at(helix.pathLength(mcHit->position().x(),mcHit->position().y())) - mcHit->position();

               float arr[55];
               int iArr = 0;
               arr[iArr++] = mcTrack->pt();
               arr[iArr++] = rcTrack ? ncommonhits : -999;
               arr[iArr++] = rcTrack ? rcTrack->fitTraits().numberOfFitPoints(kTpcId) : -999;
               arr[iArr++] = rcTrack ? rcTrack->numberOfPossiblePoints() : -999;
               arr[iArr++] = helix.origin().x();
               arr[iArr++] = helix.origin().y();
               arr[iArr++] = helix.origin().z();
               arr[iArr++] = mcTrack->pseudoRapidity();
               arr[iArr++] = mcTrack->momentum().phi();
               arr[iArr++] = mcHit->position().x();
               arr[iArr++] = mcHit->position().y();
               arr[iArr++] = mcHit->position().z();
               arr[iArr++] = mcHit->timeBucket();
               arr[iArr++] = rcHit->position().x();
               arr[iArr++] = rcHit->position().y();
               arr[iArr++] = rcHit->position().z();
               arr[iArr++] = rcHit->timeBucket();
               arr[iArr++] = mcHit->sector();
               arr[iArr++] = mcHit->padrow();
               arr[iArr++] = mcHit->pad();
               arr[iArr++] = mcHit->dE();
               arr[iArr++] = rcHit->adc();
               arr[iArr++] = mcHitTotRcTrack.x();
               arr[iArr++] = mcHitTotRcTrack.y();
               arr[iArr++] = mcHitTotRcTrack.z();

               mTpcNtuple->Fill(arr);

               hTpcHitsDiffXVsPadrowVsSector->Fill(mcHit->padrow(), mcHit->sector(), mcHit->position().x() - rcHit->position().x());
               hTpcHitsDiffYVsPadrowVsSector->Fill(mcHit->padrow(), mcHit->sector(), mcHit->position().y() - rcHit->position().y());
               hTpcHitsDiffZVsPadrowVsSector->Fill(mcHit->padrow(), mcHit->sector(), mcHit->position().z() - rcHit->position().z());
               found = true;
            }
            else
            {
               cout << "Not a good candidate" << endl;
            }
         }

         if (!found)
         {
            cout << "No mc hit was found for this rc Hit!!!!" << endl;
         }
      }


      /*
         cout<<"ncommonhits "<<ncommonhits<<endl;
         cout<<"Rc track hits"<<endl;
         for(size_t ih = 0; ih <rcTpcHits.size(); ih++)
         {
         StTpcHit* rcHit = dynamic_cast<StTpcHit*>(rcTpcHits[ih]);
         if(!rcHit) continue;
         cout<<rcHit->qaTruth()<<endl;
         }

         cout<<"MC track hits"<<endl;
         for(size_t ih = 0; ih <mcTpcHits->size(); ih++)
         {
        StMcTpcHit* mcHit = dynamic_cast<StMcTpcHit*>((*mcTpcHits)[ih]);
        if(!mcHit) continue;
        cout<<mcHit->key()<<endl;
      }
      */
   }

   return kStOk;
}


//________________________________________________
const StTrack* StMcAnalysisMaker::findPartner(StMcTrack* mcTrack, int& maxCommonTpcHits)
{
   //..StMcTrack find partner from the StTracks
   pair<mcTrackMapIter, mcTrackMapIter> p = mAssoc->mcTrackMap()->equal_range(mcTrack);

   const StTrack* maxTrack = 0;
   maxCommonTpcHits = 0;
   for (mcTrackMapIter k = p.first; k != p.second; ++k)
   {
      int commonTpcHits = k->second->commonTpcHits();
      const StTrack* track = k->second->partnerTrack()->node()->track(global);//should be global
      if (track && commonTpcHits > maxCommonTpcHits)
      {
         maxTrack = track;
         maxCommonTpcHits = commonTpcHits;
      }
   }
   return maxTrack;
}


//________________________________________________
const StMcTrack* StMcAnalysisMaker::findPartner(StGlobalTrack* rcTrack, int& maxCommonTpcHits)
{
   //.. StGlobalTracks find partner from StMcTracks.
   //.. See example from StRoot/StMcAnalysisMaker
   pair<rcTrackMapIter, rcTrackMapIter> p = mAssoc->rcTrackMap()->equal_range(rcTrack);

   const StMcTrack* maxTrack = 0;
   maxCommonTpcHits = 0;
   for (rcTrackMapIter k = p.first; k != p.second; ++k)
   {
      int commonTpcHits = k->second->commonTpcHits();

      const StMcTrack* track = k->second->partnerMcTrack();

      if (track && commonTpcHits > maxCommonTpcHits)
      {
         maxTrack = track;
         maxCommonTpcHits = commonTpcHits;
      }
   }
   return maxTrack;
}
//______________________________________________________
int StMcAnalysisMaker::Finish()
{
   mFile->cd();
   if (mIsNtuple) 
   {
     mNtuple->Write();
     mTpcNtuple->Write();
   }
   hTpcHitsDiffXVsPadrowVsSector->Write();
   hTpcHitsDiffYVsPadrowVsSector->Write();
   hTpcHitsDiffZVsPadrowVsSector->Write();
   mFile->Close();
   return kStOk;
}
