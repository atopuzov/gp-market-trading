/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include <beagle/GP.hpp>
#include "RSI.hpp"
#include "ta.hpp"

#include <sstream>
#include <cmath>
#include <fstream>

using namespace Beagle;

// Ime RSI
RSI::RSI() : Beagle::GP::Primitive(0, "RSI")
{	}

#ifdef BEAGLE_HAVE_RTTI

const std::type_info* RSI::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
	return NULL;
}

// Return vaule type: double
const std::type_info* RSI::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Double);
}

#endif // BEAGLE_HAVE_RTTI

// Relative strength index
// RSI = 100 - 100/(1 + RS*)
// RS = Average of x days' up closes / Average of x days' down closes.	
void RSI::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Double& lResult = castObjectT<Double&>(outDatum);
	trading::Context& aContext = castObjectT<trading::Context&>(ioContext);

	std::pair<double,double> agl = avg_gain_loss(aContext.database, aContext.dionica, aContext.datum, 14);
	double rs = agl.first/agl.second;
	lResult = (double)100 - (double)100/((double)1+rs);
	
//	std::cout << "RSI: " << lResult << std::endl;
}
