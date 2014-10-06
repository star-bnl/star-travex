#include "StRoot/StHftPool/EventT/Profile2D.h"

#include <exception>
#include "TH1.h"
#include "TString.h"
#include "TError.h"

class DifferentNumberOfBins: public std::exception {};
class DifferentAxisLimits: public std::exception {};
class DifferentBinLimits: public std::exception {};
class DifferentLabels: public std::exception {};


ClassImp(Profile2D)


Profile2D::Profile2D() : TProfile2D()
{
}


Profile2D::Profile2D(std::string name, std::string title, Int_t nbinsx, Double_t xlow, Double_t xup,
   Int_t nbinsy, Double_t ylow, Double_t yup, std::string options) :
   TProfile2D(name.c_str(), title.c_str(), nbinsx, xlow, xup, nbinsy, ylow, yup, options.c_str())
{
   SetOption(options.c_str());
}


Profile2D::Profile2D(std::string name, std::string title, Int_t nbinsx, const Double_t* xbins,
   Int_t nbinsy, const Double_t* ybins, std::string options) :
   TProfile2D(name.c_str(), title.c_str(), nbinsx, xbins, nbinsy, ybins, options.c_str())
{
   SetOption(options.c_str());
}


void Profile2D::ResetBinEntries(double val)
{
   for (int iBin=0; iBin<fNcells; ++iBin)
   {
      double bc = GetBinContent(iBin);
      double be = GetBinError(iBin);

      // Skip empty bins
      if (!bc && !be) continue;

      fBinEntries.fArray[iBin] = val;
   }
}


Bool_t Profile2D::Multiply(const Profile2D &prof2)
{
   try {
      CheckConsistency((TH1*) this, (TH1*) &prof2);
   } catch(DifferentNumberOfBins&) {
      Error("Multiply","Attempt to multiply histograms with different number of bins");
      return kFALSE;
   } catch(DifferentAxisLimits&) {
      Warning("Multiply","Attempt to multiply histograms with different axis limits");
   } catch(DifferentBinLimits&) {
      Warning("Multiply","Attempt to multiply histograms with different bin limits");
   } catch(DifferentLabels&) {
      Warning("Multiply","Attempt to multiply histograms with different labels");
   }

   // Loop on bins (including underflows/overflows)
   for (Int_t i = 0; i < fNcells; ++i) {
      Double_t c1 = prof2.GetBinContent(i);
      fArray[i] *= c1;
   }
   return kTRUE;
}


TProfile *Profile2D::ProfileX(const char *name, Int_t firstybin, Int_t lastybin, Option_t *option) const
{
   // *-*-*-*-*Project a 2-D histogram into a profile histogram along X*-*-*-*-*-*
   // *-*      ========================================================
   //
   //   The projection is made from the channels along the Y axis
   //   ranging from firstybin to lastybin included.
   //   The result is a 1D profile which contains the combination of all the considered bins along Y
   //   By default, bins 1 to ny are included
   //   When all bins are included, the number of entries in the projection
   //   is set to the number of entries of the 2-D histogram, otherwise
   //   the number of entries is incremented by 1 for all non empty cells.
   //
   //   The option can also be used to specify the projected profile error type.
   //   Values which can be used are 's', 'i', or 'g'. See TProfile::BuildOptions for details
   //
   //
   return DoProfile(true, name, firstybin, lastybin, option);
}


TProfile *Profile2D::ProfileY(const char *name, Int_t firstxbin, Int_t lastxbin, Option_t *option) const
{
   // *-*-*-*-*Project a 2-D histogram into a profile histogram along X*-*-*-*-*-*
   // *-*      ========================================================
   //
   //   The projection is made from the channels along the X axis
   //   ranging from firstybin to lastybin included.
   //   The result is a 1D profile which contains the combination of all the considered bins along X
   //   By default, bins 1 to ny are included
   //   When all bins are included, the number of entries in the projection
   //   is set to the number of entries of the 2-D histogram, otherwise
   //   the number of entries is incremented by 1 for all non empty cells.
   //
   //   The option can also be used to specify the projected profile error type.
   //   Values which can be used are 's', 'i', or 'g'. See TProfile::BuildOptions for details
   //
   //
   //
   return DoProfile(false, name, firstxbin, lastxbin, option);
}


TProfile * Profile2D::DoProfile(bool onX, const char *name, Int_t firstbin, Int_t lastbin, Option_t *option) const
{
   // implementation of ProfileX or ProfileY for a TProfile2D
   // Do correctly the combination of the bin averages when doing the projection

   TString opt = option;
   opt.ToLower();
   bool originalRange = opt.Contains("o");

   TString expectedName = ( onX ? "_pfx" : "_pfy" );

   TString pname(name);
   if (pname.IsNull() || name == expectedName)
      pname = TString(GetName() ) + expectedName;

   const TAxis& outAxis = ( onX ? fXaxis : fYaxis );
   const TArrayD *bins = outAxis.GetXbins();
   Int_t firstOutBin = outAxis.GetFirst();
   Int_t lastOutBin = outAxis.GetLast();

   TProfile  * p1 = 0;
   // case of fixed bins
   if (bins->fN == 0) {
      if (originalRange)
         p1 =  new TProfile(pname,GetTitle(), outAxis.GetNbins(), outAxis.GetXmin(), outAxis.GetXmax(), opt );
      else
         p1 =  new TProfile(pname,GetTitle(), lastOutBin-firstOutBin+1,
                            outAxis.GetBinLowEdge(firstOutBin),outAxis.GetBinUpEdge(lastOutBin), opt);
   } else {
      // case of variable bins
      if (originalRange )
         p1 = new TProfile(pname,GetTitle(),outAxis.GetNbins(),bins->fArray,opt);
      else
         p1 = new TProfile(pname,GetTitle(),lastOutBin-firstOutBin+1,&bins->fArray[firstOutBin-1],opt);

   }

   if (fBinSumw2.fN) p1->Sumw2();

   // make projection in a 2D first
   TH2D * h2dW = ProjectionXY("h2temp-W","W");
   TH2D * h2dN = ProjectionXY("h2temp-N","B");

   h2dW->SetDirectory(0); h2dN->SetDirectory(0);


   TString opt1 = (originalRange) ? "o" : "";
   TH1D * h1W = (onX) ? h2dW->ProjectionX("h1temp-W",firstbin,lastbin,opt1) : h2dW->ProjectionY("h1temp-W",firstbin,lastbin,opt1);
   TH1D * h1N = (onX) ? h2dN->ProjectionX("h1temp-N",firstbin,lastbin,opt1) : h2dN->ProjectionY("h1temp-N",firstbin,lastbin,opt1);
   h1W->SetDirectory(0); h1N->SetDirectory(0);


   // fill the bin content
   R__ASSERT( h1W->fN == p1->fN );
   R__ASSERT( h1N->fN == p1->fN );
   R__ASSERT( h1W->GetSumw2()->fN != 0); // h1W should always be a weighted histogram since h2dW is
   for (int i = 0; i < p1->fN ; ++i) {
      p1->fArray[i] = h1W->GetBinContent(i);   // array of profile is sum of all values
      p1->GetSumw2()->fArray[i]  = h1W->GetSumw2()->fArray[i];   // array of content square of profile is weight square of the W projected histogram
      p1->SetBinEntries(i, h1N->GetBinContent(i) );
      if (fBinSumw2.fN) p1->GetBinSumw2()->fArray[i] = h1N->GetSumw2()->fArray[i];    // sum of weight squares are stored to compute errors in h1N histogram
   }
   // delete the created histograms
   delete h2dW;
   delete h2dN;
   delete h1W;
   delete h1N;

   // Also we need to set the entries since they have not been correctly calculated during the projection
   // we can only set them to the effective entries
   p1->SetEntries( p1->GetEffectiveEntries() );

   return p1;
}
