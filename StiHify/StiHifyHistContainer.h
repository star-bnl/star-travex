#ifndef StiHifyHistContainer_h
#define StiHifyHistContainer_h

#include <map>
#include <set>
#include <string>

#include "TDirectoryFile.h"
#include "TProfile2D.h"
#include "TProfile3D.h"

#include "StiHify/StiHifyEvent.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiRootIO/Profile2D.h"
#include "StiRootIO/Profile3D.h"
#include "StiRootIO/TStiKalmanTrack.h"
#include "StiRootIO/TStiKalmanTrackNode.h"

typedef std::map<std::string, TH1*>                    HistMap;
typedef std::map<std::string, TH1*>::iterator          HistMapIter;
typedef std::map<std::string, TH1*>::const_iterator    HistMapConstIter;


class StiHifyHistContainer : public TDirectoryFile
{
public:

   StiHifyHistContainer(const char* name, TDirectory* motherDir = 0, Option_t* option = "");
   ~StiHifyHistContainer();

   void FillHists(const StiHifyEvent &event, StiNodeHitStatus hitStatus=StiNodeHitStatus::Any, const std::set<std::string> *volumeList=0);
   void FillDerivedHists();
   void SaveAllAs(std::string prefix="./");

protected:

   void BookHists();
   void FillHists(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList=0);
   void FillHistsHitsAccepted(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList=0);
   void FillHistsHitsRejected(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList=0);

   HistMap mHs;

   TH1I* hDistClosest2AcceptedHit;
   TH1I* hPullClosestHit1D;
   TH2I* hPullClosestHit2D;
   TH2I* hPullCandidateHits2D;
   TH1I* hChi2CandidateHits;
   TH1I* hCountCandidateHits;
   TH2F* hActiveLayerCounts;
};

#endif
