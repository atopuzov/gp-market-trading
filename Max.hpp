#ifndef Max_hpp
#define Max_hpp

#include "beagle/GP.hpp"
#include "sqlite3.h"
#include "Context.hpp"
#include <string>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

class Max : public Beagle::GP::Primitive {

public:

  typedef Beagle::AllocatorT<Max,Beagle::GP::Primitive::Alloc> Alloc;
  typedef Beagle::PointerT<Max,Beagle::GP::Primitive::Handle> Handle;
  typedef Beagle::ContainerT<Max,Beagle::GP::Primitive::Bag> Bag;

  explicit Max();
  virtual ~Max() { }

#ifdef BEAGLE_HAVE_RTTI
  virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
  virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

  virtual void                  execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};

#endif // Max_hpp
