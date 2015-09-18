#include "TCanvas.h"
#include "TClass.h"
#include "TColor.h"
#include "TPaletteAxis.h"
#include "TROOT.h"

#include "StiRootIO/StiHistContainer.h"

using namespace tvx;

StiHistContainer::StiHistContainer(const char* name, TDirectory* motherDir, Option_t* option) :
   TDirectoryFile(name, name, option, motherDir),
   mHs()
{
}


const TH1* StiHistContainer::FindHist(const std::string& hist_name) const
{
   auto iter = mHs.find(hist_name);
   return ( iter != mHs.end() ) ? iter->second.get() : nullptr;
}


TH1* StiHistContainer::FindHist(const std::string& hist_name)
{
   auto iter = mHs.find(hist_name);
   return ( iter != mHs.end() ) ? iter->second.get() : nullptr;
}


void StiHistContainer::SaveAllAs(std::string prefix)
{
   TCanvas canvas("canvas", "canvas", 1200, 600);
   canvas.UseCurrentStyle();
   canvas.SetGridx(true);
   canvas.SetGridy(true);

   for (auto& iHist : mHs)
   {
      std::string histName = iHist.first;
      std::unique_ptr<TH1>& hist = iHist.second;

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

      // Now check if there are other associated objects like functions and graphs
      TList* sub_list = hist->GetListOfFunctions();
      TIter  next(sub_list);

      while ( TObject *iObj = (TObject*) next() )
      {
         if ( !iObj ) continue;

         if ( ( (TClass*) iObj->IsA() )->InheritsFrom("TLine") )
            iObj->Draw();
      }

      std::string sFileName = prefix + "/c_" + histName + ".png";
      canvas.SaveAs(sFileName.c_str());

      // Restore modified color
      if (strstr(opts, "whit_zro")) {
         color->SetRGB(r, g, b);
      }
   }
}
