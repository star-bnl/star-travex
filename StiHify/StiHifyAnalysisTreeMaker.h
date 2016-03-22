#ifndef StiHifyAnalysisTreeMaker_h
#define StiHifyAnalysisTreeMaker_h

#include <set>
#include <string>

#include "TH1I.h"
#include "TH2I.h"
#include "TH2F.h"
#include "TTree.h"

#include "StiHify/StiHifyEvent.h"
#include "StiHify/StiHifyPrgOptions.h"
#include "StiRootIO/StiHistContainer.h"
#include "StiRootIO/TStiKalmanTrackNode.h"


class StiHifyAnalysisTreeMaker
{
public:

    StiHifyAnalysisTreeMaker(const StiHifyPrgOptions& );

    struct errorInfo_t {
      float errorMag;
      float pull;
      float residual;
      float eta;
      float pT;
      float phi;
      float z;
    };

    errorInfo_t errorInfo;
    errorInfo_t errorInfoAcc;
    errorInfo_t errorInfoRej;

    void FillTree(const StiHifyEvent &event, StiNodeHitStatus hitStatus=StiNodeHitStatus::Any, const std::set<std::string> *volumeList=0);
    void FillTree(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList, errorInfo_t &eI );
    virtual void createTree();
    void writeTree();
    void clearTreeStructs();

protected:

    // These are only aliases to created tree
    TTree* aTree;

private:

   const StiHifyPrgOptions& fPrgOptions; ///< Command line arguments and options requested by the user
};

#endif
