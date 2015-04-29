#include "TCanvas.h"

#include "StiRootIO/StiHistContainer.h"
#include "StiRootIO/TStiKalmanTrackNode.h"
#include "StiRootIO/TStiHitProxy.h"


StiHistContainer::StiHistContainer() :
   TDirectoryFile(),
   mHs()
{
}


StiHistContainer::StiHistContainer(const char* name, TDirectory* motherDir, Option_t* option) :
   TDirectoryFile(name, name, option, motherDir),
   mHs()
{
}


StiHistContainer::~StiHistContainer()
{
   while (!mHs.empty()) {
      delete mHs.begin()->second;
      mHs.erase(mHs.begin());
   }
}


void StiHistContainer::SaveAllAs(std::string prefix)
{
   TCanvas canvas("canvas", "canvas", 1400, 600);
   canvas.UseCurrentStyle();
   canvas.SetGridx(true);
   canvas.SetGridy(true);

   HistMapIter iHist = mHs.begin();

   for ( ; iHist!=mHs.end(); ++iHist)
   {
      string objName = iHist->first;
      TH1*   obj     = iHist->second;

      if (!obj) {
         Error("SaveAllAs", "No object found for key %s. Skipping...", objName.c_str());
         continue;
      }

      char* opts = (char*) obj->GetOption();

      if (strstr(opts, "logX")) canvas.SetLogx(true);
      else canvas.SetLogx(false);

      if (strstr(opts, "logY")) canvas.SetLogy(true);
      else canvas.SetLogy(false);

      if (strstr(opts, "logZ")) canvas.SetLogz(true);
      else canvas.SetLogz(false);

      obj->Draw();

      string sFileName = prefix + "/c_" + objName + ".png";
      canvas.SaveAs(sFileName.c_str());
   }
}
