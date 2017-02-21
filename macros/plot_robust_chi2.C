

void plot_robust_chi2()
{
   int colors[] = {kBlue, kGreen, kCyan, kRed};
   double scales[] = {10, 50, 1000, 100};

	std::string legend_items[] = { "s = 10", "s = 50", "s = 1000", "s = 100" };

   TH2I *dummy_hist = new TH2I("robust_chi2", "Overlayed histograms", 1, 0, 30, 1, 0, 30);

   dummy_hist->SetTitle(" ; #chi^{2}; s (1 - exp(-#chi^{2}/s))");

   TLegend *legend = new TLegend(0.20, 0.60, 0.40, 0.85);

   legend->SetMargin(0.40);

   gROOT->Macro("style_hists.C");
   gStyle->SetOptStat(0);

   TCanvas *canvas = new TCanvas("canvas", "canvas", 1200, 800);
   canvas->UseCurrentStyle();

   dummy_hist->Draw();

   TF1 *chi2 = new TF1("chi2", "x", 0, 30);
	chi2->SetLineWidth(1);
	chi2->Draw("same");

   int *iColor = colors;

   TF1 *robust_chi2 = new TF1("robust_chi2", "[0]*(1 - exp(-x/[0]))", 0, 30);

 
   for (int iScale = 0; iScale < 4; ++iScale)
   {
      double scale = scales[iScale];

      Info("plot_robust_chi2", "Processing scale '%f'", scale);

      robust_chi2->SetParameter(0, scale);
      robust_chi2->Print();
      robust_chi2->SetLineWidth(3);
      robust_chi2->SetLineColor(colors[iScale]);
      robust_chi2->SetMarkerColor(colors[iScale]);

      robust_chi2->DrawCopy("same");

      legend->AddEntry( ((TF1*)robust_chi2->Clone()), legend_items[iScale].c_str(), "l");
   }

   legend->Draw();

   //gSystem->mkdir(dummy_hist->GetName(), true);

   canvas->SaveAs( ( std::string(dummy_hist->GetName()) + ".png").c_str() );
}
