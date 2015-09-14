#include <string>
#include <vector>
#include <map>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TEfficiency.h"
#include "TStyle.h"

#include "VxEff.h"


TCanvas gVtxEffCanvas("vtx_eff", "Vertex Finding Efficiencies, Std and TMVA ranking", 800, 800);
TLegend gVtxEffLegend(0.6, 0.30, 0.98, 0.50);

const std::map<std::string, std::string> myDefaultVertexFiles = {
   {"STD",  "mytestout.root"},
   {"TMVA", "mytestout.root"}
};


/**
 * Creates and displays histograms with vertex finding efficiency as a function
 * of track multiplicity. The function takes two ROOT file names containing
 * histograms corresponging to standard and TMVA vertex ranks.
 */
int VxEff(const std::map<std::string, std::string> & vtx_file_names)
{

   TH1F *frame = gVtxEffCanvas.DrawFrame(0, 0, 40, 1.1);
   frame->SetTitle("Vertex Finding Efficiencies");
   frame->SetYTitle("Efficiency/Impurity");
   frame->SetXTitle("Reconstractible multiplicity");


   int file_indx = 0;

   for (const auto& key2FileName : vtx_file_names)
   {
      TFile file(key2FileName.second.c_str());

      TH1D *hMcRecMulT    = (TH1D *) file.Get("McRecMulT");

      if (! hMcRecMulT) continue;

      double T = hMcRecMulT->GetEntries();
      TH1D *hMcRecMulAny  = (TH1D *) file.Get("McRecMulAny");  double A = hMcRecMulAny->GetEntries();
      TH1D *hMcRecMulGood = (TH1D *) file.Get("McRecMulGood"); double G = hMcRecMulGood->GetEntries();
      TH1D *hMcRecMulBad  = (TH1D *) file.Get("McRecMulBad");  double B = hMcRecMulBad->GetEntries();

      // Plot the overall efficiency only from the first file
      if (file_indx == 0) {
         TEfficiency *eff_total = new TEfficiency(*hMcRecMulAny, *hMcRecMulT);
         eff_total->SetMarkerColor(3*file_indx + 1);
         eff_total->Draw("same");
         gVtxEffLegend.AddEntry(eff_total, Form("Overall Efficiency (a) = %4.2f", A / T));
      }

      TEfficiency *efficiency = new TEfficiency(*hMcRecMulGood, *hMcRecMulT);
      efficiency->SetMarkerColor(3*file_indx + 2);
      gVtxEffLegend.AddEntry(efficiency, Form("%s Efficiency (b) = %4.2f", key2FileName.first.c_str(), G / T));
      efficiency->Draw("same");

      TEfficiency *impurity = new TEfficiency(*hMcRecMulBad, *hMcRecMulT);
      impurity->SetMarkerColor(3*file_indx + 3);
      gVtxEffLegend.AddEntry(impurity, Form("%s Impurity (c) = %4.2f", key2FileName.first.c_str(), B / T));
      impurity->Draw("same");

      file_indx++;
   }

   gVtxEffLegend.Draw();

   gVtxEffCanvas.SaveAs("vtx_eff.png");
}
