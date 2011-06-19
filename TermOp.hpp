/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef term_hpp
#define term_hpp

#include "beagle/TerminationOp.hpp"

namespace trading {

class TermOp : public Beagle::TerminationOp {

public:

	//! TermOp allocator type
	typedef Beagle::AllocatorT<trading::TermOp,Beagle::TerminationOp::Alloc>
			Alloc;
	//! TermOp handle type.
	typedef Beagle::PointerT<trading::TermOp,Beagle::TerminationOp::Handle>
			Handle;
	//! TermOp bag type.
	typedef Beagle::ContainerT<trading::TermOp,Beagle::TerminationOp::Bag>
			Bag;

	explicit TermOp(Beagle::string inName="TermOp");
	virtual ~TermOp() { }

	virtual bool terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext);

};

}

#endif // term_hpp