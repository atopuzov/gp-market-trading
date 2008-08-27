#ifndef LessThan_hpp
#define LessThan_hpp

#include "beagle/GP.hpp"
#include <string>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI

class LessThan : public Beagle::GP::Primitive {

public:

  typedef Beagle::AllocatorT<LessThan,Beagle::GP::Primitive::Alloc> Alloc;
  typedef Beagle::PointerT<LessThan,Beagle::GP::Primitive::Handle> Handle;
  typedef Beagle::ContainerT<LessThan,Beagle::GP::Primitive::Bag> Bag;

  explicit LessThan();
  virtual ~LessThan() { }

#ifdef BEAGLE_HAVE_RTTI
  virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
  virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

  virtual void                  execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};

#endif // LessThan_hpp
