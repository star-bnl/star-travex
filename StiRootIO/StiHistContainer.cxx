#include "TCanvas.h"

#include "StiRootIO/StiHistContainer.h"


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

   for (const std::pair<std::string, TH1*>& iHist : mHs)
   {
      string histName = iHist.first;
      TH1*   hist     = iHist.second;

      if (!hist) {
         Error("SaveAllAs", "No object found for key %s. Skipping...", histName.c_str());
         continue;
      }

      char* opts = (char*) hist->GetOption();

      if (strstr(opts, "logX")) canvas.SetLogx(true);
      else canvas.SetLogx(false);

      if (strstr(opts, "logY")) canvas.SetLogy(true);
      else canvas.SetLogy(false);

      if (strstr(opts, "logZ")) canvas.SetLogz(true);
      else canvas.SetLogz(false);

      hist->Draw();

      string sFileName = prefix + "/c_" + histName + ".png";
      canvas.SaveAs(sFileName.c_str());
   }
}
