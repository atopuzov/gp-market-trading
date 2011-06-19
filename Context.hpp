/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef specContext_hpp
#define specContext_hpp

#include "Util.hpp"
#include "Util/Date.hpp"
#include "beagle/GP/Context.hpp"
#include "sqlite3.h"

namespace trading {

class Context : public Beagle::GP::Context {

public:

	typedef Beagle::AllocatorT<trading::Context,Beagle::GP::Context::Alloc> Alloc;
	typedef Beagle::PointerT<trading::Context,Beagle::GP::Context::Handle> Handle;
	typedef Beagle::ContainerT<trading::Context,Beagle::GP::Context::Bag> Bag;

			 Context();
	virtual ~Context() { }

	std::string	datum;			// Datum
	std::string	dionica;		// Dionica
	sqlite3	*database;		// SQLite database
};

}

#endif // specContext_hpp
