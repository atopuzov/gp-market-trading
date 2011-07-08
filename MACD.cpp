/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "MACD.hpp"
#include "ta.hpp"

#include <sstream>
#include <cmath>
#include <fstream>

using namespace Beagle;

// Primitive name: MACD
MACD::MACD() : Beagle::GP::Primitive(0, "MACD")
{	}

#ifdef BEAGLE_HAVE_RTTI

// Return value type: double
const std::type_info* MACD::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Double);
}

#endif // BEAGLE_HAVE_RTTI

// Exponential Moving Average (MACD)
// MACD = 12-day EMA - 26-day EMA
void MACD::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Double& lResult = castObjectT<Double&>(outDatum);
	trading::Context& aContext = castObjectT<trading::Context&>(ioContext);
	
	double ema_12_day = ema(aContext.database, aContext.dionica, aContext.datum, 12 );
	double ema_26_day = ema(aContext.database, aContext.dionica, aContext.datum, 26 );
//	std::cout << "12day: " << ema_12_day << ", 26day: " << ema_26_day << std::endl;

	lResult = ema_12_day - ema_26_day;
	
//	std::cout << "MACD: " << lResult << std::endl;
}


