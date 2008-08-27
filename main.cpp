
#include "beagle/GP.hpp"
#include "eval.hpp"
#include "LessThan.hpp"
#include "GreaterThan.hpp"
#include "IfThenElse.hpp"
#include "Avg.hpp"
#include "Max.hpp"
#include "Min.hpp"
#include "Eq.hpp"
#include "Context.hpp"
#include "EphemeralDay.hpp"

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
		
		lSet->insert(new LessThan);						// Manje od
		lSet->insert(new GreaterThan);					// Vece od
		lSet->insert(new IfThenElse);					// If-Then-Else
		lSet->insert(new Eq);							// Jednako

		lSet->insert(new GP::Add);						// Zbrajanje
		lSet->insert(new GP::Subtract);					// Oduzimanje
		lSet->insert(new GP::Multiply);					// Mnozenje
		lSet->insert(new GP::Divide);					// Djeljenje
		
		lSet->insert(new GP::And);						// Logicko i
		lSet->insert(new GP::Or);						// Logicko ili
		lSet->insert(new GP::Not);						// Logicko ne
		
		lSet->insert(new GP::TokenT<Bool>("True", Bool(true))); 	// Istina
		lSet->insert(new GP::TokenT<Bool>("False", Bool(false))); 	// Laz
		
		lSet->insert(new GP::EphemeralDouble);			// Konstanta [-1,1]
		lSet->insert(new EphemeralDay);					// Konstanta [0,365] za AVG, MIN, MAX
		lSet->insert(new GP::TokenT<Double>("C"));		// Cijena
		lSet->insert(new GP::TokenT<Double>("V"));		// Kolicina
		
		lSet->insert(new Avg);							// Srednja vrijednost cijene
		lSet->insert(new Min);							// Minimalna vrijednost cijene
		lSet->insert(new Max);							// Maksimalna vrijednost cijene
		
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

