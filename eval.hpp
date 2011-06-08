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

	void set_training_interval();
	void set_validation_interval();


protected:
	double evaluate_interval(Beagle::GP::Individual& inIndividual, Beagle::GP::Context& ioContext);
	void set_testing_interval(std::string start, std::string end);
	
	Beagle::String::Handle	r_database;		// Database filename
	Beagle::String::Handle	r_ticker;		// Stock symbol
	Beagle::Double::Handle	r_fee;			// Fee
	Beagle::Int::Handle		r_strategy;		// Compare to 1 B&H 2 random B&S
	Beagle::String::Handle	r_ts_date;		// Training interval start date
	Beagle::String::Handle	r_te_date;		// Training interval end date
	Beagle::String::Handle	r_vs_date;		// Validation interval start date
	Beagle::String::Handle	r_ve_date;		// Validation interval end date
	
	std::string interval_start;
	std::string interval_end;
};

#endif // eval_hpp
