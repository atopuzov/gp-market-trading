#ifndef term_hpp
#define term_hpp

#include "beagle/TerminationOp.hpp"


namespace Beagle {
namespace aco {

class TermComplex : public Beagle::TerminationOp {

public:

	//! TermComplex allocator type
	typedef AllocatorT<TermComplex,TerminationOp::Alloc>
			Alloc;
	//! TermComplex handle type.
	typedef PointerT<TermComplex,TerminationOp::Handle>
			Handle;
	//! TermComplex bag type.
	typedef ContainerT<TermComplex,TerminationOp::Bag>
			Bag;

	explicit TermComplex(string inName="TermComplex");
	virtual ~TermComplex() { }

	virtual bool terminate(const Deme& inDeme, Context& ioContext);

};

}
}

#endif // term_hpp