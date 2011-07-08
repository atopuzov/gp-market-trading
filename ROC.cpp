/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include <beagle/GP.hpp>
#include "ROC.hpp"
#include "util.hpp"
#include "ta.hpp"

#include <sstream>
#include <cmath>
#include <fstream>

using namespace Beagle;

// Name ROC
ROC::ROC() : Beagle::GP::Primitive(1, "ROC")
{	}

#ifdef BEAGLE_HAVE_RTTI

// Argumenti tipa Double
const std::type_info* ROC::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
  Beagle_AssertM(inN < 1);
  return &typeid(UInt);
}

// Povratna vrjednost double
const std::type_info* ROC::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Double);
}

#endif // BEAGLE_HAVE_RTTI

// Calculates the rate of change (ROC)
void ROC::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Double& lResult = castObjectT<Double&>(outDatum);
	UInt lArg;
	get1stArgument(lArg, ioContext); 
	trading::Context& aContext = castObjectT<trading::Context&>(ioContext);

	double close_ndaysago = price_days_ago(aContext.database, aContext.dionica, aContext.datum, lArg);
	double close_today = price_on_day(aContext.database, aContext.dionica, aContext.datum);
	lResult = (close_today/close_ndaysago-(double)1)*(double)100;
	
//	std::cout << "ROC: " << lResult << std::endl;
}
