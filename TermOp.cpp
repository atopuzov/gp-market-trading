/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include <beagle/GP.hpp>
#include <beagle/HallOfFame.hpp>
#include "TermOp.hpp"

using namespace trading;

TermOp::TermOp(Beagle::string inName) :
  TerminationOp(inName)
{	}

bool TermOp::terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext)
{
	// TODO - do complex criteria termination
	const Beagle::GP::Deme& lGPDeme = Beagle::castObjectT<const Beagle::GP::Deme&>(inDeme);
	const Beagle::HallOfFame& lHOF = lGPDeme.getHallOfFame();									// Hall of fame
	std::cout << "TODO - do complex criteria termination" << std::endl;
	return true;
}
