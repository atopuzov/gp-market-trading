/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "eval.hpp"
#include "Fitness.hpp"
#include <cmath>

using namespace Beagle;

eval::eval() :
	GP::EvaluationOp("eval")
{	}

void eval::initialize(Beagle::System& ioSystem)
{
	Beagle::GP::EvaluationOp::initialize(ioSystem);

	// Database filename
	if(ioSystem.getRegister().isRegistered("trading.database")) {
		r_database = castHandleT<String>(ioSystem.getRegister()["trading.database"]);
	} else {
		r_database = new String("se.db");
		Register::Description lDescription(
			"Database filename",
			"String",
			"se.db",
			"Name of the database file.");
		ioSystem.getRegister().addEntry("trading.database", r_database, lDescription);
	}

	// Ime dionice
	if(ioSystem.getRegister().isRegistered("trading.ticker")) {
		r_ticker = castHandleT<String>(ioSystem.getRegister()["trading.ticker"]);
	} else {
		r_ticker = new String("PBZ-R-A");
		Register::Description lDescription(
			"Stock ticker symbol",
			"String",
			"PBZ-R-A",
			"Stock ticker symbol");
		ioSystem.getRegister().addEntry("trading.ticker", r_ticker, lDescription);
	}

	// Training interval start date
	if(ioSystem.getRegister().isRegistered("trading.ts_date")) {
		r_ts_date = castHandleT<String>(ioSystem.getRegister()["trading.ts_date"]);
	} else {
		r_ts_date = new String("2006-01-01");
		Register::Description lDescription(
			"Training set interval start date",
			"String",
			"2000-01-01",
			"Training set interval start date");
		ioSystem.getRegister().addEntry("trading.ts_date", r_ts_date, lDescription);
	}

	// Training interval end date, 
	if(ioSystem.getRegister().isRegistered("trading.te_date")) {
		r_te_date = castHandleT<String>(ioSystem.getRegister()["trading.te_date"]);
	} else {
		r_te_date = new String("2005-31-12");
		Register::Description lDescription(
			"Training set interval end date",
			"String",
			"2005-31-12",
			"Training set interval end date");
		ioSystem.getRegister().addEntry("trading.te_date", r_te_date, lDescription);
	}

	// Validation interval start date
	if(ioSystem.getRegister().isRegistered("trading.vs_date")) {
		r_vs_date = castHandleT<String>(ioSystem.getRegister()["trading.vs_date"]);
	} else {
		r_vs_date = new String("2006-01-01");
		Register::Description lDescription(
			"Validation set interval start date",
			"String",
			"2006-01-01",
			"Validation set interval start date");
		ioSystem.getRegister().addEntry("trading.vs_date", r_vs_date, lDescription);
	}

	// Validation interval end date, 
	if(ioSystem.getRegister().isRegistered("trading.ve_date")) {
		r_ve_date = castHandleT<String>(ioSystem.getRegister()["trading.ve_date"]);
	} else {
		r_ve_date = new String("now");
		Register::Description lDescription(
			"Validation set interval end date",
			"String",
			"now",
			"Validation set interval end date");
		ioSystem.getRegister().addEntry("trading.ve_date", r_ve_date, lDescription);
	}

	// Fee (commision)
	if(ioSystem.getRegister().isRegistered("trading.fee")) {
		r_fee = castHandleT<Double>(ioSystem.getRegister()["trading.fee"]);
	} else {
		r_fee = new Double(0.0025);
		Register::Description lDescription(
			"Transaction cost",
			"Double",
			"0.0025",
			"Transaction cost in percentage of the price");
		ioSystem.getRegister().addEntry("trading.fee", r_fee, lDescription);
	}

	if(ioSystem.getRegister().isRegistered("trading.strategy")) {
		r_strategy = castHandleT<Int>(ioSystem.getRegister()["trading.strategy"]);
	} else {
		r_strategy = new Int(1);
		Register::Description lDescription(
			"Baseline strategy",
			"Integer",
			"1",
			"Strategy to compare to: (1) Buy&Hold (2) Random");
		ioSystem.getRegister().addEntry("trading.strategy", r_strategy, lDescription);
	}
	
	if(ioSystem.getRegister().isRegistered("trading.calc_vs")) {
		r_calc_vs = castHandleT<Bool>(ioSystem.getRegister()["trading.calc_vs"]);
	} else {
		r_calc_vs = new Bool(true);
		Register::Description lDescription(
			"Calculate fitness on the validation set",
			"Bool",
			"1",
			"Calculate fitness on the validation set");
		ioSystem.getRegister().addEntry("trading.calc_vs", r_calc_vs, lDescription);
	}
}

double eval::evaluate_interval(GP::Individual& inIndividual, GP::Context& ioContext)
{
	trading::Context& aContext = castObjectT<trading::Context&>(ioContext);
	aContext.dionica = r_ticker->getWrappedValue();

    sqlite3 *database;											// Sqlite database
	int rc;

	SQLITE_OPEN(rc,r_database->getWrappedValue().c_str(),database);
	aContext.database = database;								// SQLite database

	// SQL Query
	string sql	 = "SELECT DATUM,ZADNJA,KOLICINA FROM ZSE WHERE DIONICA=UPPER('";
	sql			+= r_ticker->getWrappedValue();
	sql			+= "') AND DATE(DATUM) BETWEEN DATE('";
	sql			+= interval_start;
	sql			+= "') AND DATE('";
	sql			+= interval_end;
	sql			+= "') ORDER BY DATE(DATUM) ASC";

//	std::cout << sql << std::endl;
	
	sqlite3_stmt *preparedStatement;
	SQLITE_PREPARE_STATEMENT_TEST(preparedStatement, sql, database);
	
	bool	on_the_market			= false;					// Indicates if we are already on the market
	bool	on_the_market_rnd		= false;					// For random b&s rule	
	double	price_t					= 0;						// Price at day t
	double	price_t_1				= 0;						// Price at dat t-1
	double	transaction_cost		= r_fee->getWrappedValue();	// Transaction cost
	double	r_r						= 0;						// Compound return for the generated rule
	double	r_bh					= 0;						// Compound return for buy & hold rule
	double	r_bs					= 0;						// Compound return for random buy & sell rule
	int		number_of_trades		= 0;						// Number of trades for the generated rule
	int		number_of_trades_rnd	= 0;						// Number of trades for the random buy & sell rule
	
	while ( (rc = sqlite3_step(preparedStatement)) == SQLITE_ROW ) {	// Loop trough all the records
	
		aContext.datum = (const char *)sqlite3_column_text(preparedStatement,0);	// Current date
		
		double price = sqlite3_column_double(preparedStatement,1);
		setValue("P", (Beagle::Double)price, ioContext);		// Set price
//		std::cout << "Price: " << price << std::endl;
		
		double volume = sqlite3_column_double(preparedStatement,2);
		setValue("V", (Beagle::Double)volume, ioContext);		// Set trading volume
//		std::cout << "Volume: " << volume << std::endl;
				
		if(price_t_1 == 0 && price_t == 0) {					// First evaluated day
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
//			std::cout << "Bought!" << std::endl;
		} else if (!trading_rule_signal && on_the_market ) {	// Sell
			on_the_market	= false;							// We are off the market
//			std::cout << "Sold!" << std::endl;
			++number_of_trades;
		} // if
		
		// Random buy & sell strategy
		bool random_rule_signal;
		if (ioContext.getSystem().getRandomizer().rollInteger(0,20) == 20)
			random_rule_signal = !on_the_market_rnd;		
		if(random_rule_signal && !on_the_market_rnd)  {			// Buy
	    	on_the_market_rnd	= true;							// We are on the market now
		} else if (!random_rule_signal && on_the_market_rnd ) {	// Sell
			on_the_market_rnd = false;							// We are off the market
			++number_of_trades_rnd;
		}	// if
	}	// while
	sqlite3_finalize(preparedStatement);						// Free resources
	sqlite3_close(database);									// Close the database

	if(on_the_market)		++number_of_trades;					// Sell all at the end
	if(on_the_market_rnd)	++number_of_trades_rnd;
	
	double cost = log((1-transaction_cost)/(1+transaction_cost));	// Calculte in the transacion fees
	r_r		+= number_of_trades * cost;
	r_bs	+= number_of_trades_rnd * cost;
	r_bh	+= cost;

//	std::cout << "r_r:\t" << r_r << "\tr_bh:\t" << r_bh << "\tnt:\t" << number_of_trades << "\tfitness:\t" << exp(r_r - r_bh) << std::endl; 

//	if(number_of_trades==0) return 0;
//	if(r_r == r_bs) return 0;

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
//	if(interval_start == "" || interval_end == "")
	set_training_interval();
	double eval1 = evaluate_interval(inIndividual, ioContext);	// Calculate the fitness on the training interval
	double eval2 = 0;
	if(r_calc_vs->getWrappedValue()) {
		set_validation_interval();
		eval2 = evaluate_interval(inIndividual, ioContext);	// Calculate the fitness on the validation interval
	}
//	std::cout << eval1 << "\t" << eval2 << std::endl;
	return new trading::Fitness(eval1,eval2);
}

void eval::set_testing_interval(std::string start, std::string end)
{
	interval_start	= start;
	interval_end	= end;
}

void eval::set_training_interval()
{
	set_testing_interval(r_ts_date->getWrappedValue(),r_te_date->getWrappedValue());
}

void eval::set_validation_interval()
{
	set_testing_interval(r_vs_date->getWrappedValue(),r_ve_date->getWrappedValue());	
}

void eval::postInit(System& ioSystem)
{
	GP::EvaluationOp::postInit(ioSystem); 
}
