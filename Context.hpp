#ifndef specContext_hpp
#define specContext_hpp

#include "Util.hpp"
#include "Util/Date.hpp"
#include "beagle/GP/Context.hpp"
#include "sqlite3.h"


namespace Beagle {
namespace GP {
namespace aco {

class Context : public Beagle::GP::Context {

public:

  typedef AllocatorT<Context,Beagle::GP::Context::Alloc> Alloc;
  typedef PointerT<Context,Beagle::GP::Context::Handle> Handle;
  typedef ContainerT<Context,Beagle::GP::Context::Bag> Bag;

           Context();
  virtual ~Context() { }

  string	datum;			// Datum
  string	dionica;		// Dionica
  sqlite3	*database;		// SQLite baza podataka
};

}
}
}

#endif // specContext_hpp
