#include "StRoot/StiScan/Profile3D.h"

#include <iostream>
#include <exception>
#include "TH1.h"
#include "TH3.h"
#include "TMath.h"
#include "TROOT.h"


class DifferentNumberOfBins: public std::exception {};
class DifferentAxisLimits: public std::exception {};
class DifferentBinLimits: public std::exception {};
class DifferentLabels: public std::exception {};


ClassImp(Profile3D)


Profile3D::Profile3D() : TProfile3D()
{
}


Profile3D::Profile3D(std::string name, std::string title, Int_t nbinsx, Double_t xlow, Double_t xup,
   Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup,
   std::string options) :
   TProfile3D(name.c_str(), title.c_str(), nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup, options.c_str()),
   fBinCumulMode(fNcells)
{
   fBinCumulMode.Reset(1);
   SetOption(options.c_str());
}


void Profile3D::ResetBinCumulMode()
{
   fBinCumulMode.Reset(1);
   fEntries++;
}


Int_t Profile3D::FillAsCumulative(Double_t x, Double_t y, Double_t z, Double_t t)
{
   if (fBuffer) return BufferFill(x,y,z,t,1);

   Int_t bin,binx,biny,binz;

   if (fTmin != fTmax) {
      if (t <fTmin || t> fTmax || TMath::IsNaN(t) ) return -1;
   }

   binx =fXaxis.FindBin(x);
   biny =fYaxis.FindBin(y);
   binz =fZaxis.FindBin(z);
   if (binx <0 || biny <0 || binz<0) return -1;
   bin  = GetBin(binx,biny,binz);
   AddBinContent(bin, t);
   fSumw2.fArray[bin] += (Double_t)t*t;

   // Increase the number of bin entries only if the CumulMode flag is set for this bin
   // One has to ResetBinCumulMode to continue counting bin entries
   if (fBinCumulMode.fArray[bin]) {
      fBinEntries.fArray[bin] += 1;
      fBinCumulMode.fArray[bin] = 0;
   }

   if (fBinSumw2.fN)  fBinSumw2.fArray[bin] += 1;
   if (binx == 0 || binx > fXaxis.GetNbins()) {
      if (!fgStatOverflows) return -1;
   }
   if (biny == 0 || biny > fYaxis.GetNbins()) {
      if (!fgStatOverflows) return -1;
   }
   if (binz == 0 || binz > fZaxis.GetNbins()) {
      if (!fgStatOverflows) return -1;
   }
//printf("x=%g, y=%g, z=%g, t=%g, binx=%d, biny=%d, binz=%d, bin=%d\n",x,y,z,t,binx,biny,binz,bin);
   ++fTsumw;
   ++fTsumw2;
   fTsumwx  += x;
   fTsumwx2 += x*x;
   fTsumwy  += y;
   fTsumwy2 += y*y;
   fTsumwxy += x*y;
   fTsumwz  += z;
   fTsumwz2 += z*z;
   fTsumwxz += x*z;
   fTsumwyz += y*z;
   fTsumwt  += t;
   fTsumwt2 += t*t;
   return bin;
}


Profile2D *Profile3D::DoProjectProfile2D(const char* name, const char * title, TAxis* projX, TAxis* projY,
                                           bool originalRange, bool useUF, bool useOF) const
{
   // internal method to project to a 2D Profile
   // called from TH3::Project3DProfile but re-implemented in case of the TPRofile3D since what is done is different

   // Get the ranges where we will work.
   Int_t ixmin = projX->GetFirst();
   Int_t ixmax = projX->GetLast();
   Int_t iymin = projY->GetFirst();
   Int_t iymax = projY->GetLast();
   if (ixmin == 0 && ixmax == 0) { ixmin = 1; ixmax = projX->GetNbins(); }
   if (iymin == 0 && iymax == 0) { iymin = 1; iymax = projY->GetNbins(); }
   Int_t nx = ixmax-ixmin+1;
   Int_t ny = iymax-iymin+1;

   // Create the projected profiles
   Profile2D *p2 = 0;
   // Create always a new TProfile2D (not as in the case of TH3 projection)

   const TArrayD *xbins = projX->GetXbins();
   const TArrayD *ybins = projY->GetXbins();
   // assume all axis have variable bins or have fixed bins
   if ( originalRange ) {
      if (xbins->fN == 0 && ybins->fN == 0) {
         p2 = new Profile2D(name,title,projY->GetNbins(),projY->GetXmin(),projY->GetXmax()
                             ,projX->GetNbins(),projX->GetXmin(),projX->GetXmax());
      } else {
         p2 = new Profile2D(name,title,projY->GetNbins(),&ybins->fArray[iymin-1],projX->GetNbins(),&xbins->fArray[ixmin-1]);
      }
   } else {
      if (xbins->fN == 0 && ybins->fN == 0) {
         p2 = new Profile2D(name,title,ny,projY->GetBinLowEdge(iymin),projY->GetBinUpEdge(iymax)
                             ,nx,projX->GetBinLowEdge(ixmin),projX->GetBinUpEdge(ixmax));
      } else {
         p2 = new Profile2D(name,title,ny,&ybins->fArray[iymin-1],nx,&xbins->fArray[ixmin-1]);
      }
   }

   // weights
   bool useWeights = (fBinSumw2.fN != 0);
   if (useWeights) p2->Sumw2();

   // make projection in a 3D first
   H3D * h3dW = (H3D*) ProjectionXYZ("h3temp-W","W");
   H3D * h3dN = (H3D*) ProjectionXYZ("h3temp-N","B");

   h3dW->SetDirectory(0); h3dN->SetDirectory(0);

   // note that h3dW is always a weighted histogram - so we need to compute error in the projection
   TAxis * projX_hW = h3dW->GetXaxis();
   TAxis * projX_hN = h3dN->GetXaxis();
   if (projX == GetYaxis() ) {  projX_hW =  h3dW->GetYaxis();  projX_hN =  h3dN->GetYaxis(); }
   if (projX == GetZaxis() ) {  projX_hW =  h3dW->GetZaxis();  projX_hN =  h3dN->GetZaxis(); }
   TAxis * projY_hW = h3dW->GetYaxis();
   TAxis * projY_hN = h3dN->GetYaxis();
   if (projY == GetXaxis() ) {  projY_hW =  h3dW->GetXaxis();  projY_hN =  h3dN->GetXaxis(); }
   if (projY == GetZaxis() ) {  projY_hW =  h3dW->GetZaxis();  projY_hN =  h3dN->GetZaxis(); }

   TH2D * h2W = Profile3D::DoProject2D(*h3dW, "htemp-W","",projX_hW, projY_hW, true, originalRange, useUF, useOF);
   TH2D * h2N = Profile3D::DoProject2D(*h3dN, "htemp-N","",projX_hN, projY_hN, useWeights, originalRange, useUF, useOF);
   h2W->SetDirectory(0); h2N->SetDirectory(0);


   // fill the bin content
   R__ASSERT( h2W->fN == p2->fN );
   R__ASSERT( h2N->fN == p2->fN );
   R__ASSERT( h2W->GetSumw2()->fN != 0); // h2W should always be a weighted histogram since h3dW is weighted
   for (int i = 0; i < p2->fN ; ++i) {
      //std::cout << " proj bin " << i << "  " <<  h2W->fArray[i] << "  " << h2N->fArray[i] << std::endl;
      p2->fArray[i] = h2W->fArray[i];   // array of profile is sum of all values
      p2->GetSumw2()->fArray[i]  = h2W->GetSumw2()->fArray[i];   // array of content square of profile is weight square of the W projected histogram
      p2->SetBinEntries(i, h2N->fArray[i] );
      if (useWeights) p2->GetBinSumw2()->fArray[i] = h2N->GetSumw2()->fArray[i];    // sum of weight squares are stored to compute errors in h1N histogram
   }
   // delete the created histograms
   delete h3dW;
   delete h3dN;
   delete h2W;
   delete h2N;

   // Also we need to set the entries since they have not been correctly calculated during the projection
   // we can only set them to the effective entries
   p2->SetEntries( p2->GetEffectiveEntries() );

   return p2;
}


Profile2D *Profile3D::Project3DProfile(Option_t *option) const
{
   // Project a 3-d histogram into a 2-d profile histograms depending
   // on the option parameter
   // option may contain a combination of the characters x,y,z
   // option = "xy" return the x versus y projection into a TProfile2D histogram
   // option = "yx" return the y versus x projection into a TProfile2D histogram
   // option = "xz" return the x versus z projection into a TProfile2D histogram
   // option = "zx" return the z versus x projection into a TProfile2D histogram
   // option = "yz" return the y versus z projection into a TProfile2D histogram
   // option = "zy" return the z versus y projection into a TProfile2D histogram
   // NB: the notation "a vs b" means "a" vertical and "b" horizontal
   //
   // option = "o" original axis range of the target axes will be
   //   kept, but only bins inside the selected range will be filled.
   //
   // The projection is made for the selected bins only.
   // To select a bin range along an axis, use TAxis::SetRange, eg
   //    h3.GetYaxis()->SetRange(23,56);
   //
   // NOTE 1: The generated histogram is named th3name + "_p" + option
   // eg if the TH3* h histogram is named "myhist", then
   // h->Project3D("xy"); produces a TProfile2D histogram named "myhist_pxy".
   // The following sequence
   //    h->Project3DProfile("xy");
   //    h->Project3DProfile("xy2");
   //  will generate two TProfile2D histograms named "myhist_pxy" and "myhist_pxy2"
   //  So, passing additional characters in the option string one can customize the name.
   //
   //  NOTE 2: If a profile of the same type already exists with compatible axes,
   //  the profile is reset and filled again with the projected contents of the TH3.
   //  In the case of axes incompatibility, an error is reported and a NULL pointer is returned.
   //
   //  NOTE 3: The number of entries in the projected profile is estimated from the number of
   //  effective entries for all the cells included in the projection.
   //
   //  NOTE 4: underflow/overflow are by default excluded from the projection
   //  (Note that this is a different default behavior compared to the projection to an histogram)
   //  To include the underflow and/or overflow use option "UF" and/or "OF"

   TString opt = option; opt.ToLower();
   Int_t pcase = 0;
   TString ptype;
   if (opt.Contains("xy")) { pcase = 4; ptype = "xy"; }
   if (opt.Contains("yx")) { pcase = 5; ptype = "yx"; }
   if (opt.Contains("xz")) { pcase = 6; ptype = "xz"; }
   if (opt.Contains("zx")) { pcase = 7; ptype = "zx"; }
   if (opt.Contains("yz")) { pcase = 8; ptype = "yz"; }
   if (opt.Contains("zy")) { pcase = 9; ptype = "zy"; }

   if (pcase == 0) {
      Error("Project3D","No projection axis specified - return a NULL pointer");
      return 0;
   }
   // do not remove ptype from opt to use later in the projected histo name

   Bool_t useUF = kFALSE;
   if (opt.Contains("uf") ) {
      useUF = kTRUE;
      opt.Remove(opt.Index("uf"),2);
   }
   Bool_t useOF = kFALSE;
   if (opt.Contains("of") ) {
      useOF = kTRUE;
      opt.Remove(opt.Index("of"),2);
   }

   Bool_t originalRange = kFALSE;
   if (opt.Contains('o') ) {
      originalRange = kTRUE;
      opt.Remove(opt.First("o"),1);
   }

   // Create the projected profile
   Profile2D *p2 = 0;
   TString name = GetName();
   TString title = GetTitle();
   name  += "_p";   name  += opt;  // opt may include a user defined name
   title += " profile ";   title += ptype; title += " projection";

   // Call the method with the specific projected axes.
   switch (pcase) {
      case 4:
         // "xy"
         p2 = DoProjectProfile2D(name, title, GetXaxis(), GetYaxis(), originalRange, useUF, useOF);
         p2->GetYaxis()->SetTitle(GetXaxis()->GetTitle());
         p2->GetXaxis()->SetTitle(GetYaxis()->GetTitle());
         break;

      case 5:
         // "yx"
         p2 = DoProjectProfile2D(name, title, GetYaxis(), GetXaxis(), originalRange, useUF, useOF);
         p2->GetYaxis()->SetTitle(GetYaxis()->GetTitle());
         p2->GetXaxis()->SetTitle(GetXaxis()->GetTitle());
         break;

      case 6:
         // "xz"
         p2 = DoProjectProfile2D(name, title, GetXaxis(), GetZaxis(), originalRange, useUF, useOF);
         p2->GetYaxis()->SetTitle(GetXaxis()->GetTitle());
         p2->GetXaxis()->SetTitle(GetZaxis()->GetTitle());
         break;

      case 7:
         // "zx"
         p2 = DoProjectProfile2D(name, title, GetZaxis(), GetXaxis(), originalRange, useUF, useOF);
         p2->GetYaxis()->SetTitle(GetZaxis()->GetTitle());
         p2->GetXaxis()->SetTitle(GetXaxis()->GetTitle());
         break;

      case 8:
         // "yz"
         p2 = DoProjectProfile2D(name, title, GetYaxis(), GetZaxis(), originalRange, useUF, useOF);
         p2->GetYaxis()->SetTitle(GetYaxis()->GetTitle());
         p2->GetXaxis()->SetTitle(GetZaxis()->GetTitle());
         break;

      case 9:
         // "zy"
         p2 = DoProjectProfile2D(name, title, GetZaxis(), GetYaxis(), originalRange, useUF, useOF);
         p2->GetYaxis()->SetTitle(GetZaxis()->GetTitle());
         p2->GetXaxis()->SetTitle(GetYaxis()->GetTitle());
         break;

   }

   return p2;
}


void Profile3D::Print(Option_t *option) const
{
   // Print some global quantities for this histogram.
   //
   //  If option "base" is given, number of bins and ranges are also printed
   //  If option "range" is given, bin contents and errors are also printed
   //                     for all bins in the current range (default 1-->nbins)
   //  If option "all" is given, bin contents and errors are also printed
   //                     for all bins including under and overflows.

   printf( "TH1.Print Name  = %s, Entries= %d, Total sum= %g\n",GetName(),Int_t(fEntries),GetSumOfWeights());
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
         w = RetrieveBinContent(binx);
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
            w = RetrieveBinContent(bin);
            e = GetBinError(bin);
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
               w = RetrieveBinContent(bin);
               e = GetBinError(bin);
               int n = GetBinEntries(bin);
               if (!w && !e) continue;
               if(fSumw2.fN) printf(" fSumw[%d][%d][%d]=%g, x=%g, y=%g, z=%g, error=%g, n=%d\n",binx,biny,binz,w,x,y,z,e, n);
               else          printf(" fSumw[%d][%d][%d]=%g, x=%g, y=%g, z=%g\n",binx,biny,binz,w,x,y,z);
            }
         }
      }
   }
}


TH3D *Profile3D::ProjectionXYZ(const char *name, Option_t *option) const
{
//*-*-*-*-*Project this profile3D into a 3-D histogram along X,Y,Z*-*-*-*-*-*-*
//*-*      =====================================================
//
//   The projection is always of the type TH3D.
//
//   if option "E" is specified, the errors are computed. (default)
//   if option "B" is specified, the content of bin of the returned histogram
//      will be equal to the GetBinEntries(bin) of the profile,
//   if option "C=E" the bin contents of the projection are set to the
//       bin errors of the profile
//   if option "E" is specified  the errors of the projected histogram are computed and set
//      to be equal to the errors of the profile.
//      Option "E" is defined as the default one in the header file.
//   if option "" is specified the histogram errors are simply the sqrt of its content
//   if option "B" is specified, the content of bin of the returned histogram
//      will be equal to the GetBinEntries(bin) of the profile,
//   if option "C=E" the bin contents of the projection are set to the
//       bin errors of the profile
//   if option "W" is specified the bin content of the projected histogram  is set to the
//       product of the bin content of the profile and the entries.
//       With this option the returned histogram will be equivalent to the one obtained by
//       filling directly a TH2D using the 3-rd value as a weight.
//       This option makes sense only for profile filled with all weights =1.
//       When the profile is weighted (filled with weights different than 1) the
//       bin error of the projected histogram (obtained using this option "W") cannot be
//       correctly computed from the information stored in the profile. In that case the
//       obtained histogram contains as bin error square the weighted sum of the square of the
//       profiled observable (TProfile2D::fSumw2[bin] )


   TString opt = option;
   opt.ToLower();
   Int_t nx = fXaxis.GetNbins();
   Int_t ny = fYaxis.GetNbins();
   Int_t nz = fZaxis.GetNbins();
   const TArrayD *xbins = fXaxis.GetXbins();
   const TArrayD *ybins = fYaxis.GetXbins();
   const TArrayD *zbins = fZaxis.GetXbins();

   // Create the projection histogram
   TString pname = name;
   if (pname == "_px") {
      pname = GetName();  pname.Append("_pxyz");
   }
   TH3D *h1 = 0 ;
   if (xbins->fN == 0 && ybins->fN == 0 && zbins->fN == 0)
      h1 = new TH3D(pname,GetTitle(),nx,fXaxis.GetXmin(),fXaxis.GetXmax(),ny,fYaxis.GetXmin(),fYaxis.GetXmax(),nz,fZaxis.GetXmin(),fZaxis.GetXmax());
   else if ( xbins->fN != 0 && ybins->fN != 0 && zbins->fN != 0)
      h1 = new TH3D(pname,GetTitle(),nx,xbins->GetArray(),ny,ybins->GetArray(), nz,zbins->GetArray() );
   else {
      Error("ProjectionXYZ","Histogram has an axis with variable bins and an axis with fixed bins. This case is not cupported - return a null pointer");
      return 0;
   }


   Bool_t computeErrors = kFALSE;
   Bool_t cequalErrors  = kFALSE;
   Bool_t binEntries    = kFALSE;
   Bool_t binWeight     = kFALSE;

   if (opt.Contains("b")) binEntries = kTRUE;
   if (opt.Contains("e")) computeErrors = kTRUE;
   if (opt.Contains("w")) binWeight = kTRUE;
   if (opt.Contains("c=e")) {cequalErrors = kTRUE; computeErrors=kFALSE;}
   if (computeErrors || binWeight || (binEntries && fBinSumw2.fN)  ) h1->Sumw2();

   // Fill the projected histogram
   Int_t bin,binx,biny,binz;
   Double_t cont;
   for (binx =0;binx<=nx+1;binx++) {
      for (biny =0;biny<=ny+1;biny++) {
         for (binz =0;binz<=nz+1;binz++) {
            bin = GetBin(binx,biny,binz);

            if (binEntries)         cont = GetBinEntries(bin);
            else if (cequalErrors)  cont = GetBinError(bin);
            else if (binWeight)     cont = GetBinContent(bin) * GetBinEntries(bin);
            else                    cont = GetBinContent(bin);    // default case

            h1->SetBinContent(bin ,cont);

            // if option E projected histogram errors are same as profile
            if (computeErrors ) h1->SetBinError(bin , GetBinError(bin) );
            // in case of option W bin error is deduced from bin sum of z**2 values of profile
            // this is correct only if the profile is unweighted
            if (binWeight)      h1->GetSumw2()->fArray[bin] = fSumw2.fArray[bin];
            // in case of bin entries and profile is weighted, we need to set also the bin error
            if (binEntries && fBinSumw2.fN ) {
               R__ASSERT(  h1->GetSumw2() );
               h1->GetSumw2()->fArray[bin] =  fBinSumw2.fArray[bin];
            }
         }
      }
   }
   h1->SetEntries(fEntries);
   return h1;
}
