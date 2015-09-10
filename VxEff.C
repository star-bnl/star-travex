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
   const Char_t *M[2] = {"old", "TMVA"};

   TCanvas *c1  = new TCanvas("Eff", "Vertex Finding Efficiencies, old and TMVA ranking", 1200, 800);
   TLegend *l;

   TH1F *frame = c1->cd()->DrawFrame(0, 0, 40, 1.1);
   frame->SetTitle("Vertex Finding Efficiencies");
   frame->SetYTitle("Efficiency/Impurity");
   frame->SetXTitle("Reconstractible multiplicity");
   l = new TLegend(0.4, 0.30, 0.9, 0.50);

   for (Int_t j = 0; j < 2; j++) { // old and TMVA
      TFile file(fileNames[j].c_str());
      TH1D *McRecMulT    = (TH1D *) file.Get("McRecMulT");

      if (! McRecMulT) continue;

      Double_t T = McRecMulT->GetEntries();
      TH1D *McRecMulAny  = (TH1D *) file.Get("McRecMulAny");  Double_t A = McRecMulAny->GetEntries();
      TH1D *McRecMulGood = (TH1D *) file.Get("McRecMulGood"); Double_t G = McRecMulGood->GetEntries();
      TH1D *McRecMulBad  = (TH1D *) file.Get("McRecMulBad");  Double_t B = McRecMulBad->GetEntries();

      if (j == 0) {
         TEfficiency *a = new TEfficiency(*McRecMulAny, *McRecMulT);
         a->SetMarkerColor(3 * j + 1);
         a->Draw("same");
         l->AddEntry(a, Form("Overall Efficiency (a) = %4.2f", A / T));
      }

      TEfficiency *g = new TEfficiency(*McRecMulGood, *McRecMulT);
      g->SetMarkerColor(3 * j + 2);
      l->AddEntry(g, Form("%s Efficiency (b) = %4.2f", M[j], G / T));
      g->Draw("same");
      TEfficiency *b = new TEfficiency(*McRecMulBad, *McRecMulT); b->SetMarkerColor(3 * j + 3); b->Draw("same");
      l->AddEntry(b, Form("%s Impurity (c) = %4.2f", M[j], B / T));
   }

   l->Draw();
}
