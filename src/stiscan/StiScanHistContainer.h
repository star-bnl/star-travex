#ifndef StiScanHistContainer_h
#define StiScanHistContainer_h

#include <set>
#include <string>

#include "travex/HistContainer.h"

#include "stiscan/StiScanEvent.h"
#include "stiscan/StiScanPrgOptions.h"
#include "stirootio/Profile2D.h"
#include "stirootio/Profile3D.h"
#include "stirootio/TStiKalmanTrack.h"
#include "gearootio/TGeaEvent.h"


class StiScanHistContainer : public tvx::HistContainer
{
public:

   StiScanHistContainer(StiScanPrgOptions& prgOpts, const char* name, TDirectory* motherDir = 0, bool doProjection=true, Option_t* option = "");

   void FillHists(const StiScanEvent &eventT, const std::set<std::string> *volumeList=0);
   void FillHists(const TGeaEvent &eventG, const std::set<std::string> *volumeList=0);
   virtual void FillDerivedHists();
   void SetZRange(double minZ, double maxZ) { mNodeZMin = minZ; mNodeZMax = maxZ; }
   double GetZMin() const { return mNodeZMin; }
   double GetZMax() const { return mNodeZMax; }
   void SetRRange(float minR, float maxR) { mNodeRMin = minR; mNodeRMax = maxR; }
   float GetRMin() const { return mNodeRMin; }
   float GetRMax() const { return mNodeRMax; }

protected:

   virtual void FillHists(const TStiKalmanTrack &kalmTrack, const std::set<std::string> *volumeList=0);
   void FillHists(const TGeaTrack &trackG, const std::set<std::string> *volumeList=0);

   /// Command line arguments and options requested by the user
   const StiScanPrgOptions& fPrgOptions;

   double  mNodeZMin;
   double  mNodeZMax;
   float   mNodeRMin;
   float   mNodeRMax;

   /// If true will create integral projections of 2D profiles instead of
   /// creating 1D profiles with bin averages
   bool  mDoProjection;

   Profile3D* hNStepsVsPhiVsRVsZ;
   Profile3D* hELossVsPhiVsRVsZ;
   Profile3D* hELossVsXVsYVsZ;
   Profile3D* hDensityVsPhiVsRVsZ;
   Profile3D* hRelRadLengthVsPhiVsRVsZ;

private:

   void BookHists();
   void InitRange();
};

#endif
