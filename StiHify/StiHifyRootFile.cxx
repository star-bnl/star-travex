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
   TFile(prgOpts.GetOutFileName().c_str(), option, ftitle, compress), mDirs(),
   fPrgOptions(prgOpts)
{
   Info("StiHifyRootFile", "Created ROOT file: %s", GetName());

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


Int_t StiHifyRootFile::Write(const char* name, Int_t opt, Int_t bufsiz)
{
   Info("Write", "%s", GetName());

   return TFile::Write(name, opt, bufsiz);
}


Int_t StiHifyRootFile::Write(const char* name, Int_t opt, Int_t bufsiz) const
{
   return TFile::Write(name, opt, bufsiz);
}


void StiHifyRootFile::Close(Option_t *option)
{
   TFile::Close(option);
}


void StiHifyRootFile::SaveAllAs(std::string prefix)
{
   namespace fs = boost::filesystem;

   if (fs::create_directories(prefix))
      Info("SaveAllAs", "Created dir: %s", prefix.c_str());
   else
      Warning("SaveAllAs", "Perhaps dir already exists: %s", prefix.c_str());

   for (TDirMapConstIter iDir=mDirs.begin() ; iDir!=mDirs.end(); ++iDir)
   {
      std::string  dirName = iDir->first;
      StiHifyHistContainer *container = static_cast<StiHifyHistContainer*> (iDir->second);

      if (!container) {
         Error("SaveAllAs", "No container/directory found for key %s. Skipping...", dirName.c_str());
         continue;
      }

      std::string path = prefix + "/" + dirName;

      if (gSystem->mkdir(path.c_str()) < 0)
         Warning("SaveAllAs", "Perhaps dir already exists: %s", path.c_str());
      else
         Info("SaveAllAs", "Created dir: %s", path.c_str());

      container->SaveAllAs(path);
   }
}
