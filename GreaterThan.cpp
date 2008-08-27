#include "beagle/GP.hpp"
#include "GreaterThan.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;

// Ime GT
GreaterThan::GreaterThan() :
  Beagle::GP::Primitive(2, "GT")
{ }

#ifdef BEAGLE_HAVE_RTTI

const std::type_info* GreaterThan::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
  Beagle_AssertM(inN < 2);
  return &typeid(Double);
}

const std::type_info* GreaterThan::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Bool);
}

#endif // BEAGLE_HAVE_RTTI

// Usporeduje 2 Double vrijednosti (vece od)
// Vraca boolean vrjednost
void GreaterThan::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
  Bool& lResult = castObjectT<Bool&>(outDatum);
  Double lArgs[2];
  getArguments(lArgs, sizeof(Double), ioContext);
  lResult = lArgs[0].getWrappedValue() > lArgs[1].getWrappedValue();
}




