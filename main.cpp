/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "eval.hpp"
#include "Primitives.hpp"
#include "StatsCalcOp.hpp"
#include "TermOp.hpp"
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
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet(&typeid(Bool));	// STGP, root node type "Boolean"
		AddPrimitives(lSet);	// Add primitives
		
		// 2. Build a system.
		trading::Context::Alloc::Handle lContextAlloc = new trading::Context::Alloc();
		GP::System::Handle lSystem = new GP::System(lSet, lContextAlloc);
		
		// 3. Evaluation operator
		eval::Handle lEvalOp = new eval;
		
		// 4. Build an evolver and a vivarium.
		GP::Evolver::Handle lEvolver = new GP::Evolver(lEvalOp);
		trading::StatsCalcOp::Handle lStatsCalcOp = new trading::StatsCalcOp;	// Modified statistics operator
		lEvolver->addOperator(lStatsCalcOp);									// Add new statistics calculation operator
		trading::TermOp::Handle lTermOp = new trading::TermOp;					// Modified termination operator
		lEvolver->addOperator(lTermOp);											// Add new termination operator
		
		// Build vivarium
		GP::Tree::Alloc::Handle lTreeAlloc = new GP::Tree::Alloc;
		trading::Fitness::Alloc::Handle lFitnessAlloc = new trading::Fitness::Alloc;	// Fitness
		GP::Vivarium::Handle lVivarium = new GP::Vivarium(lTreeAlloc, lFitnessAlloc);
		
		// 5. Initialize and evolve the vivarium.
		lEvolver->initialize(lSystem, argc, argv);
		lEvolver->evolve(lVivarium);
		
		// Go trough the hall of fame and retest the individuals on the validaion set
		HallOfFame& lHOF = lVivarium->getHallOfFame();
		lHOF.sort();
//		std::cout << std::endl << "Training set:\tValidation set:" << std::endl;
		GP::Individual::Handle lBestIndividual = castHandleT<GP::Individual>(lHOF[0].mIndividual);
		trading::Fitness::Handle lBestFitness = castHandleT<trading::Fitness>(lBestIndividual->getFitness());
		
		// Find the best individual on the validation set
		for(unsigned int i=0;i<lHOF.size();++i)
		{
			GP::Individual::Handle lIndividual = castHandleT<GP::Individual>(lHOF[i].mIndividual);
			trading::Fitness::Handle lFitness = castHandleT<trading::Fitness>(lIndividual->getFitness());
			std::cout << "-> "<< lFitness->getTrainingSetValue() << "\t" << lFitness->getValidationSetValue() << std::endl;
			if(lBestFitness->getValidationSetValue() < lFitness->getValidationSetValue() ) {
				lBestIndividual = castHandleT<GP::Individual>(lHOF[i].mIndividual);
 				lBestFitness = castHandleT<trading::Fitness>(lBestIndividual->getFitness());
//				std::cout << "Old    -> " << lBestIndividual->serialize() << std::endl;
//				std::cout << "Better -> " << lIndividual->serialize() << std::endl;
			}
		}

		std::ofstream best;
		best.open("best.xml");
		PACC::XML::Streamer str(best,2);
		lBestIndividual->write(str,true);
		best.close();

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
