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
	
	// Trading log
	if(ioSystem.getRegister().isRegistered("aco.log")) {
		r_log = castHandleT<Bool>(ioSystem.getRegister()["aco.log"]);
	} else {
		r_log = new Bool(false);
		Register::Description lDescription(
			"log",
			"Bool",
			"0",
			"Trading log");
		ioSystem.getRegister().addEntry("aco.log", r_log, lDescription);
	}
	
	// Fee (commision)
	if(ioSystem.getRegister().isRegistered("aco.fee")) {
		r_fee = castHandleT<Double>(ioSystem.getRegister()["aco.fee"]);
	} else {
		r_fee = new Double(0.995);
		Register::Description lDescription(
			"fee",
			"Double",
			"0.995",
			"Factor to calculate in fee for every transaction");
		ioSystem.getRegister().addEntry("aco.fee", r_fee, lDescription);
	}
	
	// Amount of money to invest
	if(ioSystem.getRegister().isRegistered("aco.invest")) {
		r_invest = castHandleT<Double>(ioSystem.getRegister()["aco.invest"]);
	} else {
		r_invest = new Double(100000.);
		Register::Description lDescription(
			"invest",
			"Double",
			"100000",
			"Investment ammount");
		ioSystem.getRegister().addEntry("aco.invest", r_invest, lDescription);
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
			"Strategy to compare to: 1 Buy&Hold 2 Random");
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
	
	//std::cout << "Ticker: " << aContext.dionica << std::endl;
		
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
	
	bool	trading_log			= r_log->getWrappedValue();
	bool	on_the_market		= false;						// Indicates if we are already on the market
	bool	on_the_market_rnd	= false;						// For random b&s rule
	bool	traded				= false;						// Indicates if the rule did any transactions
    double	fee_factor			= r_fee->getWrappedValue();		// Factor that incorporates fee in the calculations
	double	invest				= r_invest->getWrappedValue();	// Initial ammount of money
	double	shares				= 0;							// Initial number of shares
	double	shares_rnd			= 0;							// For random b&s rule
	double	money				= invest;						// Amount of money available
	double	money_rnd			= invest;						// For random b&s rule
	double	price_on_first_day = 0;						// Share price on the first day
	double	price_on_last_day  = 0;						// Share price on the last day
	
	while ( (rc = sqlite3_step(preparedStatement)) == SQLITE_ROW ) {	// Loop trough all the records
	
		aContext.datum = (const char *)sqlite3_column_text(preparedStatement,0);	// Current date
		
		double vrjednost = sqlite3_column_double(preparedStatement,1);
		setValue("C", (Beagle::Double)vrjednost, ioContext);	// Set price
		
		double kolicina = sqlite3_column_double(preparedStatement,2);
		setValue("V", (Beagle::Double)kolicina, ioContext);		// Set volume traded
		
		price_on_last_day = vrjednost;
		
		//std::cout << "Vrjednost: " << vrjednost << ", datum:" << aContext.datum << std::endl;

		Bool trading_rule_signal;
		inIndividual.run(trading_rule_signal, ioContext);		// Execute the rule
		
		// 1. If off the market and the signal is buy (true)   => buy
		// 2. If on the market and the signal is buy (true)    => stay on market (don't sell)
		// 3. If off the market and the signal is sell (false) => stay off the market (don't buy)
		// 4. If on the market and the signal is sell (false)  => sell
		if(trading_rule_signal && !on_the_market)  {			// Buy
	    	shares = (money*fee_factor)/vrjednost;				// Calculate the number of shares
			if(trading_log) {
		    	std::string msg = "Date: " + aContext.datum + ", price: " + to_string<double>(vrjednost);
		    	msg            += ",\tbuying: " + to_string<double>(shares) + "\tshares for: " + to_string<double>(money);
		    	Beagle_LogBasicM(ioContext.getSystem().getLogger(),"eval", "eval",msg);
    		}
	    	money     = 0;
	    	on_the_market = true;								// We are on the market now
	    	traded        = true;								// We have traded at least once
    	} else if (!trading_rule_signal && on_the_market ) {	// Sell
    		money = (shares*vrjednost)*fee_factor;				// Calculate the value
    		if(trading_log) {
		    	std::string msg = "Date: " + aContext.datum + ", price: " + to_string<double>(vrjednost);
		    	msg            += ",\tselling: " + to_string<double>(shares) + "\tshares for: " + to_string<double>(money);
		    	Beagle_LogBasicM(ioContext.getSystem().getLogger(),"eval", "eval",msg);
			}
    		shares = 0;
    		on_the_market = false;								// We are off the market
    	}
    	if(price_on_first_day==(double)0) price_on_first_day = vrjednost;

		// Random buy & sell strategy
		bool random_rule_signal = ioContext.getSystem().getRandomizer().rollInteger(0,1);
		if(random_rule_signal && !on_the_market_rnd)  {			// Buy
			shares_rnd = (money_rnd*fee_factor)/vrjednost;		// Calculate the number of shares
	    	money_rnd			= 0;
	    	on_the_market_rnd	= true;							// We are on the market now
		} else if (!random_rule_signal && on_the_market_rnd ) {	// Sell
			money_rnd = (shares_rnd*vrjednost)*fee_factor;		// Calculate the value
			shares_rnd = 0;
			on_the_market_rnd = false;							// We are off the market
		}
	}
	
	sqlite3_finalize(preparedStatement);						// Free resources
	
	if(shares		!= (double)0) money		= shares*price_on_last_day*fee_factor;			// Sell everything at the end
	if(shares_rnd	!= (double)0) money_rnd	= shares_rnd*price_on_last_day*fee_factor;		// Random B&S strategy
	double buyhold = (price_on_last_day/price_on_first_day)*fee_factor*fee_factor*invest;	// Buy and hold strategy
	
	if(trading_log) {
		std::string msg = "Trgovano dionicom: " + aContext.dionica;
		Beagle_LogBasicM(ioContext.getSystem().getLogger(),"eval", "eval",msg);
		msg = "Price at the beginning of the interval: " + to_string<double>(price_on_first_day) + 
			", at the end of the interval: " + to_string<double>(price_on_last_day);
		Beagle_LogBasicM(ioContext.getSystem().getLogger(),"eval", "eval",msg);
		msg = "Profit: generated rule: " + to_string<double>(money - invest) +
			", buy_hold rule: " + to_string<double>(buyhold - invest);
		Beagle_LogBasicM(ioContext.getSystem().getLogger(),"eval", "eval",msg);
	}

	sqlite3_close(database);		// Close the database

	if(!traded)						// Penalize the rules that don't trade
		return (double)0;
	else if(money<invest)		// Or the ones that are bad (lose money)
		return (double)0;
	else
		return money/buyhold;
}

Fitness::Handle eval::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{ 
	sqlite3 *database;
	int rc;

	SQLITE_OPEN(rc,"",database)
	
	double interval_divider = r_divider->getWrappedValue();

	// Calculate the date that splits the dataset into 2 sets (training and validation)
	string sql  = "SELECT DATE(DATETIME(";
	sql        += "(STRFTIME('%s','" + r_startdate->getWrappedValue() + "') +";
	sql        += "(STRFTIME('%s','" + r_enddate->getWrappedValue() + "') - STRFTIME('%s','" + r_startdate->getWrappedValue() + "'))*" + to_string<double>(interval_divider) + ")";
	sql        += ",'unixepoch'))";
	
//	std::cout << sql << std::endl;
	
	sqlite3_stmt *preparedStatement;
	SQLITE_PREPARE_STATEMENT_TEST(preparedStatement, sql, database);
	SQLITE_STEP_TEST(preparedStatement);
	std::string  intermediate_date = (const char *)sqlite3_column_text(preparedStatement,0);
	sqlite3_finalize(preparedStatement);	// Free resources
	sqlite3_close(database);				// Close the database

	// Training set
	interval_start = r_startdate->getWrappedValue();		// Starting date
	interval_end   = intermediate_date;
	double rule_train		= evaluate_interval(inIndividual, ioContext);
	
	// Validation set
	interval_start = intermediate_date;
	interval_end   = r_enddate->getWrappedValue();		// End date
//	double rule_validation  = evaluate_interval(inIndividual, ioContext);
			
	FitnessMultiObj::Handle lFitness = new FitnessMultiObj(1);	// 2 fitness values
	(*lFitness)[0] = rule_train;								// Fitness on the training set
//	(*lFitness)[1] = rule_validation;							// Fitness on the validation set
	
//	std::cout << r_startdate->getWrappedValue() << " - " << intermediate_date << " - " << r_enddate->getWrappedValue() << std::endl;
//	std::cout << "Training set: " << to_string<double>(rule_train) << ", validation set: " << to_string<double>(rule_validation) << std::endl;
	
	return lFitness;
}

void eval::postInit(System& ioSystem)
{
  GP::EvaluationOp::postInit(ioSystem); 
}

