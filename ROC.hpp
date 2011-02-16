/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef ROC_hpp
#define ROC_hpp

#include "beagle/GP.hpp"
#include "sqlite3.h"
#include "Context.hpp"
#include <string>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

class ROC : public Beagle::GP::Primitive {

public:
  typedef Beagle::AllocatorT<ROC,Beagle::GP::Primitive::Alloc> Alloc;
  typedef Beagle::PointerT<ROC,Beagle::GP::Primitive::Handle> Handle;
  typedef Beagle::ContainerT<ROC,Beagle::GP::Primitive::Bag> Bag;

  explicit ROC();
  virtual ~ROC() { }

#ifdef BEAGLE_HAVE_RTTI
  virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
  virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

  virtual void                  execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);
};

#endif // ROC_hpp
