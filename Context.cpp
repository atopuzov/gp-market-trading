#include "beagle/GP.hpp"
#include "Context.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;

Beagle::GP::aco::Context::Context() : 
 datum(""),
 dionica(""),
 database(NULL)
{ }

