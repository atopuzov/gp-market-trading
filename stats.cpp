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
	outStats.resize(4);
	outStats[3].mId  = "fitness2";
	outStats[3].mAvg = 0.0;
	outStats[3].mStd = 0.0;
	outStats[3].mMax = 0.0;
	outStats[3].mMin = 0.0;
    
	std::cout << "TODO - calculate statistics for validation set!" << std::endl;
}


