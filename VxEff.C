#include <iostream>
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


int VxEff(const std::vector<std::string> &vtx_file_keys, const std::vector<std::string> &vtx_file_names)
{
   // Check user's input
   if (vtx_file_keys.size() != vtx_file_names.size()) {
      std::cout << "ERROR: Different number of keys and files" << std::endl;
      return EXIT_FAILURE;
   }

   if (vtx_file_keys.size() > 4) {
      std::cout << "ERROR: Too many histograms to overlay on one plot. Specify at most four files" << std::endl;
      return EXIT_FAILURE;
   }

   std::map<std::string, std::string> myVertexFiles;

   auto fname = vtx_file_names.begin();

   for (auto key=vtx_file_keys.begin(); key!=vtx_file_keys.end(); ++key, ++fname)
   {
      myVertexFiles.insert( std::pair<std::string, std::string>(*key, *fname) );
   }
   
   VxEff(myVertexFiles);
}


/**
 * Creates and displays histograms with vertex finding efficiency as a function
 * of track multiplicity. The function takes two ROOT file names containing
 * histograms corresponging to standard and TMVA vertex ranks.
 */
int VxEff(const std::map<std::string, std::string> & vtx_file_names)
{
   // Check user's input
   if (vtx_file_names.empty()) {
      std::cout << "ERROR: No input files provided" << std::endl;
      return EXIT_FAILURE;
   }

   // Print out user's input
   std::cout << "User provided files with histograms:\n";

   for (const auto& key2FileName : vtx_file_names) {
      std::cout << "key => file: "
                << key2FileName.first << " => " << key2FileName.second << "\n";
   }

   std::cout << std::endl;

   // For efficiency plots we create here the default marker is unscalable dot
   // so, change it
   gStyle->SetMarkerStyle(kFullDotLarge);

   TH1F *frame = gVtxEffCanvas.DrawFrame(0, 0, 40, 1.1);
   frame->SetTitle("Vertex Finding Efficiency");
   frame->SetYTitle("Efficiency/Impurity");
   frame->SetXTitle("Vertex Track Multiplicity");


   int file_indx = 0;

   for (const auto& key2FileName : vtx_file_names)
   {
      TFile file(key2FileName.second.c_str());

      TH1D *hMcRecMulT    = (TH1D *) file.Get("McRecMulT");

      if (!hMcRecMulT) continue;

      TH1D *hMcRecMulAny  = (TH1D*) file.Get("McRecMulAny");
      TH1D *hMcRecMulGood = (TH1D*) file.Get("McRecMulGood");
      TH1D *hMcRecMulBad  = (TH1D*) file.Get("McRecMulBad");

      double T = hMcRecMulT->GetEntries();
      double A = hMcRecMulAny->GetEntries();
      double G = hMcRecMulGood->GetEntries();
      double B = hMcRecMulBad->GetEntries();

      // Plot the overall efficiency only from the first file
      if (file_indx == 0) {
         TEfficiency *eff_total = new TEfficiency(*hMcRecMulAny, *hMcRecMulT);
         eff_total->SetMarkerColor(3*file_indx + 1);
         eff_total->Draw("same p");
         gVtxEffLegend.AddEntry(eff_total, Form("%s Overall Efficiency = %4.2f", key2FileName.first.c_str(), A / T));
      }

      TEfficiency *efficiency = new TEfficiency(*hMcRecMulGood, *hMcRecMulT);
      efficiency->SetMarkerColor(3*file_indx + 2);
      gVtxEffLegend.AddEntry(efficiency, Form("%s Max Rank Efficiency = %4.2f", key2FileName.first.c_str(), G / T));
      efficiency->Draw("same p");

      TEfficiency *impurity = new TEfficiency(*hMcRecMulBad, *hMcRecMulT);
      impurity->SetMarkerColor(3*file_indx + 3);
      gVtxEffLegend.AddEntry(impurity, Form("%s Impurity (c) = %4.2f", key2FileName.first.c_str(), B / T));
      impurity->Draw("same p");

      file_indx++;
   }

   gVtxEffLegend.Draw();

   gVtxEffCanvas.SaveAs("vtx_eff.png");
}
