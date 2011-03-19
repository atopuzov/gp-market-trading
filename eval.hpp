/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef eval_hpp
#define eval_hpp

#include "beagle/GP.hpp"
#include "Context.hpp"
#include "sqlite3.h"
#include "util.hpp"
#include <string>
#include <vector>

class eval : public Beagle::GP::EvaluationOp {

public:

  //! eval allocator type.
  typedef Beagle::AllocatorT<eval,Beagle::GP::EvaluationOp::Alloc> Alloc;
  //!< eval handle type.
  typedef Beagle::PointerT<eval,Beagle::GP::EvaluationOp::Handle> Handle;
  //!< eval bag type.
  typedef Beagle::ContainerT<eval,Beagle::GP::EvaluationOp::Bag> Bag;

  explicit eval();

  virtual void initialize(Beagle::System& ioSystem);
  virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
                                           Beagle::GP::Context& ioContext);
  virtual void postInit(Beagle::System& ioSystem);

protected:
	double evaluate_interval(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext);
	
	Beagle::String::Handle	baza;			// Ime baze
	Beagle::String::Handle	dionica;		// Ime dionice
	Beagle::String::Handle	start_date;		// Datum od kojeg pocinje trgovanje
	Beagle::String::Handle	end_date;		// Datum do kojeg se trguje
	Beagle::Bool::Handle    logTrgovanja;	// Log trgovanja
	
	std::string interval_start;
	std::string interval_end;
};

#endif // eval_hpp
