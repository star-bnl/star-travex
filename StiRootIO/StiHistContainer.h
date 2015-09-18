#ifndef StiHistContainer_h
#define StiHistContainer_h

#include <map>
#include <set>
#include <string>

#include "TH1.h"
#include "TDirectoryFile.h"


typedef std::map<std::string, TH1*>  HistMap;

class StiHistContainer : public TDirectoryFile
{
public:

   StiHistContainer(const char* name, TDirectory* motherDir=nullptr, Option_t* option="");
   ~StiHistContainer();

   const HistMap& GetHists() const { return mHs; }
   const TH1*     FindHist(const std::string& hist_name) const;
   virtual void FillDerivedHists() = 0;
   void SaveAllAs(std::string prefix="./");

protected:

   virtual void BookHists() = 0;
   TH1* FindHist(const std::string& hist_name);

   HistMap mHs;
};

#endif
