/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef stats_hpp
#define stats_hpp

namespace trading {

class StatsCalcComplex : public Beagle::GP::StatsCalcFitnessSimpleOp {

public:

	//! StatsCalcComplex allocator type
	typedef Beagle::AllocatorT<trading::StatsCalcComplex,Beagle::GP::StatsCalcFitnessSimpleOp::Alloc>
		Alloc;
	//! StatsCalcComplex handle type.
	typedef Beagle::PointerT<trading::StatsCalcComplex,Beagle::GP::StatsCalcFitnessSimpleOp::Handle>
		Handle;
	//! StatsCalcComplex bag type.
	typedef Beagle::ContainerT<trading::StatsCalcComplex,Beagle::GP::StatsCalcFitnessSimpleOp::Bag>
		Bag;
//	virtual void initialize(Beagle::System& ioSystem);

	explicit StatsCalcComplex(Beagle::string inName="GP-StatsCalcComplex");
	virtual ~StatsCalcComplex() { }

	virtual void calculateStatsDeme(Beagle::Stats& outStats,
									Beagle::Deme& ioDeme,
									Beagle::Context& ioContext) const;
};

}

#endif // stats_hpp
