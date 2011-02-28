/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef Primitives_hpp
#define Primitives_hpp

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
#include "ConstantDouble.hpp"


void AddPrimitives(Beagle::GP::PrimitiveSet::Handle lSet);


#endif // Primitives_hpp
