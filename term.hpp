/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef term_hpp
#define term_hpp

#include "beagle/TerminationOp.hpp"

namespace trading {

class TermComplex : public Beagle::TerminationOp {

public:

	//! TermComplex allocator type
	typedef Beagle::AllocatorT<trading::TermComplex,Beagle::TerminationOp::Alloc>
			Alloc;
	//! TermComplex handle type.
	typedef Beagle::PointerT<trading::TermComplex,Beagle::TerminationOp::Handle>
			Handle;
	//! TermComplex bag type.
	typedef Beagle::ContainerT<trading::TermComplex,Beagle::TerminationOp::Bag>
			Bag;

	explicit TermComplex(Beagle::string inName="TermComplex");
	virtual ~TermComplex() { }

	virtual bool terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext);

};

}

#endif // term_hpp