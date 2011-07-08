/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "EMA.hpp"
#include "ta.hpp"
#include "util.hpp"

#include <sstream>
#include <cmath>
#include <fstream>

using namespace Beagle;

// Primitive name: EMA
EMA::EMA() : Beagle::GP::Primitive(1, "EMA")
{	}

#ifdef BEAGLE_HAVE_RTTI

// Arguments: 1 integer
const std::type_info* EMA::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
  Beagle_AssertM(inN < 1);
  return &typeid(UInt);
}

// Return value
const std::type_info* EMA::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Double);
}

#endif // BEAGLE_HAVE_RTTI

// Exponential Moving Average (EMA)
void EMA::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Double& lResult = castObjectT<Double&>(outDatum);
	UInt lArg;
	get1stArgument(lArg, ioContext); 
	trading::Context& aContext = castObjectT<trading::Context&>(ioContext);

	lResult = ema(aContext.database, aContext.dionica, aContext.datum, lArg );
	
	//std::cout << "EMA: " << lResult << std::endl;
}


