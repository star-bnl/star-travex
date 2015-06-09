#include "TCanvas.h"
#include "TColor.h"
#include "TPaletteAxis.h"
#include "TROOT.h"

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
   TCanvas canvas("canvas", "canvas", 1200, 600);
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

      if (strstr(opts, "XX")) canvas.SetLogx(true);
      else canvas.SetLogx(false);

      if (strstr(opts, "XY")) canvas.SetLogy(true);
      else canvas.SetLogy(false);

      if (strstr(opts, "XZ")) canvas.SetLogz(true);
      else canvas.SetLogz(false);

      hist->Draw();

      TColor *color;
      float r, g, b;

      if (strstr(opts, "whit_zro")) {
         hist->GetZaxis()->SetRangeUser(-1,1);
         hist->SetContour(11);
         gPad->Update();
         TPaletteAxis *palette = (TPaletteAxis*) hist->GetListOfFunctions()->FindObject("palette");
         color = gROOT->GetColor( palette->GetValueColor(0) );
         color->GetRGB(r, g, b);
         color->SetRGB(255, 255, 255);
      }

      string sFileName = prefix + "/c_" + histName + ".png";
      canvas.SaveAs(sFileName.c_str());

      // Restore modified color
      if (strstr(opts, "whit_zro")) {
         color->SetRGB(r, g, b);
      }
   }
}
