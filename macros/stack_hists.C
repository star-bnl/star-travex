
/**
 * To produce a plot with overlayed histograms (e.g. hMcRecoVertexDelta in vertex/)
 * existing in all files execute:
 *
 * $ root fileA.root fileB.root fileC.root 'stack_hists.C("vertex/hMcRecoVertexDelta")'
 */

void stack_hists(std::string histName=std::string("vertex/hMcRecoVertexDelta"))
{
   if ( histName.empty() ) {
      Error("stack_hists", "Histogram name must be provided");
      return;
   }

   int colors[] = {kRed, kGreen, kBlue, kYellow, kMagenta, kCyan, kOrange, kSpring, kTeal, kAzure, kViolet, kPink};

   THStack *hists = new THStack("hists", "Overlayed histograms");
   TLegend *legend = new TLegend(0.01, 0.90, 0.99, 0.99);
   legend->SetMargin(0.05);

   gROOT->Macro("style_hists.C");

   TSeqCollection *files = gROOT->GetListOfFiles();

   TIter next(files);
   TFile *iFile;
   int *iColor = colors;

   while ( iFile = dynamic_cast<TFile*>(next()) )
   {
      iFile->ls();
      TH1 *hist = dynamic_cast<TH1*>( iFile->Get(histName.c_str()) );

      if (!hist) {
         Warning("stack_hists", "Histogram '%s' not found in '%s'", histName.c_str(), iFile->GetName());
         continue;
      }

      hist->Print();
      hist->SetLineWidth(3);
      hist->SetLineColor(*(iColor++));
      hists->Add(hist);
      legend->AddEntry(hist, iFile->GetName(), "l");
   }

   TCanvas *canvas = new TCanvas("canvas", "canvas", 1200, 600);
   canvas->UseCurrentStyle();

   if (hists->GetNhists())
      hists->Draw("nostack");
   legend->Draw();

   gSystem->mkdir(histName.c_str(), true);

   canvas->SaveAs( (histName + ".png").c_str() );
}
