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
	
		// 4: Build an evolver and a vivarium.
		GP::Evolver::Handle lEvolver = new GP::Evolver(lEvalOp);
		GP::Vivarium::Handle lVivarium = new GP::Vivarium;
		
		// 5: Initialize and evolve the vivarium.
		lEvolver->initialize(lSystem, argc, argv);
		lEvolver->evolve(lVivarium);
		
		// Snimi najbolju jedinku u datoteku
		HallOfFame& lHOF = lVivarium->getHallOfFame();	// Dvorana slavnih
		lHOF.sort();
		GP::Individual::Handle lBestOfHOF = castHandleT<GP::Individual>(lHOF[0].mIndividual);
		ofstream best;
		best.open("best.xml");
		PACC::XML::Streamer str(best,2);
		lBestOfHOF->write(str,true);
		best.close();
		
	
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

