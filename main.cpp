/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "eval.hpp"
#include "Primitives.hpp"

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
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet(&typeid(Bool));	// STGP, root node type "Boolean"
		AddPrimitives(lSet);	// Add primitives
		
		// 2. Build a system.
		aco::Context::Alloc::Handle lContextAlloc = new aco::Context::Alloc();
		GP::System::Handle lSystem = new GP::System(lSet, lContextAlloc);
		
		// 3. Evaluation operator
		eval::Handle lEvalOp = new eval;
	
		// 4. Build an evolver and a vivarium.
		GP::Evolver::Handle lEvolver = new GP::Evolver(lEvalOp);
		GP::Vivarium::Handle lVivarium = new GP::Vivarium;
		
		// 5. Initialize and evolve the vivarium.
		lEvolver->initialize(lSystem, argc, argv);
		lEvolver->evolve(lVivarium);
		
		HallOfFame& HOF = lVivarium->getHallOfFame();	// Hall of fame (contains the best individuals)
		HOF.sort();
		lEvalOp->set_validation_interval();
		
		// Go trough the hall of fame and retest the individuals on the validaion set
		for(unsigned int i=0;i<HOF.size();++i)
		{
			GP::Individual::Handle lIndividual = castHandleT<GP::Individual>(HOF[i].mIndividual);
			Beagle::Fitness::Handle loldFitness = lIndividual->getFitness();
			Beagle::Fitness::Handle lFitness = lEvalOp->test(lIndividual, lSystem);
//			std::cout << lIndividual->serialize() << std::endl;
			if (loldFitness->isLess(*lFitness)) {
				std::cout << "Fitness increase on validation interval:" << std::endl;
				std::cout << loldFitness->serialize() << "\t:\t" << lFitness->serialize() << std::endl;
			}
		}
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
