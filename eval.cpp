/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "eval.hpp"

#include <cmath>

using namespace Beagle;

eval::eval() :
  GP::EvaluationOp("eval")
{ }

void eval::initialize(Beagle::System& ioSystem)
{
	Beagle::GP::EvaluationOp::initialize(ioSystem);

	// Database filename
	if(ioSystem.getRegister().isRegistered("aco.baza")) {
		baza = castHandleT<String>(ioSystem.getRegister()["aco.baza"]);
	} else {
		baza = new String("se.db");
		Register::Description lDescription(
			"Database",
			"String",
			"se.db",
			"Name of the database file.");
		ioSystem.getRegister().addEntry("aco.baza", baza, lDescription);
	}
	
	// Ime dionice
	if(ioSystem.getRegister().isRegistered("aco.dionica")) {
		dionica = castHandleT<String>(ioSystem.getRegister()["aco.dionica"]);
	} else {
		dionica = new String("PBZ-R-A");
		Register::Description lDescription(
			"Ticker",
			"String",
			"PBZ-R-A",
			"Dionica kojom se trguje");
		ioSystem.getRegister().addEntry("aco.dionica", dionica, lDescription);
	}
	
	// Start date
	if(ioSystem.getRegister().isRegistered("aco.s_date")) {
		r_startdate = castHandleT<String>(ioSystem.getRegister()["aco.s_date"]);
	} else {
		r_startdate = new String("2006-01-01");
		Register::Description lDescription(
			"s_date",
			"String",
			"2006-01-01",
			"Trading start date");
		ioSystem.getRegister().addEntry("aco.s_date", r_startdate, lDescription);
	}
	
	// End date
	if(ioSystem.getRegister().isRegistered("aco.e_date")) {
		r_enddate = castHandleT<String>(ioSystem.getRegister()["aco.e_date"]);
	} else {
		r_enddate = new String("now");
		Register::Description lDescription(
			"e_date",
			"String",
			"now",
			"Trading end date");
		ioSystem.getRegister().addEntry("aco.e_date", r_enddate, lDescription);
	}
		
	// Fee (commision)
	if(ioSystem.getRegister().isRegistered("aco.fee")) {
		r_fee = castHandleT<Double>(ioSystem.getRegister()["aco.fee"]);
	} else {
		r_fee = new Double(0.025);
		Register::Description lDescription(
			"fee",
			"Double",
			"0.025",
			"Transaction cost in percentage of the price");
		ioSystem.getRegister().addEntry("aco.fee", r_fee, lDescription);
	}
	
	// Interval divider (for training and validation set)
	if(ioSystem.getRegister().isRegistered("aco.divider")) {
		r_divider = castHandleT<Double>(ioSystem.getRegister()["aco.divider"]);
	} else {
		r_divider = new Double(0.8);
		Register::Description lDescription(
			"divider",
			"Double",
			"0.8",
			"Training/validation set interval divider");
		ioSystem.getRegister().addEntry("aco.divider", r_divider, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("aco.strategy")) {
		r_strategy = castHandleT<Int>(ioSystem.getRegister()["aco.strategy"]);
	} else {
		r_strategy = new Int(1);
		Register::Description lDescription(
			"strategy",
			"Integer",
			"1",
			"Strategy to compare to: (1) Buy&Hold (2) Random");
		ioSystem.getRegister().addEntry("aco.strategy", r_divider, lDescription);
	}
}

double eval::evaluate_interval(GP::Individual& inIndividual, GP::Context& ioContext)
{
	Beagle::GP::aco::Context& aContext = castObjectT<Beagle::GP::aco::Context&>(ioContext);
	aContext.dionica = dionica->getWrappedValue();

    sqlite3 *database;													// Sqlite database
	int rc;
    
	SQLITE_OPEN(rc,baza->getWrappedValue().c_str(),database);
	
	aContext.database = database;	// SQLite database
		
	// SQL Query
	string sql  = "SELECT DATUM,ZADNJA,KOLICINA FROM ZSE WHERE DIONICA=UPPER('";
	sql        += dionica->getWrappedValue();
	sql        += "') AND DATE(DATUM) BETWEEN DATE('";
	sql        += interval_start;
	sql        += "') AND DATE('";
	sql        += interval_end;
	sql        += "') ORDER BY DATE(DATUM) ASC";

//	std::cout << sql << std::endl;
	
	sqlite3_stmt *preparedStatement;
	SQLITE_PREPARE_STATEMENT_TEST(preparedStatement, sql, database);
	
	bool	on_the_market		= false;						// Indicates if we are already on the market
	bool	on_the_market_rnd	= false;						// For random b&s rule	
	double	price_t					= 0;						// Price at day t
	double	price_t_1				= 0;						// Price at dat t-1
	double	transaction_cost		= r_fee->getWrappedValue();	// Transaction cost
	double	r_r						= 0;						// Compound return for the generated rule
	double	r_bh					= 0;						// Compound return for buy & hold rule
	double	r_bs					= 0;						// Compound return for random buy & sell rule
	int		number_of_trades		= 0;
	int		number_of_trades_rnd	= 0;
	
	while ( (rc = sqlite3_step(preparedStatement)) == SQLITE_ROW ) {	// Loop trough all the records
	
		aContext.datum = (const char *)sqlite3_column_text(preparedStatement,0);	// Current date
		
		double price = sqlite3_column_double(preparedStatement,1);
		setValue("P", (Beagle::Double)price, ioContext);		// Set price
		
		double volume = sqlite3_column_double(preparedStatement,2);
		setValue("V", (Beagle::Double)volume, ioContext);		// Set volume traded
				
		if(price_t == 0 && price_t_1 == 0)						// First evaluated day
		{
			price_t = price;									// price at day t
		} else {
			price_t_1	= price_t;								// price at day t/1
			price_t		= price;								// price at day t
			double	r_t = log(price_t) - log(price_t_1);		// daily continuously compounded return
			if(on_the_market)		r_r  += r_t;				// Generated trading rule compound return
			if(on_the_market_rnd)	r_bs += r_t;				// Random buy & sell rule compound return
			r_bh += r_t;										// Buy & hold
		}
		
		// Generated trading rule 
		Bool trading_rule_signal;
		inIndividual.run(trading_rule_signal, ioContext);		// Execute the rule
		// 1. If off the market and the signal is buy (true)   => buy
		// 2. If on the market and the signal is buy (true)    => stay on market (don't sell)
		// 3. If off the market and the signal is sell (false) => stay off the market (don't buy)
		// 4. If on the market and the signal is sell (false)  => sell
		if(trading_rule_signal && !on_the_market)  {			// Buy
			on_the_market	= true;								// We are on the market now
		} else if (!trading_rule_signal && on_the_market ) {	// Sell
			on_the_market	= false;							// We are off the market
			++number_of_trades;
		} // if
		
		// Random buy & sell strategy
		bool random_rule_signal = ioContext.getSystem().getRandomizer().rollInteger(0,1);
		if(random_rule_signal && !on_the_market_rnd)  {			// Buy
	    	on_the_market_rnd	= true;							// We are on the market now
		} else if (!random_rule_signal && on_the_market_rnd ) {	// Sell
			on_the_market_rnd = false;							// We are off the market
			++number_of_trades_rnd;
		}	// if
	}	// while
	sqlite3_finalize(preparedStatement);						// Free resources
	sqlite3_close(database);		// Close the database

	if(on_the_market)		++number_of_trades;					// Sell all at the end
	if(on_the_market_rnd)	++number_of_trades_rnd;
	
	double cost = log((1-transaction_cost)/(1+transaction_cost));	// Calculte in the transacion fees
	r_r		+= number_of_trades * cost;
//	r_bs	+= number_of_trades_rnd * cost;
	r_bh	+= cost;

	switch(r_strategy->getWrappedValue())
	{
		case	1:
			break;
		case	2:
			return exp(r_r - r_bs);
			break;
	}
	return exp(r_r - r_bh);
}


Fitness::Handle eval::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{ 
	interval_start	= r_startdate->getWrappedValue();				// Starting date
	interval_end	= r_enddate->getWrappedValue();					// End date
	double eval		= evaluate_interval(inIndividual, ioContext);	// Calculate the fitness
	return new FitnessSimple(eval);
}

void eval::postInit(System& ioSystem)
{
  GP::EvaluationOp::postInit(ioSystem); 
}

