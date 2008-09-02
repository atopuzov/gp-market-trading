#include "beagle/GP.hpp"
#include "ConstantDouble.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;


ConstantDouble::ConstantDouble(Beagle::Double::Handle inValue) :
  GP::EphemeralT<Beagle::Double>(inValue, "K")
{ }

GP::Primitive::Handle ConstantDouble::generate(Beagle::string inName, GP::Context& ioContext)
{
  Double::Handle lValue = new Double(ioContext.getSystem().getRandomizer().rollUniform(-1000.,1000.));
  return new ConstantDouble(lValue);
}
