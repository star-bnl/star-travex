#include <iostream>
#include <string>
#include <deque>

#include "TApplication.h"
#include "TColor.h"
#include "TEveBrowser.h"
#include "TEveGeoShapeExtract.h"
#include "TEveGeoNode.h"
#include "TEveManager.h"
#include "TEveTrans.h"
#include "TFile.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoPhysicalNode.h"
#include "TList.h"
#include "TROOT.h"

#include "geo/common.h"


TEveGeoShapeExtract* DumpShapeTree(TGeoNode*            geoNode,
                                   TEveGeoShapeExtract* parent,
                                   bool                 leafs_only)
{
   static const TEveException eh("DumpShapeTree ");

   static std::deque<std::string> currStack;

   TGeoVolume* tvolume = 0;
   TGeoShape*  tshape  = 0;

   if (geoNode == 0)
   {
      Info(eh, "Null TGeoNode for TEveGeoNode '%s': assuming it's a holder and descending.", geoNode->GetName());
   }
   else
   {
      tvolume = geoNode->GetVolume();
      if (tvolume == 0) {
         Warning(eh, "Null TGeoVolume for TEveGeoNode '%s'; skipping its sub-tree.\n", geoNode->GetName());
         return 0;
      }

      tshape  = tvolume->GetShape();
      if (tshape->IsComposite())
      {
         Warning(eh, "Failed extracting CSG tesselation TEveGeoNode '%s'; skipping its sub-tree.\n", geoNode->GetName());
         return 0;
      }
   }

   currStack.push_back(geoNode->GetName());

   std::ostream_iterator<std::string>  cout_iter(std::cout, "/");
   std::copy(currStack.begin(), currStack.end(), cout_iter);

   //std::cout << << std::endl;
   std::cout << " currLevel: " << currStack.size();

   // Keep track of current level with respect to the first call passing the top node
   int currLevel = currStack.size();

   //for (auto& stack_name : currStack)
   //{
   //   std::cout << stack_name << "/";
   //}
   //std::cout << std::endl;

   // transformation
   TEveTrans trans;

   if (parent)
      trans.SetFromArray(parent->GetTrans());

   if (geoNode)
   {
      TGeoMatrix     *gm = geoNode->GetMatrix();
      const Double_t *rm = gm->GetRotationMatrix();
      const Double_t *tv = gm->GetTranslation();
      TEveTrans t;
      t(1,1) = rm[0]; t(1,2) = rm[1]; t(1,3) = rm[2];
      t(2,1) = rm[3]; t(2,2) = rm[4]; t(2,3) = rm[5];
      t(3,1) = rm[6]; t(3,2) = rm[7]; t(3,3) = rm[8];
      t(1,4) = tv[0]; t(2,4) = tv[1]; t(3,4) = tv[2];
      trans *= t;
   }

   TEveGeoShapeExtract* gse = new TEveGeoShapeExtract(geoNode->GetName(), geoNode->GetTitle());
   gse->SetTrans(trans.Array());
   int  ci = 0;
   char transp = 0;
   if (tvolume) {
      ci = tvolume->GetLineColor();
      transp = tvolume->GetTransparency();
   }
   TColor* c = gROOT->GetColor(ci);
   float rgba[4] = {1, 0, 0, 1.0f - transp/100.0f};
   if (c) {
      rgba[0] = c->GetRed();
      rgba[1] = c->GetGreen();
      rgba[2] = c->GetBlue();
   }
   gse->SetRGBA(rgba);
   rgba[3] = 1;
   c = gROOT->GetColor(TColor::GetColorDark(ci));
   if (c) {
      rgba[0] = c->GetRed();
      rgba[1] = c->GetGreen();
      rgba[2] = c->GetBlue();
   }
   gse->SetRGBALine(rgba);

   // Keep default extract line color --> black.
   bool rnr     = false; //geoNode->IsVisible();
   bool rnr_els = false; //geoNode->IsVisDaughters();

   //if (tvolume) {
   //   rnr     = rnr     && tvolume->IsVisible();
   //   rnr_els = rnr_els && tvolume->IsVisDaughters();
   //}

   std::string currNodeName( geoNode->GetName() );

   if ( geoNode->IsVisible() ) {
      rnr = true;
      rnr_els = true;
      std::cout<< " - match" << std::endl;
   } else {
      std::cout<< " - no match" << std::endl;
      currStack.pop_back();
      return 0;
   }

   gse->SetRnrSelf    (rnr);
   gse->SetRnrElements(rnr_els);
   gse->SetRnrFrame   (false);
   gse->SetMiniFrame  (false);

   TObjArray *daughters = tvolume->GetNodes();

   gse->SetShape((leafs_only && daughters && daughters->GetEntries()) ? 0 : tshape);

   if (daughters && daughters->GetEntries())
   {
      TList* ele = new TList();
      gse->SetElements(ele);
      gse->GetElements()->SetOwner(true);

      ROOT::Detail::TTypedIter<TGeoNode> next(daughters);

      while (auto daughterNode = next())
      {
         DumpShapeTree(daughterNode, gse, leafs_only);
      }
   }


   if (parent)
      parent->GetElements()->Add(gse);

   currStack.pop_back();
   return gse;
}
