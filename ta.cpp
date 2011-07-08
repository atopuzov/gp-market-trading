/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "ta.hpp"
#include "util.hpp"

//	Simple moving average
double sma(sqlite3 *database, std::string ticker, std::string date, int days)
{
	// Simple moving average
	std::string sql	 = "SELECT AVG(ZADNJA) FROM ZSE WHERE DIONICA=UPPER('";
	sql			+= ticker;
	sql			+= "') AND DATUM IN (";
	sql			+= "SELECT DATUM FROM ZSE WHERE DATUM < '";
	sql			+= date;
	sql			+= "' LIMIT "; 
	sql			+= to_string<int>(days);
	sql			+= ")";

	//std::cout << sql  << std::endl;

	sqlite3_stmt *preparedStatement;
	SQLITE_PREPARE_STATEMENT_TEST(preparedStatement, sql, database);
	SQLITE_STEP_TEST(preparedStatement);
	double sma = sqlite3_column_double(preparedStatement,0);	// Simple moving average
	sqlite3_finalize(preparedStatement);

	//std::cout << "SMA: " << sma << std::endl;

	return sma;
}

// Exponential moving average
double ema(sqlite3 *database, std::string ticker, std::string date, int days)
{
	if(days == 1) {
		return sma(database, ticker, date, days);
	}
	double multiplier = (double)2 / ((double)days + (double)1);
	
	// std::cout << "Multiplier: " << multiplier << ",\t Days: " << days << std::endl;
		
	std::string previous_date = date_days_ago(database, ticker, date, 1);
	double ema_previous_day = ema(database, ticker, previous_date, days-1);
	double price_this_day   = price_on_day(database, ticker, date);
	
	return ema_previous_day + multiplier*(price_this_day - ema_previous_day);
}

// Price on a given day
double price_on_day(sqlite3 *database, std::string ticker, std::string date)
{
	std::string sql	 = "SELECT DATUM FROM ZSE WHERE DIONICA=UPPER('";
	sql				+= ticker;
	sql				+= "') AND DATUM = '";
	sql				+= date;
	sql				+= "'";
	
	//std::cout << sql  << std::endl;
	
	sqlite3_stmt *preparedStatement;
	SQLITE_PREPARE_STATEMENT_TEST(preparedStatement, sql, database);
	SQLITE_STEP_TEST(preparedStatement);
	double price = sqlite3_column_double(preparedStatement,0);
	sqlite3_finalize(preparedStatement);
	
	//std::cout << "Price: " << price << std::endl;
	
	return price;
}

double price_days_ago(sqlite3 *database, std::string ticker, std::string date, int days)
{
	return price_on_day(database, ticker, date_days_ago(database, ticker, date, days));
}

// Date days ago
std::string date_days_ago(sqlite3 *database, std::string ticker, std::string date, int days)
{
	std::string sql	 = "SELECT DATUM FROM ZSE WHERE DIONICA=UPPER('";
	sql				+= ticker;
	sql				+= "') AND DATUM < '";
	sql				+= date;
	sql				+= "' ORDER BY DATUM DESC LIMIT ";
	sql				+= to_string<int>(days-1);
	sql				+= ", 1";

//	std::cout << sql  << std::endl;
	
	sqlite3_stmt *preparedStatement;
	SQLITE_PREPARE_STATEMENT_TEST(preparedStatement, sql, database);
	SQLITE_STEP_TEST(preparedStatement);
	std::string date_days_ago = reinterpret_cast<const char*>(sqlite3_column_text(preparedStatement,0));
	sqlite3_finalize(preparedStatement);
	
//	std::cout << "Date: " << date_days_ago << std::endl;
	
	return date_days_ago;
}
