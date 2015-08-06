#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TEfficiency.h"
void VxEff() {
#if 1
  /*const Char_t *files[4] = {"PPV_old_rank.root",
			    "PPV_tmva_rank.root",
			    "KFV_old_rank.root",
			    "KFV_tmva_rank.root"}; */
  const Char_t *files[4] = {"PPV2012eff.root",
			    "PPV2012effTMVArank.root",
			    "KFV2012eff.root",
			    "KFV2012effTMVArank.root"};
#else
  const Char_t *files[4] = {"PPVV_MuMcPrV3.root",
			    "PPVV_MuMcPrV3TMVArank.root",
			    "KFV_MuMcPrV3.root",
			    "KFV_MuMcPrV3TMVArank.root"};
#endif
  const Char_t *V[2] = {"PPV","KFV"};
  const Char_t *M[2] = {"old","TMVA"};
  TCanvas *c1  = new TCanvas("Eff","PPV and KFV efficiencies, old and TMVA ranking",1200,800);
  c1->Divide(2,1);
  TLegend *l[2];
  for (Int_t i = 0; i < 2; i++) { // PPV and KFV
    TH1F *frame = c1->cd(i+1)->DrawFrame(0,0,40,1.1);
    frame->SetTitle(Form("%s efficiencies",V[i]));
    frame->SetYTitle("Efficiency/Impurity");
    frame->SetXTitle("Reconstractible multiplicity");
    l[i] = new TLegend(0.4,0.30,0.9,0.50);
    for (Int_t j = 0; j < 2; j++) { // old and TMVA
      TFile *f = new TFile(files[2*i+j]);
      f->cd();
      TH1D *McRecMulT    = (TH1D *) f->Get("McRecMulT"); 
      if (! McRecMulT) continue;
      Double_t T = McRecMulT->GetEntries();
      TH1D *McRecMulAny  = (TH1D *) f->Get("McRecMulAny");  Double_t A = McRecMulAny->GetEntries();
      TH1D *McRecMulGood = (TH1D *) f->Get("McRecMulGood"); Double_t G = McRecMulGood->GetEntries();
      TH1D *McRecMulBad  = (TH1D *) f->Get("McRecMulBad");  Double_t B = McRecMulBad->GetEntries();
      if (j == 0) {
	TEfficiency *a = new TEfficiency(*McRecMulAny,*McRecMulT); 
	a->SetMarkerColor(3*j+1);
	a->Draw("same");
	l[i]->AddEntry(a,Form("Overall Efficiency (a) = %4.2f", A/T)); 
      }
      TEfficiency *g = new TEfficiency(*McRecMulGood,*McRecMulT); 
      g->SetMarkerColor(3*j+2);
      l[i]->AddEntry(g,Form("%s Efficiency (b) = %4.2f",M[j],G/T)); 
      g->Draw("same");
      TEfficiency *b = new TEfficiency(*McRecMulBad,*McRecMulT); b->SetMarkerColor(3*j+3); b->Draw("same");
      l[i]->AddEntry(b,Form("%s Impurity (c) = %4.2f",M[j],B/T)); 
      delete f;
    }
    l[i]->Draw();
  }
}
