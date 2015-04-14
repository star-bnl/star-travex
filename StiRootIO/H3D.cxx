#include "StiScan/H3D.h"

#include "TROOT.h"
#include "TH2D.h"
#include "TError.h"
#include "TMath.h"

ClassImp(H3D)


TH2D *H3D::DoProject2D(const char* name, const char * title, TAxis* projX, TAxis* projY,  
                    bool computeErrors, bool originalRange,
                    bool useUF, bool useOF) const
{
   // internal method performing the projection to a 2D histogram
   // called from TH3::Project3D 

   TH2D *h2 = 0;

   // Get range to use as well as bin limits
   Int_t ixmin = projX->GetFirst();
   Int_t ixmax = projX->GetLast();
   Int_t iymin = projY->GetFirst();
   Int_t iymax = projY->GetLast();
   if (ixmin == 0 && ixmax == 0) { ixmin = 1; ixmax = projX->GetNbins(); }
   if (iymin == 0 && iymax == 0) { iymin = 1; iymax = projY->GetNbins(); }
   Int_t nx = ixmax-ixmin+1;
   Int_t ny = iymax-iymin+1;

   // Create the histogram, either reseting a preexisting one 
   //  or creating one from scratch.
   // Does an object with the same name exists?
   TObject *h2obj = gROOT->FindObject(name);
   if (h2obj && h2obj->InheritsFrom(TH1::Class())) {
      if ( h2obj->IsA() != TH2D::Class() ) { 
         Error("DoProject2D","Histogram with name %s must be a TH2D and is a %s",name,h2obj->ClassName());
         return 0; 
      }
      h2 = (TH2D*)h2obj;
      // reset histogram and its axes
      h2->Reset();
      const TArrayD *xbins = projX->GetXbins();
      const TArrayD *ybins = projY->GetXbins();
      if ( originalRange ) {
         h2->SetBins(projY->GetNbins(),projY->GetXmin(),projY->GetXmax()
                     ,projX->GetNbins(),projX->GetXmin(),projX->GetXmax());
         // set bins for mixed axis do not exists - need to set afterwards the variable bins
         if (ybins->fN != 0) 
            h2->GetXaxis()->Set(projY->GetNbins(),&ybins->fArray[iymin-1]);
         if (xbins->fN != 0)
            h2->GetYaxis()->Set(projX->GetNbins(),&xbins->fArray[ixmin-1]);
      } else {
         h2->SetBins(ny,projY->GetBinLowEdge(iymin),projY->GetBinUpEdge(iymax)
                     ,nx,projX->GetBinLowEdge(ixmin),projX->GetBinUpEdge(ixmax));
         if (ybins->fN != 0) 
            h2->GetXaxis()->Set(ny,&ybins->fArray[iymin-1]);
         if (xbins->fN != 0) 
            h2->GetYaxis()->Set(nx,&xbins->fArray[ixmin-1]);
      }
   }

      
   if (!h2) { 
      const TArrayD *xbins = projX->GetXbins();
      const TArrayD *ybins = projY->GetXbins();
      if ( originalRange )
      {
         if (xbins->fN == 0 && ybins->fN == 0) {
            h2 = new TH2D(name,title,projY->GetNbins(),projY->GetXmin(),projY->GetXmax()
                          ,projX->GetNbins(),projX->GetXmin(),projX->GetXmax());
         } else if (ybins->fN == 0) {
            h2 = new TH2D(name,title,projY->GetNbins(),projY->GetXmin(),projY->GetXmax()
                          ,projX->GetNbins(),&xbins->fArray[ixmin-1]);
         } else if (xbins->fN == 0) {
            h2 = new TH2D(name,title,projY->GetNbins(),&ybins->fArray[iymin-1]
                          ,projX->GetNbins(),projX->GetXmin(),projX->GetXmax());
         } else {
            h2 = new TH2D(name,title,projY->GetNbins(),&ybins->fArray[iymin-1],projX->GetNbins(),&xbins->fArray[ixmin-1]);
         }
      } else {
         if (xbins->fN == 0 && ybins->fN == 0) {
            h2 = new TH2D(name,title,ny,projY->GetBinLowEdge(iymin),projY->GetBinUpEdge(iymax)
                          ,nx,projX->GetBinLowEdge(ixmin),projX->GetBinUpEdge(ixmax));
         } else if (ybins->fN == 0) {
            h2 = new TH2D(name,title,ny,projY->GetBinLowEdge(iymin),projY->GetBinUpEdge(iymax)
                          ,nx,&xbins->fArray[ixmin-1]);
         } else if (xbins->fN == 0) {
            h2 = new TH2D(name,title,ny,&ybins->fArray[iymin-1]
                          ,nx,projX->GetBinLowEdge(ixmin),projX->GetBinUpEdge(ixmax));
         } else {
            h2 = new TH2D(name,title,ny,&ybins->fArray[iymin-1],nx,&xbins->fArray[ixmin-1]);
         }
      }
   }

   h2->SetLineColor(this->GetLineColor());
   h2->SetFillColor(this->GetFillColor());
   h2->SetMarkerColor(this->GetMarkerColor());
   h2->SetMarkerStyle(this->GetMarkerStyle());

   // Activate errors
   if ( computeErrors) h2->Sumw2();

   // Set references to the axis, so that the bucle has no branches.
   TAxis* out = 0;
   if ( projX != GetXaxis() && projY != GetXaxis() ) {
      out = GetXaxis();
   } else if ( projX != GetYaxis() && projY != GetYaxis() ) {
      out = GetYaxis();
   } else {
      out = GetZaxis();
   }

   Int_t *refX = 0, *refY = 0, *refZ = 0;
   Int_t ixbin, iybin, outbin;
   if ( projX == GetXaxis() && projY == GetYaxis() ) { refX = &ixbin;  refY = &iybin;  refZ = &outbin; }
   if ( projX == GetYaxis() && projY == GetXaxis() ) { refX = &iybin;  refY = &ixbin;  refZ = &outbin; }
   if ( projX == GetXaxis() && projY == GetZaxis() ) { refX = &ixbin;  refY = &outbin; refZ = &iybin;  }
   if ( projX == GetZaxis() && projY == GetXaxis() ) { refX = &iybin;  refY = &outbin; refZ = &ixbin;  }
   if ( projX == GetYaxis() && projY == GetZaxis() ) { refX = &outbin; refY = &ixbin;  refZ = &iybin;  }
   if ( projX == GetZaxis() && projY == GetYaxis() ) { refX = &outbin; refY = &iybin;  refZ = &ixbin;  }
   R__ASSERT (refX != 0 && refY != 0 && refZ != 0); 

   // Fill the projected histogram excluding underflow/overflows if considered in the option
   // if specified in the option (by default they considered)
   Double_t totcont  = 0;

   Int_t outmin = out->GetFirst(); 
   Int_t outmax = out->GetLast(); 
   // GetFirst(), GetLast() can return (0,0) when the range bit is set artifically (see TAxis::SetRange)
   if (outmin == 0 && outmax == 0) { outmin = 1; outmax = out->GetNbins(); }
   // correct for underflow/overflows
   if (useUF && !out->TestBit(TAxis::kAxisRange) )  outmin -= 1; 
   if (useOF && !out->TestBit(TAxis::kAxisRange) )  outmax += 1; 

   for (ixbin=0;ixbin<=1+projX->GetNbins();ixbin++){
      if ( projX->TestBit(TAxis::kAxisRange) && ( ixbin < ixmin || ixbin > ixmax )) continue;
      Int_t ix = h2->GetYaxis()->FindBin( projX->GetBinCenter(ixbin) );

      for (iybin=0;iybin<=1+projY->GetNbins();iybin++){
         if ( projY->TestBit(TAxis::kAxisRange) && ( iybin < iymin || iybin > iymax )) continue;
         Int_t iy = h2->GetXaxis()->FindBin( projY->GetBinCenter(iybin) );

         Double_t cont = 0; 
         Double_t err2 = 0;

         // loop on the bins to be integrated (outbin should be called inbin)
         for (outbin = outmin; outbin <= outmax; outbin++){

            Int_t bin = GetBin(*refX,*refY,*refZ);

            // sum the bin contents and errors if needed
            cont += GetBinContent(bin);
            if (computeErrors) { 
               Double_t exyz = GetBinError(bin);
               err2 += exyz*exyz;
            }

         }

         // remember axis are inverted 
         h2->SetBinContent(iy , ix, cont);
         if (computeErrors) h2->SetBinError(iy, ix, TMath::Sqrt(err2) ); 
         // sum all content
         totcont += cont;

      }
   }

   // since we use fill we need to reset and recalculate the statistics (see comment in DoProject1D )
   // or keep original statistics if consistent sumw2  
   bool resetStats = true; 
   double eps = 1.E-12;
   if (IsA() == TH3F::Class() ) eps = 1.E-6;
   if (fTsumw != 0 && TMath::Abs( fTsumw - totcont) <  TMath::Abs(fTsumw) * eps) resetStats = false; 

   bool resetEntries = resetStats; 
   // entries are calculated using underflow/overflow. If excluded entries must be reset
   resetEntries |= !useUF || !useOF; 

   if (!resetStats) {
      Double_t stats[kNstat];
      Double_t oldst[kNstat]; // old statistics
      for (Int_t i = 0; i < kNstat; ++i) { oldst[i] = 0; }
      GetStats(oldst); 
      std::copy(oldst,oldst+kNstat,stats);
      // not that projX refer to Y axis and projX refer to the X axis of projected histogram
      // nothing to do for projection in Y vs X
      if ( projY == GetXaxis() && projX == GetZaxis() ) {  // case XZ
         stats[4] = oldst[7];
         stats[5] = oldst[8];
         stats[6] = oldst[9];
      }
      if ( projY == GetYaxis() ) {
         stats[2] = oldst[4];
         stats[3] = oldst[5]; 
         if ( projX == GetXaxis() )  { // case YX
            stats[4] = oldst[2]; 
            stats[5] = oldst[3];
         }
         if ( projX == GetZaxis() )  { // case YZ
            stats[4] = oldst[7]; 
            stats[5] = oldst[8];
            stats[6] = oldst[10];
         }
      }
      else if  ( projY == GetZaxis() ) {
         stats[2] = oldst[7];
         stats[3] = oldst[8]; 
         if ( projX == GetXaxis() )  { // case ZX
            stats[4] = oldst[2]; 
            stats[5] = oldst[3];
            stats[6] = oldst[9];
         }
         if ( projX == GetYaxis() )  { // case ZY
            stats[4] = oldst[4]; 
            stats[5] = oldst[5];
            stats[6] = oldst[10];
         }
      }
      // set the new statistics 
      h2->PutStats(stats);
   }
   else { 
      // recalculate the statistics
      h2->ResetStats(); 
   }

   if (resetEntries) { 
      // use the effective entries for the entries
      // since this  is the only way to estimate them
      Double_t entries =  h2->GetEffectiveEntries();
      if (!computeErrors) entries = TMath::Floor( entries + 0.5); // to avoid numerical rounding
      h2->SetEntries( entries );  
   }
   else { 
      h2->SetEntries( fEntries );  
   }


   return h2;
}


void H3D::Print(Option_t *option) const
{
   // Print some global quantities for this histogram.
   //
   //  If option "base" is given, number of bins and ranges are also printed
   //  If option "range" is given, bin contents and errors are also printed
   //                     for all bins in the current range (default 1-->nbins)
   //  If option "all" is given, bin contents and errors are also printed
   //                     for all bins including under and overflows.

   printf( "TH1.Print Name  = %s, Entries= %d, Total sum= %g\n",GetName(),Int_t(fEntries),GetSumOfWeights());
   return;
   TString opt = option;
   opt.ToLower();
   Int_t all;
   if      (opt.Contains("all"))   all = 0;
   else if (opt.Contains("range")) all = 1;
   else if (opt.Contains("base"))  all = 2;
   else                            return;

   Int_t bin, binx, biny, binz;
   Int_t firstx=0,lastx=0,firsty=0,lasty=0,firstz=0,lastz=0;
   if (all == 0) {
      lastx  = fXaxis.GetNbins()+1;
      if (fDimension > 1) lasty = fYaxis.GetNbins()+1;
      if (fDimension > 2) lastz = fZaxis.GetNbins()+1;
   } else {
      firstx = fXaxis.GetFirst(); lastx  = fXaxis.GetLast();
      if (fDimension > 1) {firsty = fYaxis.GetFirst(); lasty = fYaxis.GetLast();}
      if (fDimension > 2) {firstz = fZaxis.GetFirst(); lastz = fZaxis.GetLast();}
   }

   if (all== 2) {
      printf("          Title = %s\n", GetTitle());
      printf("          NbinsX= %d, xmin= %g, xmax=%g", fXaxis.GetNbins(), fXaxis.GetXmin(), fXaxis.GetXmax());
      if( fDimension > 1) printf(", NbinsY= %d, ymin= %g, ymax=%g", fYaxis.GetNbins(), fYaxis.GetXmin(), fYaxis.GetXmax());
      if( fDimension > 2) printf(", NbinsZ= %d, zmin= %g, zmax=%g", fZaxis.GetNbins(), fZaxis.GetXmin(), fZaxis.GetXmax());
      printf("\n");
      return;
   }

   Double_t w,e;
   Double_t x,y,z;
   if (fDimension == 1) {
      for (binx=firstx;binx<=lastx;binx++) {
         x = fXaxis.GetBinCenter(binx);
         w = GetBinContent(binx);
         e = GetBinError(binx);
         if(fSumw2.fN) printf(" fSumw[%d]=%g, x=%g, error=%g\n",binx,w,x,e);
         else          printf(" fSumw[%d]=%g, x=%g\n",binx,w,x);
      }
   }
   if (fDimension == 2) {
      for (biny=firsty;biny<=lasty;biny++) {
         y = fYaxis.GetBinCenter(biny);
         for (binx=firstx;binx<=lastx;binx++) {
            bin = GetBin(binx,biny);
            x = fXaxis.GetBinCenter(binx);
            w = GetBinContent(bin);
            e = GetBinError(bin);
            if (!w & !e) continue;
            if(fSumw2.fN) printf(" fSumw[%d][%d]=%g, x=%g, y=%g, error=%g\n",binx,biny,w,x,y,e);
            else          printf(" fSumw[%d][%d]=%g, x=%g, y=%g\n",binx,biny,w,x,y);
         }
      }
   }
   if (fDimension == 3) {
      for (binz=firstz;binz<=lastz;binz++) {
         z = fZaxis.GetBinCenter(binz);
         for (biny=firsty;biny<=lasty;biny++) {
            y = fYaxis.GetBinCenter(biny);
            for (binx=firstx;binx<=lastx;binx++) {
               bin = GetBin(binx,biny,binz);
               x = fXaxis.GetBinCenter(binx);
               w = GetBinContent(bin);
               e = GetBinError(bin);
               if (!w & !e) continue;
               if(fSumw2.fN) printf(" fSumw[%d][%d][%d]=%g, x=%g, y=%g, z=%g, error=%g\n",binx,biny,binz,w,x,y,z,e);
               else          printf(" fSumw[%d][%d][%d]=%g, x=%g, y=%g, z=%g\n",binx,biny,binz,w,x,y,z);
            }
         }
      }
   }
}
