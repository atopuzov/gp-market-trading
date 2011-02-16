/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef IfThenElse_hpp
#define IfThenElse_hpp

#include "beagle/GP.hpp"
#include <string>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

class IfThenElse : public Beagle::GP::Primitive {

public:

  typedef Beagle::AllocatorT<IfThenElse,Beagle::GP::Primitive::Alloc> Alloc;
  typedef Beagle::PointerT<IfThenElse,Beagle::GP::Primitive::Handle> Handle;
  typedef Beagle::ContainerT<IfThenElse,Beagle::GP::Primitive::Bag> Bag;

  explicit IfThenElse();
  virtual ~IfThenElse() { }

#ifdef BEAGLE_HAVE_RTTI
  virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
  virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

  virtual void                  execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // IfThenElse_hpp
