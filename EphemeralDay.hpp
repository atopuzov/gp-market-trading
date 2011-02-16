/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef EphemeralDay_hpp
#define EphemeralDay_hpp

#include "beagle/GP.hpp"
#include <string>


class EphemeralDay : public Beagle::GP::EphemeralT<Beagle::UInt> {

public:

  typedef Beagle::AllocatorT<EphemeralDay,Beagle::GP::EphemeralT<Beagle::UInt>::Alloc> Alloc;
  typedef Beagle::PointerT<EphemeralDay,Beagle::GP::EphemeralT<Beagle::UInt>::Handle> Handle;
  typedef Beagle::ContainerT<EphemeralDay,Beagle::GP::EphemeralT<Beagle::UInt>::Bag> Bag;

  explicit EphemeralDay(Beagle::UInt::Handle inValue=NULL);
  virtual ~EphemeralDay() { }

  virtual Beagle::GP::Primitive::Handle generate(Beagle::string inName, Beagle::GP::Context& ioContext);

};


#endif // EphemeralDay_hpp
