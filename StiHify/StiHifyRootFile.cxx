#include <boost/filesystem.hpp>

#include "StiHifyRootFile.h"

#include "TH1.h"

#include "StiHify/StiHifyHistContainer.h"
#include "StiHify/StiHifyRatiosHistContainer.h"
#include "StiScan/StiScanPrgOptions.h"


StiHifyRootFile::StiHifyRootFile(StiScanPrgOptions& prgOpts, Option_t *option, const char *ftitle, Int_t compress) :
   StiRootFile(prgOpts, option, ftitle, compress)
{
   BookHists();
}


void StiHifyRootFile::BookHists()
{

   mDirs["sti_hit_closest"]  = new StiHifyHistContainer("sti_hit_closest",  this);
   mDirs["sti_hit_accepted"] = new StiHifyHistContainer("sti_hit_accepted", this);
   mDirs["sti_hit_rejected"] = new StiHifyHistContainer("sti_hit_rejected", this);
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
   for (const std::pair<std::string, TDirectoryFile*>& iDir : mDirs)
   {
      std::string  dirName = iDir.first;
      StiHistContainer *container = static_cast<StiHistContainer*>(iDir.second);

      if (!container) {
         Error("FillDerivedHists", "No container/directory found for key %s. Skipping...", dirName.c_str());
         continue;
      }

      container->FillDerivedHists();
   }

   StiHifyRatiosHistContainer *ratios;
   mDirs["sti_hit_ratio"] = ratios = new StiHifyRatiosHistContainer("sti_hit_ratio", this);

   TH1* hitsAll
      = static_cast<StiHifyHistContainer*>(mDirs["sti_hit_closest"])->GetHists().find("hActiveLayerCounts")->second;
   TH1* hitsAcc
      = static_cast<StiHifyHistContainer*>(mDirs["sti_hit_accepted"])->GetHists().find("hActiveLayerCounts")->second;

   ratios->CreateRatioHist(hitsAcc, hitsAll);
}
