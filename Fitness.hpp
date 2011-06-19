/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef fitness_hpp
#define fitness_hpp

#include <beagle/Beagle.hpp>
#include <beagle/AllocatorT.hpp>
#include <beagle/PointerT.hpp>
#include <beagle/ContainerT.hpp>
#include <beagle/macros.hpp>
#include <beagle/Fitness.hpp>
#include <beagle/FitnessSimple.hpp>

namespace trading {

class Fitness : public Beagle::FitnessSimple {

public:

	//! Fitness allocator type.
	typedef Beagle::AllocatorT<trading::Fitness,Beagle::FitnessSimple::Alloc>
		Alloc;
	//! Fitness handle type.
	typedef Beagle::PointerT<trading::Fitness,Beagle::FitnessSimple::Handle>
		Handle;
	//! Fitness bag type.
	typedef Beagle::ContainerT<trading::Fitness,Beagle::FitnessSimple::Bag>
		Bag;
	
			Fitness();
	explicit Fitness(float inFitness);
	explicit Fitness(float inFitness1, float inFitness2);
	virtual ~Fitness() { }
  
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

#endif // fitness_hpp
