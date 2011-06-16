
#ifndef stats_hpp
#define stats_hpp

namespace Beagle {
namespace GP {
namespace aco {

class StatsCalcComplex : public Beagle::GP::StatsCalcFitnessSimpleOp {

public:

	//! StatsCalcComplex allocator type
	typedef AllocatorT<StatsCalcComplex,Beagle::GP::aco::StatsCalcComplex::Alloc>
		Alloc;
	//! StatsCalcComplex handle type.
	typedef PointerT<StatsCalcComplex,Beagle::GP::aco::StatsCalcComplex::Handle>
		Handle;
	//! StatsCalcComplex bag type.
	typedef ContainerT<StatsCalcComplex,Beagle::GP::aco::StatsCalcComplex::Bag>
		Bag;
//	virtual void initialize(Beagle::System& ioSystem);

	explicit StatsCalcComplex(string inName="GP-StatsCalcComplex");
	virtual ~StatsCalcComplex() { }

	virtual void calculateStatsDeme(Beagle::Stats& outStats,
									Beagle::Deme& ioDeme,
									Beagle::Context& ioContext) const;
};

}
}
}
#endif // stats_hpp
