#ifndef ST_MCANALYSISMAKER_H
#define ST_MCANALYSISMAKER_H

#include "TString.h"

class TFile;
class TH3F;
class TNtuple;

class StMcTrack;
class StTrack;
class StGlobalTrack;
class StAssociationMaker;
class StMcEvent;
class StEvent;

#include "StChain/StMaker.h"


class StMcAnalysisMaker : public StMaker
{
  private:
    TFile* mFile;
    bool mIsNtuple;
    TNtuple* mNtuple;
    TNtuple* mTpcNtuple;

    TH3F* hTpcHitsDiffXVsPadrowVsSector;
    TH3F* hTpcHitsDiffYVsPadrowVsSector;
    TH3F* hTpcHitsDiffZVsPadrowVsSector;

    StAssociationMaker* mAssoc;
    const StTrack* findPartner(StMcTrack*, int&);
    const StMcTrack* findPartner(StGlobalTrack*, int&);

    int fillTracks(StMcEvent*,StEvent*);

   TString mOutFileName;
  public:
    StMcAnalysisMaker (const char *name="StMcAnalysisMaker", const char *title="event/StMcAnalysisMaker");

    int Init();
    int Make();
    int Finish();

    void produceNtuple(bool f);
   virtual void setOutFileName(TString in);

  ClassDef(StMcAnalysisMaker, 1)
};

inline void StMcAnalysisMaker::setOutFileName(TString in) { mOutFileName = in;}
inline void StMcAnalysisMaker::produceNtuple(bool f){ mIsNtuple = f;}
#endif
