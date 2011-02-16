/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "LessThan.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;

// Ime LT
LessThan::LessThan() :
  Beagle::GP::Primitive(2, "LT")
{ }

#ifdef BEAGLE_HAVE_RTTI

// Argumenti tipa Double
const std::type_info* LessThan::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
  Beagle_AssertM(inN < 2);
  return &typeid(Double);
}

// Povratna vrjednost boolean
const std::type_info* LessThan::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Bool);
}

#endif // BEAGLE_HAVE_RTTI

// Usporeduje 2 Double vrijednosti (manje od)
// Vraca boolean vrjednost 
void LessThan::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
  Bool& lResult = castObjectT<Bool&>(outDatum);
  Double lArgs[2];
  getArguments(lArgs, sizeof(Double), ioContext);
  lResult = lArgs[0].getWrappedValue() < lArgs[1].getWrappedValue();
}




