#include <cmath>
#include <boost/filesystem.hpp>

#include "StiHifyRootFile.h"

#include "TCanvas.h"
#include "TH2S.h"
#include "TProfile2D.h"
#include "TRandom.h"
#include "TSystem.h"

#include "StiHify/StiHifyHistContainer.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiRootIO/TStiKalmanTrackNode.h"


StiHifyRootFile::StiHifyRootFile(StiScanPrgOptions& prgOpts, Option_t *option, const char *ftitle, Int_t compress) :
   StiRootFile(prgOpts, option, ftitle, compress)
{
   BookHists();
}


void StiHifyRootFile::BookHists()
{

   mDirs["sti_hit_closest"] = new StiHifyHistContainer(fPrgOptions, "sti_hit_closest", this);
   mDirs["sti_hit_accepted"] = new StiHifyHistContainer(fPrgOptions, "sti_hit_accepted", this);
   mDirs["sti_hit_rejected"] = new StiHifyHistContainer(fPrgOptions, "sti_hit_rejected", this);
}


void StiHifyRootFile::FillHists(const StiHifyEvent &event, const std::set<std::string> *volumeList)
{
   StiHifyHistContainer* container;

   container = static_cast<StiHifyHistContainer*> (mDirs["sti_hit_closest"]);
   container->FillHists(event, StiNodeHitStatus::Any, volumeList);

   container = static_cast<StiHifyHistContainer*> (mDirs["sti_hit_accepted"]);
   container->FillHists(event, StiNodeHitStatus::Accepted, volumeList);

   container = static_cast<StiHifyHistContainer*> (mDirs["sti_hit_rejected"]);
   container->FillHists(event, StiNodeHitStatus::Rejected, volumeList);
}


void StiHifyRootFile::FillDerivedHists()
{
   for (TDirMapConstIter iDir=mDirs.begin() ; iDir!=mDirs.end(); ++iDir)
   {
      std::string  dirName = iDir->first;
      StiHifyHistContainer *container = (StiHifyHistContainer*) iDir->second;

      if (!container) {
         Error("FillDerivedHists", "No container/directory found for key %s. Skipping...", dirName.c_str());
         continue;
      }

      container->FillDerivedHists();
   }
}
