/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "eval.hpp"
#include "Primitives.hpp"
#include "Fitness.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

using namespace std;
using namespace Beagle;
using namespace GP;


int main(int argc, char *argv[]) {
	try {
		// 1. Build primitives.
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet(&typeid(Bool));	// STGP, root node type Bool
		
		AddPrimitives(lSet);	// Add primitives
		
		// 2. Build a system.
		trading::Context::Alloc::Handle lContextAlloc = new trading::Context::Alloc();
		GP::System::Handle lSystem = new GP::System(lSet, lContextAlloc);
		
		// 3. Evaluation operator
		eval::Handle lEvalOp = new eval;

		// Initialize operators and system
		lEvalOp->initialize(*lSystem);
		lSystem->initialize(argc, argv);
		lSystem->postInit();
		lEvalOp->postInit(*lSystem);
		std::string lFileName = "best.xml"; 		// Load the best individual
		
		GP::Tree::Alloc::Handle lTreeAlloc = new GP::Tree::Alloc;
		trading::Fitness::Alloc::Handle lFitnessAlloc = new trading::Fitness::Alloc;	// Fitness
		Beagle::GP::Individual::Handle lIndividual = new Beagle::GP::Individual(lTreeAlloc,lFitnessAlloc);
		lIndividual->readFromFile(lFileName.c_str(), *lSystem);
		
	    // Write individual to screen
		//std::cout << std::endl << "Read individual: " << lIndividual->serialize() << std::endl;
	
	    // Evaluate individual
	    trading::Fitness::Handle lFitness = castHandleT<trading::Fitness>(lEvalOp->test(lIndividual, lSystem));
	
	    // Write fitness to screen
	    std::cout << "Fitness: " << std::endl;
	    std::cout << lFitness->getTrainingSetValue() << std::endl;

	} catch(Exception& inException) {
		inException.terminate();
	} catch(exception& inException) {
		cerr << "Cought a standard exception:" << endl;
		cerr << inException.what() << endl << flush;
		return 1;
	} catch(...) {
		cerr << "Cought an unknown exception!" << endl << flush;
		return 1;
	}
  return 0;
}

