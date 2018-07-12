#ifndef root_v6_header
#define root_v6_header

#include "RVersion.h"

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)

#include "TCollection.h"

#else

#include <cassert>
#include "TCollection.h"
#include "TClass.h"


namespace ROOT {

namespace Internal {


const TCollection &EmptyCollection();
bool ContaineeInheritsFrom(TClass *cl, TClass *base);

/// @brief Internal help class implmenting an iterator for TRangeDynCast.
template <class Containee> // Containee must derive from TObject.
class TRangeDynCastIterator : public TIter {
   static_assert(std::is_base_of<TObject, Containee>::value, "Containee type must inherit from TObject");


   TRangeDynCastIterator() = default;

public:
   using TIter::TIter;
   TRangeDynCastIterator(const TIter &iter) : TIter(iter) {}

   Containee *operator()() = delete;

   Containee *Next() { return dynamic_cast<Containee *>(TIter::Next()); }
   Containee *operator*() const { return dynamic_cast<Containee *>(TIter::operator*()); }
};

} // namespace Internal

namespace Detail {

/// @brief TTypedIter is a typed version of TIter.
///
/// This requires the collection to contains elements of the type requested
/// (or a derived class).  Any deviation from this expectation
/// will only be caught/reported by an assert in debug builds.
///
/// This is best used with a TClonesArray, for other cases prefered TRangeDynCast.
///
/// The typical use is:
/// ~~~ {.cpp}
///    TTypedIter<TBaseClass> next(tbaseClassClonesArrayPtr);
///    while(auto bcl = next()) {
///       ... use bcl as a TBaseClass*
///    }
/// ~~~ {.cpp}
template <class Containee> // Containee must derive from TObject.
class TTypedIter : public TIter {
   static_assert(std::is_base_of<TObject, Containee>::value, "Containee type must inherit from TObject");

   TTypedIter() = default;

   static Containee *StaticCast(TObject *obj)
   {
      assert(!obj || ROOT::Internal::ContaineeInheritsFrom(obj->IsA(), Containee::Class()));
      return static_cast<Containee *>(obj);
   }

public:
   using TIter::TIter;
   TTypedIter(const TIter &iter) : TIter(iter) {}

   Containee *operator()() { return StaticCast(TIter::Next()); }
   Containee *Next() { return StaticCast(TIter::Next()); }
   Containee *operator*() const { return StaticCast(TIter::operator*()); }
};

/// @brief TRangeStaticCast is an adaptater class that allows the typed iteration
/// through a TCollection.  This requires the collection to contains element
/// of the type requested (or a derived class).  Any deviation from this expectation
/// will only be caught/reported by an assert in debug builds.
///
/// This is best used with a TClonesArray, for other cases prefered TRangeDynCast.
///
/// The typical use is:
/// ~~~ {.cpp}
///    for(auto bcl : TRangeStaticCast<TBaseClass>( *tbaseClassClonesArrayPtr )) {
///        assert(bcl && bcl->IsA()->InheritsFrom(TBaseClass::Class()));
///        ... use bcl as a TBaseClass*
///    }
///    for(auto bcl : TRangeStaticCast<TBaseClass>( tbaseClassClonesArrayPtr )) {
///        assert(bcl && bcl->IsA()->InheritsFrom(TBaseClass::Class()));
///        ... use bcl as a TBaseClass*
///    }
/// ~~~ {.cpp}
template <class T>
class TRangeStaticCast {
   const TCollection &fCollection;

public:
   TRangeStaticCast(const TCollection &col) : fCollection(col) {}
   TRangeStaticCast(const TCollection *col) : fCollection(col != nullptr ? *col : ROOT::Internal::EmptyCollection()) {}

   TTypedIter<T> begin() const { return fCollection.begin(); }
   TTypedIter<T> end() const { return fCollection.end(); }
};

} // namespace Detail
} // namespace ROOT


#endif

#endif
