#include <string>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TEfficiency.h"


/**
 * Creates and displays histograms with vertex finding efficiency as a function
 * of track multiplicity. The function takes two ROOT file names containing
 * histograms corresponging to standard and TMVA vertex ranks.
 */
void VxEff(std::string fname_vtx_std_rank, std::string fname_vtx_tmva_rank)
{
   std::vector<std::string> fileNames = {fname_vtx_std_rank, fname_vtx_tmva_rank};
   const char *M[2] = {"Std", "TMVA"};

   TCanvas *c1  = new TCanvas("Eff", "Vertex Finding Efficiencies, Std and TMVA ranking", 800, 800);

   TH1F *frame = c1->DrawFrame(0, 0, 40, 1.1);
   frame->SetTitle("Vertex Finding Efficiencies");
   frame->SetYTitle("Efficiency/Impurity");
   frame->SetXTitle("Reconstractible multiplicity");

   TLegend *legend = new TLegend(0.6, 0.30, 0.98, 0.50);

   for (int j = 0; j < 2; j++) { // Std and TMVA
      TFile file(fileNames[j].c_str());
      TH1D *hMcRecMulT    = (TH1D *) file.Get("McRecMulT");

      if (! hMcRecMulT) continue;

      double T = hMcRecMulT->GetEntries();
      TH1D *hMcRecMulAny  = (TH1D *) file.Get("McRecMulAny");  double A = hMcRecMulAny->GetEntries();
      TH1D *hMcRecMulGood = (TH1D *) file.Get("McRecMulGood"); double G = hMcRecMulGood->GetEntries();
      TH1D *hMcRecMulBad  = (TH1D *) file.Get("McRecMulBad");  double B = hMcRecMulBad->GetEntries();

      if (j == 0) {
         TEfficiency *eff_total = new TEfficiency(*hMcRecMulAny, *hMcRecMulT);
         eff_total->SetMarkerColor(3 * j + 1);
         eff_total->Draw("same");
         legend->AddEntry(eff_total, Form("Overall Efficiency (a) = %4.2f", A / T));
      }

      TEfficiency *efficiency = new TEfficiency(*hMcRecMulGood, *hMcRecMulT);
      efficiency->SetMarkerColor(3 * j + 2);
      legend->AddEntry(efficiency, Form("%s Efficiency (b) = %4.2f", M[j], G / T));
      efficiency->Draw("same");

      TEfficiency *impurity = new TEfficiency(*hMcRecMulBad, *hMcRecMulT);
      impurity->SetMarkerColor(3 * j + 3);
      legend->AddEntry(impurity, Form("%s Impurity (c) = %4.2f", M[j], B / T));
      impurity->Draw("same");
   }

   legend->Draw();
}
