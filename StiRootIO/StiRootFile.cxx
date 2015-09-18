#include <boost/filesystem.hpp>

#include "StiRootIO/StiRootFile.h"

#include "TSystem.h"
#include "TROOT.h"

#include "src-tools/config.h"

#include "StiRootIO/StiHistContainer.h"
#include "StiRootIO/PrgOptionProcessor.h"

using namespace tvx;

StiRootFile::StiRootFile(PrgOptionProcessor& prgOpts, Option_t *option, const char *ftitle, Int_t compress) :
   TFile(prgOpts.GetOutFileName().c_str(), option, ftitle, compress),
   mDirs(),
   fPrgOptions(prgOpts)
{
   Info("StiRootFile", "Created ROOT file: %s", GetName());

   std::string macroPath = std::string(gROOT->GetMacroPath()) + ":" + stitools::gStiToolsMacrosPath;
   gROOT->SetMacroPath(macroPath.c_str());
}


void StiRootFile::FillDerivedHists()
{
   for (const std::pair<std::string, StiHistContainer*>& iDir : mDirs)
   {
      std::string  dirName = iDir.first;
      StiHistContainer *container = iDir.second;

      if (!container) {
         Error("FillDerivedHists", "No container/directory found for key %s. Skipping...", dirName.c_str());
         continue;
      }

      container->FillDerivedHists();
   }
}


Int_t StiRootFile::Write(const char* name, Int_t opt, Int_t bufsiz)
{
   Info("Write", "%s", GetName());

   return TFile::Write(name, opt, bufsiz);
}


Int_t StiRootFile::Write(const char* name, Int_t opt, Int_t bufsiz) const
{
   return TFile::Write(name, opt, bufsiz);
}


void StiRootFile::Close(Option_t *option)
{
   if (fPrgOptions.SaveGraphics()) {
      SaveAllAs(fPrgOptions.GetOutPrefix());
   }

   TFile::Close(option);
}


void StiRootFile::SaveAllAs(std::string prefix)
{
   gROOT->Macro("style_hists.C");

   namespace fs = boost::filesystem;

   if (fs::create_directories(prefix))
      Info("SaveAllAs", "Created dir: %s", prefix.c_str());
   else
      Warning("SaveAllAs", "Perhaps dir already exists: %s", prefix.c_str());

   for (const std::pair<std::string, StiHistContainer*>& iDir : mDirs)
   {
      std::string  dirName = iDir.first;
      StiHistContainer *container = iDir.second;

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
