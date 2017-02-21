void plot_lambda()
{
   gROOT->Macro("style_hists.C");
   gStyle->SetOptDate(0);
   gStyle->SetOptStat(0);
   gStyle->SetPadLeftMargin(0.10);
   gStyle->SetPadRightMargin(0.05);
   gStyle->SetPadBottomMargin(0.10);
   gStyle->SetPadTopMargin(0.05);
   gStyle->SetTitleOffset(1.2, "y");

   TCanvas *canvas = new TCanvas("canvas", "canvas", 1200, 800);
   canvas->UseCurrentStyle();

   std::string histName("h_plot_lambda");

   TFile *file_blno = TFile::Open("/scratch/smirnovd/public/lamb_PPV_blno/file9.list.hist.root");
   //TFile *file_bl1D = TFile::Open("/scratch/smirnovd/public/lamb_PPV_bl1D/file9.list.hist.root");
   TFile *file_bl3D = TFile::Open("/scratch/smirnovd/public/lamb_PPV_bl3D/file9.list.hist.root");

   TTree *tree_blno = dynamic_cast<TTree*>( file_blno->Get("decayVertexTree") );
   //TTree *tree_bl1D = dynamic_cast<TTree*>( file_bl1D->Get("decayVertexTree") );
   TTree *tree_bl3D = dynamic_cast<TTree*>( file_bl3D->Get("decayVertexTree") );

   tree_blno->Draw("v.mVertices.dl_p/sqrt(v.mPrimaryVertex.positionError.mag2()+0.01)>>h_dlength_blno(25,0,100)","v.mVertices.parent==1","", 1e6, 0);
   //tree_bl1D->Draw("v.mVertices.dl_p/sqrt(v.mPrimaryVertex.positionError.mag2()+0.01)>>h_dlength_bl1D(25,0,100)","v.mVertices.parent==1","", 1e6, 0);
   tree_bl3D->Draw("v.mVertices.dl_p/sqrt(v.mPrimaryVertex.positionError.mag2()+0.01)>>h_dlength_bl3D(25,0,100)","v.mVertices.parent==1","", 1e6, 0);

   //h_dlength_bl1D->SetLineWidth(3);
   //h_dlength_bl1D->SetLineColor(kGreen);

   h_dlength_blno->SetLineWidth(5);
   h_dlength_blno->SetLineColor(kBlue);

   h_dlength_bl3D->SetLineWidth(5);
   h_dlength_bl3D->SetLineColor(kRed);

   //double s_bl1D = h_dlength_bl1D ? h_dlength_bl1D->Integral(1,10) : 0;
   double s_blno = h_dlength_blno->Integral(1,10);
   double s_bl3D = h_dlength_bl3D->Integral(1,10);

   //printf("s: %d, %d, %d\n", s_bl1D, s_blno, s_bl3D);

   //h_dlength_bl1D->Scale(1./s_bl1D);
   h_dlength_blno->Scale(1./s_blno);
   h_dlength_bl3D->Scale(1./s_bl3D);

   h_dlength_blno->SetMinimum(0);

   h_dlength_blno->SetTitle(" ; Decay Length Significance #Sigma; Events/A. U.");

   //h_dlength_bl1D->Draw();
   h_dlength_blno->Draw();
   h_dlength_bl3D->Draw("same");

   TLine* mycut = new TLine(10, 0, 10, 0.17);
   mycut->SetLineColor(kGreen+1);
   mycut->SetLineWidth(4);
   mycut->Draw();

   TText* label_blno = new TText(60, 0.14, "3D fit w/o beamline");
   label_blno->SetTextFont(62);
   label_blno->SetTextSize(0.06);
   label_blno->SetTextColor(kBlue);
   label_blno->Draw();

   TText* label_bl3D = new TText(60, 0.12, "3D fit w/ beamline");
   label_bl3D->SetTextFont(62);
   label_bl3D->SetTextSize(0.06);
   label_bl3D->SetTextColor(kRed);
   label_bl3D->Draw();

   TLatex* tex_significance = new TLatex(30, 0.13, "#Sigma = #frac{L}{#sigma_{L}}");
   tex_significance->SetTextFont(62);
   tex_significance->SetTextSize(0.08);
   tex_significance->SetTextColor(kGreen+1);
   tex_significance->Draw();

   canvas->SaveAs( (histName + ".png").c_str() );

   return;


   tree_blno->Draw("v.mVertices.im_p>>h_mass_blno(20,1.10,1.13)","v.mVertices.parent==1&&(v.mVertices.dl_p/sqrt(v.mPrimaryVertex.positionError.mag2()+0.01)>20)","", 1e6, 0);
   //tree_bl1D->Draw("v.mVertices.im_p>>h_mass_bl1D(20,1.10,1.13)","v.mVertices.parent==1&&(v.mVertices.dl_p/sqrt(v.mPrimaryVertex.positionError.mag2()+0.01)>20)","", 1e6, 0);
   tree_bl3D->Draw("v.mVertices.im_p>>h_mass_bl3D(20,1.10,1.13)","v.mVertices.parent==1&&(v.mVertices.dl_p/sqrt(v.mPrimaryVertex.positionError.mag2()+0.01)>20)","", 1e6, 0);

   //h_mass_bl1D->SetLineWidth(3);
   //h_mass_bl1D->SetLineColor(kGreen);

   h_mass_blno->SetLineWidth(5);
   h_mass_blno->SetLineColor(kBlue);

   h_mass_bl3D->SetLineWidth(5);
   h_mass_bl3D->SetLineColor(kRed);

   //double s_bl1D = h_mass_bl1D->Integral(11,11);
   double s_blno = h_mass_blno->Integral(11,11);
   double s_bl3D = h_mass_bl3D->Integral(11,11);

   //printf("s: %d, %d, %d\n", s_bl1D, s_blno, s_bl3D);

   //h_mass_bl1D->Scale(1./s_bl1D);
   //h_mass_blno->Scale(1./s_blno);
   //h_mass_bl3D->Scale(1./s_bl3D);

   h_mass_bl3D->SetTitle(" ; Invariant Mass, GeV; Events");

   h_mass_bl3D->Draw();
   //h_mass_bl1D->Draw("same");
   h_mass_blno->Draw("same");

   //canvas->SetLogy(true);

   canvas->SaveAs( (histName + ".png").c_str() );
}
