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
	if(ioSystem.getRegister().isRegistered("aco.pdatum")) {
		start_date = castHandleT<String>(ioSystem.getRegister()["aco.pdatum"]);
	} else {
		start_date = new String("2006-01-01");
		Register::Description lDescription(
			"pDatum",
			"String",
			"2006-01-01",
			"Trading start date");
		ioSystem.getRegister().addEntry("aco.pdatum", start_date, lDescription);
	}
	
	// End date
	if(ioSystem.getRegister().isRegistered("aco.zdatum")) {
		end_date = castHandleT<String>(ioSystem.getRegister()["aco.zdatum"]);
	} else {
		end_date = new String("now");
		Register::Description lDescription(
			"zDatum",
			"String",
			"now",
			"Trading end date");
		ioSystem.getRegister().addEntry("aco.zdatum", end_date, lDescription);
	}
	
	// Trading log
	if(ioSystem.getRegister().isRegistered("aco.log")) {
		logTrgovanja = castHandleT<Bool>(ioSystem.getRegister()["aco.log"]);
	} else {
		logTrgovanja = new Bool(false);
		Register::Description lDescription(
			"log",
			"Bool",
			"0",
			"Trading log");
		ioSystem.getRegister().addEntry("aco.log", logTrgovanja, lDescription);
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
	
	bool   trading_log        = logTrgovanja->getWrappedValue();
	bool   on_the_market      = false;					// Indicates if we are already on the market
	bool   traded             = false;					// Indicates if the rule did any transactions
    double provizija          = 0.995;					// Faktor kojim racunalo proviziju trgovanja
    double iInvesticija       = 100000.;				// Inicijalna investicija
	double shares             = 0;						// Initial number of shares
	double money              = iInvesticija;			// Initial ammount of money
	double price_on_first_day = 0;						// Share price on the first day
	double price_on_last_day  = 0;						// Share price on the last day

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
	    	shares = (money*provizija)/vrjednost;				// Calculate the number of shares
			if(trading_log) {
		    	std::string msg = "Date: " + aContext.datum + ", price: " + to_string<double>(vrjednost);
		    	msg            += ",\tbuying: " + to_string<double>(shares) + "\tshares for: " + to_string<double>(money);
		    	Beagle_LogBasicM(ioContext.getSystem().getLogger(),"eval", "eval",msg);
    		}
	    	money     = 0;
	    	on_the_market = true;								// We are on the market now
	    	traded        = true;								// We have traded at least once
    	} else if (!trading_rule_signal && on_the_market ) {	// Sell
    		money = (shares*vrjednost)*provizija;				// Calculate the value
    		if(trading_log) {
		    	std::string msg = "Date: " + aContext.datum + ", price: " + to_string<double>(vrjednost);
		    	msg            += ",\tselling: " + to_string<double>(shares) + "\tshares for: " + to_string<double>(money);
		    	Beagle_LogBasicM(ioContext.getSystem().getLogger(),"eval", "eval",msg);
			}
    		shares = 0;
    		on_the_market = false;								// We are off the market
    	}
    	if(price_on_first_day==(double)0) price_on_first_day = vrjednost;
	}
	
	sqlite3_finalize(preparedStatement);						// Free resources
	
	if(shares!=(double)0) money = shares*price_on_last_day*provizija;										// Sell everything at the end
	double buyhold = (price_on_last_day/price_on_first_day)*provizija*provizija*(double)iInvesticija;		// Buy and hold strategy
	
	if(trading_log) {
		std::string msg = "Trgovano dionicom: " + aContext.dionica;
		Beagle_LogBasicM(ioContext.getSystem().getLogger(),"eval", "eval",msg);
		msg = "Price at the beginning of the interval: " + to_string<double>(price_on_first_day) + 
			", at the end of the interval: " + to_string<double>(price_on_last_day);
		Beagle_LogBasicM(ioContext.getSystem().getLogger(),"eval", "eval",msg);
		msg = "Profit: generated rule: " + to_string<double>(money - iInvesticija) +
			", buy_hold rule: " + to_string<double>(buyhold - iInvesticija);
		Beagle_LogBasicM(ioContext.getSystem().getLogger(),"eval", "eval",msg);
	}

	sqlite3_close(database);		// Close the database

	if(!traded)						// Penalize the rules that don't trade
		return (double)0;
	else if(money<iInvesticija)		// Or the ones that are bad (lose money)
		return (double)0;
	else
		return money/buyhold;
}

Fitness::Handle eval::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{ 
	sqlite3 *database;
	int rc;
	
	SQLITE_OPEN(rc,"",database)
	
	double interval_divider = 0.5;

	// Calculate the date that splits the dataset into 2 sets (training and validation)
	string sql  = "SELECT DATE(DATETIME(";
	sql        += "(STRFTIME('%s','" + start_date->getWrappedValue() + "') +";
	sql        += "(STRFTIME('%s','" + end_date->getWrappedValue() + "') - STRFTIME('%s','" + start_date->getWrappedValue() + "'))*" + to_string<double>(interval_divider) + ")";
	sql        += ",'unixepoch'))";
	
//	std::cout << sql << std::endl;
	
	sqlite3_stmt *preparedStatement;
	SQLITE_PREPARE_STATEMENT_TEST(preparedStatement, sql, database);
	SQLITE_STEP_TEST(preparedStatement);
	std::string  intermediate_date = (const char *)sqlite3_column_text(preparedStatement,0);
	sqlite3_finalize(preparedStatement);	// Free resources
	sqlite3_close(database);				// Close the database

	// Training set
	interval_start = start_date->getWrappedValue();		// Starting date
	interval_end   = intermediate_date;
	double rule_train		= evaluate_interval(inIndividual, ioContext);
	
	// Validation set
	interval_start = intermediate_date;
	interval_end   = end_date->getWrappedValue();		// End date
	double rule_validation  = evaluate_interval(inIndividual, ioContext);
		
	FitnessMultiObj::Handle lFitness = new FitnessMultiObj(2);	// 2 fitness values
	(*lFitness)[0] = rule_train;								// Fitness on the train set
	(*lFitness)[1] = rule_validation;							// Fitness on the validation set
	
//	std::cout << start_date->getWrappedValue() << " - " << intermediate_date << " - " << end_date->getWrappedValue() << std::endl;
//	std::cout << "Training set: " << to_string<double>(rule_train) << ", validation set: " << to_string<double>(rule_validation) << std::endl;
	
	return lFitness;
}

void eval::postInit(System& ioSystem)
{
  GP::EvaluationOp::postInit(ioSystem); 
}

