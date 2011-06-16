#include "term.hpp"

using namespace Beagle;
using namespace aco;

TermComplex::TermComplex(Beagle::string inName) :
  TerminationOp(inName)
{ }

bool TermComplex::terminate(const Deme& inDeme, Context& ioContext)
{
	// TODO - do complex criteria termination
	std::cout << "TODO - do complex criteria termination" << std::endl;
	return true;
}

