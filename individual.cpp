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
		// 1: Kreiraj skup primitiva
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet(&typeid(Bool));	// STGP, korjenski cvor Bool
		
		AddPrimitives(lSet);	// Dodaj primitive
		
		// 2: Kreiraj sustav
		aco::Context::Alloc::Handle lContextAlloc = new aco::Context::Alloc();
		GP::System::Handle lSystem = new GP::System(lSet, lContextAlloc);
		
		// 3: Evaluacijski operator
		eval::Handle lEvalOp = new eval;

		// Initialize operators and system
		lEvalOp->initialize(*lSystem);
		lSystem->initialize(argc, argv);
		lSystem->postInit();
		lEvalOp->postInit(*lSystem);
		std::string lFileName = "best.xml"; 		// Ucitaj najboljeg
		

		// Log trgovanja
		//Beagle::Bool::Handle logTrgovanja = castHandleT<Beagle::Bool>(lSystem->getRegister()["aco.log"]);
		//logTrgovanja->setWrappedValue(true);
		
		Beagle::GP::Individual::Handle lIndividual = new Beagle::GP::Individual;
		lIndividual->readFromFile(lFileName.c_str(), *lSystem);
		
	    // Write individual to screen
		//std::cout << std::endl << "Read individual: " << lIndividual->serialize() << std::endl;
	
	    // Evaluate individual
	    Beagle::Fitness::Handle lFitness = lEvalOp->test(lIndividual, lSystem);
	
	    // Write fitness to screen
	    //std::cout << "Fitness: " << std::endl;
	    //std::cout << lFitness->serialize() << std::endl;

	} catch(Exception& inException) {
		inException.terminate();
	} catch(exception& inException) {
		cerr << "Uhvacena standardna iznimka:" << endl;
		cerr << inException.what() << endl << flush;
		return 1;
	} catch(...) {
		cerr << "Uhvacena nepoznata iznimka!" << endl << flush;
		return 1;
	}
  return 0;
}

