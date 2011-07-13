/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "PPO.hpp"
#include "ta.hpp"

#include <sstream>
#include <cmath>
#include <fstream>

using namespace Beagle;

// Primitive name: PPO
PPO::PPO() : Beagle::GP::Primitive(0, "PPO")
{	}

#ifdef BEAGLE_HAVE_RTTI

// Return value type: double
const std::type_info* PPO::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Double);
}

#endif // BEAGLE_HAVE_RTTI

// Exponential Moving Average (PPO)
// PPO = (12-day EMA - 26-day EMA)/ 26-day EMA * 100
void PPO::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Double& lResult = castObjectT<Double&>(outDatum);
	trading::Context& aContext = castObjectT<trading::Context&>(ioContext);
	
	lResult	= ppo(aContext.database, aContext.dionica, aContext.datum);
//	std::cout << "PPO: " << lResult << std::endl;
}


