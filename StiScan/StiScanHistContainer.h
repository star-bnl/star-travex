#ifndef StiScanHistContainer_h
#define StiScanHistContainer_h

#include <map>
#include <set>
#include <string>

#include "TDirectoryFile.h"
#include "TProfile2D.h"
#include "TProfile3D.h"

#include "StiScan/StiScanEvent.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiScan/TStiKalmanTrack.h"
#include "StiScan/Profile2D.h"
#include "StiScan/Profile3D.h"
#include "StarGenerator/STEP/AgUStep.h"

typedef Event EventG;
typedef Track TrackG;
typedef Step  StepG;

typedef std::map<std::string, TH1*>                    HistMap;
typedef std::map<std::string, TH1*>::iterator          HistMapIter;
typedef std::map<std::string, TH1*>::const_iterator    HistMapConstIter;


class StiScanHistContainer : public TDirectoryFile
{
public:

   StiScanHistContainer(StiScanPrgOptions& prgOpts);
   StiScanHistContainer(StiScanPrgOptions& prgOpts, const char* name, TDirectory* motherDir = 0, bool doProjection=false, Option_t* option = "");
   ~StiScanHistContainer();

   void FillHists(const StiScanEvent &eventT, const std::set<std::string> *volumeList=0);
   void FillHists(const EventG &eventG, const std::set<std::string> *volumeList=0);
   void FillDerivedHists();
   void SaveAllAs(std::string prefix="./");
   void SetZRange(double minZ, double maxZ) { mNodeZMin = minZ; mNodeZMax = maxZ; }
   double GetZMin() { return mNodeZMin; }
   double GetZMax() { return mNodeZMax; }
   void SetRRange(float minR, float maxR) { mNodeRMin = minR; mNodeRMax = maxR; }
   float GetRMin() const { return mNodeRMin; }
   float GetRMax() const { return mNodeRMax; }

protected:

   void BookHists();
   virtual void FillHists(const TStiKalmanTrack &kalmTrack, const std::set<std::string> *volumeList=0);
   void FillHists(const TrackG &trackG, const std::set<std::string> *volumeList=0);

   StiScanPrgOptions& fPrgOptions; ///< Command line arguments and options requested by the user
   HistMap mHs;
   double  mNodeZMin;
   double  mNodeZMax;
   float   mNodeRMin;
   float   mNodeRMax;
   /** If true will create integral projections of 2D profiles instead of creating 1D profiles with bin averages. */
   bool    mDoProjection;

   Profile3D* hNStepsVsPhiVsRVsZ;
   Profile3D* hELossVsPhiVsRVsZ;
   Profile3D* hRelRadLengthVsPhiVsRVsZ;

private:

   void InitRange();

   Profile2D* hNStepsVsPhiVsR_buf;
   Profile2D* hNStepsVsPhiVsR;
};

#endif
