
/**
 * To produce a plot with overlayed histograms (e.g. hMcRecoVertexDelta in vertex/)
 * existing in all files execute:
 *
 * $ root fileA.root fileB.root fileC.root 'stack_hists.C("vertex/hMcRecoVertexDelta")'
 *
 * The second argument `encl_func_name` can be used to draw an object from the
 * list of enclosed objects of the histogram. E.g.:
 *
 * $ root fileA.root fileB.root fileC.root 'stack_hists.C("event/McRecMulAny_eff", "McRecMulT_clone")'
 *
 */

void stack_hists(std::string histName=std::string("vertex/hMcRecoVertexDelta"),
   std::string encl_func_name = "", double norm=0.)
{
   if ( histName.empty() ) {
      Error("stack_hists", "Histogram name must be provided");
      return;
   }

   int colors[] = {kRed, kBlue, kGreen, kMagenta, kCyan, kOrange, kSpring, kTeal, kAzure, kViolet, kPink};

   THStack *hists = new THStack("hists", "Overlayed histograms");
   TLegend *legend = new TLegend(0.01, 0.80, 0.99, 0.99);
   legend->SetMargin(0.05);

   gROOT->Macro("style_hists.C");

   TCanvas *canvas = new TCanvas("canvas", "canvas", 1200, 800);
   canvas->UseCurrentStyle();

   static std::string draw_opt_same = "";

   TSeqCollection *files = gROOT->GetListOfFiles();

   TIter next(files);
   TFile *iFile;
   int *iColor = colors;

   while ( iFile = dynamic_cast<TFile*>(next()) )
   {
      Info("stack_hists", "Processing file '%s'", iFile->GetName());

      TH1 *hist = dynamic_cast<TH1*>( iFile->Get(histName.c_str()) );

      if (!hist) {
         Warning("stack_hists", "Histogram '%s' not found in '%s'", histName.c_str(), iFile->GetName());
         continue;
      }

      // Check the histogram's enclosed list of objects for an object named
      // 'encl_func_name'
      if ( !encl_func_name.empty() )
      {
         TList* obj_list = hist->GetListOfFunctions();
         TObject* obj = obj_list->FindObject(encl_func_name.c_str());

         if (!obj) {
            Warning("stack_hists", "Could not find '%s' in histogram '%s'", encl_func_name.c_str(), histName.c_str());
            continue;
         }

         Info("stack_hists", "Found %s", obj->GetName());

         if (obj->InheritsFrom("TEfficiency"))
         {
            TEfficiency* effcy = static_cast<TEfficiency*>(obj);
            effcy->SetLineWidth(3);
            effcy->SetLineColor(*(iColor));
            effcy->SetMarkerColor(*(iColor++));
            effcy->Draw(draw_opt_same.c_str());
            draw_opt_same += draw_opt_same.empty() ? " same" : "";

            legend->AddEntry(effcy, iFile->GetName(), "l");
         }

      } else { // Process, i.e. draw, normal histograms
         if (norm > 0)
            hist->Scale( norm/hist->Integral() );
         hist->Print();
         hist->SetLineWidth(3);
         hist->SetLineColor(*(iColor));
         hist->SetMarkerColor(*(iColor++));
         hists->Add(hist);

         legend->AddEntry(hist, iFile->GetName(), "l");
      }
   }

   if (hists->GetNhists())
      hists->Draw("nostack");

   legend->Draw();

   gSystem->mkdir(histName.c_str(), true);

   canvas->SaveAs( (histName + ".png").c_str() );
}
