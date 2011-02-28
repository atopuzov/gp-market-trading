/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "Abs.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;

// Name of the primitive is ABS, 1 argument
Abs::Abs() :
  Beagle::GP::Primitive(1, "ABS")
{ }

#ifdef BEAGLE_HAVE_RTTI

// Argument type: Double
const std::type_info* Abs::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
  Beagle_AssertM(inN < 1);
  return &typeid(Double);
}

// Return type: Double
const std::type_info* Abs::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Double);
}

#endif // BEAGLE_HAVE_RTTI

// Returns the absolute value
void Abs::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Double& lResult = castObjectT<Double&>(outDatum);
	Double lArg;
	double result;
	get1stArgument(lArg, ioContext);
	result = lArg;
	if (result < 0.)
		result*=-1.;
	lResult = result;
}
