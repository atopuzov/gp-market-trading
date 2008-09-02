#ifndef ConstantDouble_hpp
#define ConstantDouble_hpp

#include "beagle/GP.hpp"
#include <string>


class ConstantDouble : public Beagle::GP::EphemeralT<Beagle::Double> {

public:

  typedef Beagle::AllocatorT<ConstantDouble,Beagle::GP::EphemeralT<Beagle::Double>::Alloc> Alloc;
  typedef Beagle::PointerT<ConstantDouble,Beagle::GP::EphemeralT<Beagle::Double>::Handle> Handle;
  typedef Beagle::ContainerT<ConstantDouble,Beagle::GP::EphemeralT<Beagle::Double>::Bag> Bag;

  explicit ConstantDouble(Beagle::Double::Handle inValue=NULL);
  virtual ~ConstantDouble() { }

  virtual Beagle::GP::Primitive::Handle generate(Beagle::string inName, Beagle::GP::Context& ioContext);

};


#endif // ConstantDouble_hpp
