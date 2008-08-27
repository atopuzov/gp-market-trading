#include "beagle/GP.hpp"
#include "EphemeralDay.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;


EphemeralDay::EphemeralDay(Beagle::UInt::Handle inValue) :
  GP::EphemeralT<Beagle::UInt>(inValue, "D")
{ }

GP::Primitive::Handle EphemeralDay::generate(Beagle::string inName, GP::Context& ioContext)
{
  UInt::Handle lValue = new UInt((unsigned int)ioContext.getSystem().getRandomizer().rollUniform(0.,365.));
  return new EphemeralDay(lValue);
}
