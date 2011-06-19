/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include <beagle/GP.hpp>
#include <beagle/macros.hpp>
#include <cmath>
#include "stats.hpp"
#include "fit.hpp"

using namespace trading;

StatsCalcOp::StatsCalcOp(Beagle::string inName) :
  Beagle::GP::StatsCalcFitnessSimpleOp(inName)
{ }

void StatsCalcOp::calculateStatsDeme(Beagle::Stats& outStats,
										  Beagle::Deme& ioDeme,
										  Beagle::Context& ioContext) const
{
	Beagle::GP::StatsCalcFitnessSimpleOp::calculateStatsDeme(outStats, ioDeme, ioContext);	// Call base class method
	const Beagle::GP::Deme& lGPDeme = Beagle::castObjectT<const Beagle::GP::Deme&>(ioDeme);
	
	if(ioDeme.size() == 0) {
		outStats.setGenerationValues(Beagle::string("deme")+Beagle::uint2str(ioContext.getDemeIndex()+1), ioContext.getGeneration(), 0, true);
		outStats.resize(4);
		outStats.back().mId  = "fitness2";
		outStats.back().mAvg = 0.0;
		outStats.back().mStd = 0.0;
		outStats.back().mMax = 0.0;
		outStats.back().mMin = 0.0;
		return;
	}

	const trading::Fitness::Handle lFirstIndivFitness = Beagle::castHandleT<trading::Fitness>(ioDeme[0]->getFitness());

	if(ioDeme.size() == 1) {
		outStats.setGenerationValues(Beagle::string("deme")+Beagle::uint2str(ioContext.getDemeIndex()+1), ioContext.getGeneration(), 1, true);
		outStats.resize(4);
		outStats.back().mId  = "fitness2";
		outStats.back().mAvg = lFirstIndivFitness->getValidationSetValue();
		outStats.back().mStd = 0.0;
		outStats.back().mMax = lFirstIndivFitness->getValidationSetValue();
		outStats.back().mMin = lFirstIndivFitness->getValidationSetValue();
		return;
	}
	
	double lSum = (double)lFirstIndivFitness->getValidationSetValue();
	double lPow2Sum = Beagle::pow2Of<double>((double)lFirstIndivFitness->getValidationSetValue());
	double lMax = lFirstIndivFitness->getValidationSetValue();
	double lMin = lFirstIndivFitness->getValidationSetValue();
	
	for(unsigned int i=1; i<lGPDeme.size(); i++) {
		const trading::Fitness::Handle lIndivFitness = Beagle::castHandleT<trading::Fitness>(lGPDeme[i]->getFitness());
		lSum     += (double)lIndivFitness->getValidationSetValue();
	    lPow2Sum += Beagle::pow2Of<double>((double)lIndivFitness->getValidationSetValue());
	    lMax     =  Beagle::maxOf<double>(lMax, (double)lIndivFitness->getValidationSetValue());
	    lMin     =  Beagle::minOf<double>(lMin, (double)lIndivFitness->getValidationSetValue());
	}
	
	double lAverage  = lSum / ioDeme.size();
	double lStdError = (lPow2Sum - (Beagle::pow2Of<double>(lSum)/ioDeme.size())) / (ioDeme.size()-1);
	lStdError        = sqrt(lStdError);

	outStats.setGenerationValues(Beagle::string("deme")+Beagle::uint2str(ioContext.getDemeIndex()+1), ioContext.getGeneration(), ioDeme.size(), true);
	
	outStats.resize(4);
	outStats.back().mId  = "fitness2";
	outStats.back().mAvg = lAverage;
	outStats.back().mStd = lStdError;
	outStats.back().mMax = lMax;
	outStats.back().mMin = lMin;
}
