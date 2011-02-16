/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
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

