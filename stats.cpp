#include "beagle/GP.hpp"
#include "stats.hpp"

Beagle::GP::aco::StatsCalcComplex::StatsCalcComplex(Beagle::string inName) :
  Beagle::GP::StatsCalcFitnessSimpleOp(inName)
{ }

//void Beagle::GP::aco::StatsCalcComplex::initialize(Beagle::System& ioSystem)
//{
//	Beagle::StatsCalculate::initialize(ioSystem);
//}

void Beagle::GP::aco::StatsCalcComplex::calculateStatsDeme(Beagle::Stats& outStats,
															Beagle::Deme& ioDeme,
															Beagle::Context& ioContext) const
{
	// TODO - calculate statistics for validation set
//	outStats[x].mId  = "fitness2";
//	outStats[x].mAvg = 0.0;
//	outStats[x].mStd = 0.0;
//	outStats[x].mMax = 0.0;
//	outStats[x].mMin = 0.0;
    
	std::cout << "Kalkuliram statistike!" << std::endl;
}


