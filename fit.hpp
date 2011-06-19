/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef fit_hpp
#define fit_hpp

#include <beagle/Beagle.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/PointerT.hpp>
#include <beagle/ContainerT.hpp>
#include <beagle/macros.hpp>
#include <beagle/Fitness.hpp>
#include <beagle/FitnessSimple.hpp>

namespace trading {

class FitnessTrading : public Beagle::FitnessSimple {

public:

	//! FitnessTrading allocator type.
	typedef Beagle::AllocatorT<trading::FitnessTrading,Beagle::FitnessSimple::Alloc>
		Alloc;
	//! FitnessTrading handle type.
	typedef Beagle::PointerT<trading::FitnessTrading,Beagle::FitnessSimple::Handle>
		Handle;
	//! FitnessTrading bag type.
	typedef Beagle::ContainerT<trading::FitnessTrading,Beagle::FitnessSimple::Bag>
		Bag;
	
			FitnessTrading();
	explicit FitnessTrading(float inFitness);
	explicit FitnessTrading(float inFitness1, float inFitness2);
	virtual ~FitnessTrading() { }
  
	inline float getValue() const
	{
		return mFitness;
	}
	
	inline float getTrainingSetValue() const
	{
		return mFitness;
	}
	inline float getValidationSetValue() const
	{
		return vsFitness;
	}
	
	virtual void setValue(float inFitness);
	virtual void setValue(float inFitness1, float inFitness2);
	void write(PACC::XML::Streamer& ioStreamer, bool inIndent) const;
	void read(PACC::XML::ConstIterator inIter);	
	
protected:
	//float mFitness	//!< Fitness on the training set
	float vsFitness;	//!< Fitness on the validation set	

};

}

#endif // fit_hpp
