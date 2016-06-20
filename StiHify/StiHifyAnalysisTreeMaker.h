#ifndef StiHifyAnalysisTreeMaker_h
#define StiHifyAnalysisTreeMaker_h

#include "TTree.h"

#include "StiHify/StiHifyEvent.h"
#include "StiHify/StiHifyPrgOptions.h"
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

    void FillTree(const StiHifyEvent &event, StiNodeHitStatus hitStatus=StiNodeHitStatus::Any, bool onlyNodesWithCandidates=false);
    void FillTree(const TStiKalmanTrackNode &trkNode, errorInfo_t &eI );
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
