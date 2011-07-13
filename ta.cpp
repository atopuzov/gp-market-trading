/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include <beagle/macros.hpp>
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
	sql			+= Beagle::int2str(days);
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
double ema_r(sqlite3 *database, std::string ticker, std::string date, int days, int day)
{
	if(day == 1) {
		return sma(database, ticker, date, days);
	}
	double multiplier = (double)2 / ((double)days + (double)1);
	
	// std::cout << "Multiplier: " << multiplier << ",\t Days: " << days << std::endl;
		
	std::string previous_date = date_days_ago(database, ticker, date, 1);
	double ema_previous_day   = ema_r(database, ticker, previous_date, days, day-1);
	double price_this_day     = price_on_day(database, ticker, date);
	
	return ema_previous_day + multiplier*(price_this_day - ema_previous_day);
}

double ema(sqlite3 *database, std::string ticker, std::string date, int days)
{
	return ema_r(database, ticker, date, days, days);
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

double price_on_previous_day(sqlite3 *database, std::string ticker, std::string date)
{
	return price_on_day(database, ticker, date_days_ago(database, ticker, date, 1));
}

// Date days ago
std::string date_days_ago(sqlite3 *database, std::string ticker, std::string date, int days)
{
	std::string sql	 = "SELECT DATUM FROM ZSE WHERE DIONICA=UPPER('";
	sql				+= ticker;
	sql				+= "') AND DATUM < '";
	sql				+= date;
	sql				+= "' ORDER BY DATUM DESC LIMIT ";
	sql				+= Beagle::int2str(days-1);
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

// Calculates average gain and avergae loss
std::pair<double,double> avg_gain_loss_r(sqlite3 *database, std::string ticker, std::string date, int days, int day)
{
	double gain = 0, loss = 0;
	if(day == 1)
	{
		std::string sql	 = "SELECT ZADNJA FROM ZSE WHERE DIONICA=UPPER('";
		sql				+= ticker;
		sql				+= "') AND DATUM < '";
		sql				+= date;
		sql				+= "' ORDER BY DATUM DESC LIMIT ";
		sql				+= Beagle::int2str(days);
	
//		std::cout << sql << std::endl;
	
		sqlite3_stmt *preparedStatement;
		SQLITE_PREPARE_STATEMENT_TEST(preparedStatement, sql, database);
	
		double price_t_1 = 0;
		int rc;
	
		while ( (rc = sqlite3_step(preparedStatement)) == SQLITE_ROW ) {        // Loop trough all the records
			double price = sqlite3_column_double(preparedStatement,0);
			if(price_t_1 == 0) {
				price_t_1 = price;
				continue;
			}
			if(price > price_t_1)
			{
				gain += price - price_t_1;
			} else {
				loss += price_t_1 - price;
			}
			price_t_1 = price;
		}
	
		double avg_gain = gain / (double)days;
		double avg_loss = loss / (double)days;
		//std::cout << "avg_gain: " << avg_gain << ", avg_loss: " << avg_loss << std::endl;
		return	std::pair<double,double>(avg_gain, avg_loss);
	}
	std::pair<double,double> prev = avg_gain_loss_r(database, ticker, date, days, day-1);
	double price          = price_on_day(database, ticker, date);
	double previous_price = price_on_previous_day(database, ticker, date);
	if(price > previous_price)
	{
		gain = price - previous_price;
	} else {
		loss = previous_price - price;
	}
	
	double avg_gain = (prev.first  * (double)(days-1) + gain) / (double)days;
	double avg_loss = (prev.second * (double)(days-1) + loss) / (double)days;
	//std::cout << "avg_gain: " << avg_gain << ", avg_loss: " << avg_loss << std::endl;
	return std::pair<double,double>(avg_gain, avg_loss);
}

std::pair<double,double> avg_gain_loss(sqlite3 *database, std::string ticker, std::string date, int days)
{
	return avg_gain_loss_r(database, ticker, date, days, days);
}










