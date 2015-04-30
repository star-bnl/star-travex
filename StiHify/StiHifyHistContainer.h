#ifndef StiHifyHistContainer_h
#define StiHifyHistContainer_h

#include <set>
#include <string>

#include "TH1I.h"
#include "TH2I.h"
#include "TH2F.h"

#include "StiHify/StiHifyEvent.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiRootIO/StiHistContainer.h"
#include "StiRootIO/TStiKalmanTrackNode.h"


class StiHifyHistContainer : public StiHistContainer
{
public:

   StiHifyHistContainer(const char* name, TDirectory* motherDir = 0, Option_t* option = "");

   void FillHists(const StiHifyEvent &event, StiNodeHitStatus hitStatus=StiNodeHitStatus::Any, const std::set<std::string> *volumeList=0);
   virtual void FillDerivedHists();

protected:

   virtual void BookHists();
   void FillHists(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList=0);
   void FillHistsHitsAccepted(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList=0);
   void FillHistsHitsRejected(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList=0);

   TH1I* hDistClosest2AcceptedHit;
   TH1I* hPullClosestHit1D;
   TH2I* hPullClosestHit2D;
   TH2I* hPullCandidateHits2D;
   TH1I* hChi2CandidateHits;
   TH1I* hCountCandidateHits;
   TH2F* hActiveLayerCounts;
};

#endif
