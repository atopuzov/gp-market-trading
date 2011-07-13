/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "Primitives.hpp"
#include "LessThan.hpp"
#include "GreaterThan.hpp"
#include "IfThenElse.hpp"
#include "Avg.hpp"
#include "Abs.hpp"
#include "Max.hpp"
#include "Min.hpp"
#include "Eq.hpp"
#include "Context.hpp"
#include "EphemeralDay.hpp"
#include "ROC.hpp"
#include "RSI.hpp"
#include "EMA.hpp"
#include "MACD.hpp"
#include "PPO.hpp"
#include "ConstantDouble.hpp"


void AddPrimitives(Beagle::GP::PrimitiveSet::Handle lSet)
{
	lSet->insert(new LessThan);			// Less than (<)
	lSet->insert(new GreaterThan);		// Greater than (>)
	lSet->insert(new IfThenElse);		// If-Then-Else
	lSet->insert(new Eq);				// Equals (=)
	
	lSet->insert(new Beagle::GP::Add);			// Addition (+)
	lSet->insert(new Beagle::GP::Subtract);		// Subtraction (-)
	lSet->insert(new Beagle::GP::Multiply);		// Multiplication (*)
	lSet->insert(new Beagle::GP::Divide);		// Division (/)
	
	lSet->insert(new Beagle::GP::And);		// Logical and (&)
	lSet->insert(new Beagle::GP::Or);		// Logical or (|)
	lSet->insert(new Beagle::GP::Not);		// Logical not (!)
	
	lSet->insert(new Beagle::GP::TokenT<Beagle::Bool>("True",  Beagle::Bool(true ))); 	// Logical constant true
	lSet->insert(new Beagle::GP::TokenT<Beagle::Bool>("False", Beagle::Bool(false))); 	// Logical constant false
	
	lSet->insert(new Beagle::GP::EphemeralDouble);		// Constant [-1,1]
	lSet->insert(new EphemeralDay);						// Constant [0,365] used for AVG, MIN, MAX
	lSet->insert(new ConstantDouble);
	lSet->insert(new Beagle::GP::TokenT<Beagle::Double>("P"));	// Price
	lSet->insert(new Beagle::GP::TokenT<Beagle::Double>("V"));	// Volume
	
	lSet->insert(new Avg);			// Average price in N days
	lSet->insert(new Min);			// Minimal price in N days
	lSet->insert(new Max);			// Maximum price in N days
	lSet->insert(new Abs);			// Absolute value
	lSet->insert(new ROC);			// Rate of change
//	lSet->insert(new RSI);			// Relative strength indicator
	lSet->insert(new EMA);			// Exponential moving average
//	lSet->insert(new MACD);			// Moving Average Convergence-Divergence
//	lSet->insert(new PPO);			// Percentage Price Oscilator
}
