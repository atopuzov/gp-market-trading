#include "beagle/GP.hpp"
#include "Eq.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;

// Ime LT
Eq::Eq() :
  Beagle::GP::Primitive(2, "EQ")
{ }


#ifdef BEAGLE_HAVE_RTTI

// Argumenti tipa Double
const std::type_info* Eq::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
  Beagle_AssertM(inN < 2);
  return &typeid(Double);
}

// Povratna vrjednost boolean
const std::type_info* Eq::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Bool);
}

#endif // BEAGLE_HAVE_RTTI

// Usporeduje 2 Double vrijednosti (jednako)
// Vraca boolean vrjednost 
void Eq::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
  Bool& lResult = castObjectT<Bool&>(outDatum);
  Double lArgs[2];
  getArguments(lArgs, sizeof(Double), ioContext);
  lResult = lArgs[0].getWrappedValue() == lArgs[1].getWrappedValue();
}




