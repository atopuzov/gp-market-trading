/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "IfThenElse.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;

// Ime IF
IfThenElse::IfThenElse() :
  Beagle::GP::Primitive(3, "IF")
{ }

#ifdef BEAGLE_HAVE_RTTI

const std::type_info* IfThenElse::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
  Beagle_AssertM(inN < 3);
  if(inN == 0) return &typeid(Bool);
  return &typeid(Double);
}

const std::type_info* IfThenElse::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Double);
}

#endif // BEAGLE_HAVE_RTTI

void IfThenElse::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
  Bool lCondition;
  get1stArgument(lCondition, ioContext);
  if(lCondition == true) get2ndArgument(outDatum, ioContext);
  else get3rdArgument(outDatum, ioContext);
}




