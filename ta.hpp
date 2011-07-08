/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef ta_hpp
#define ta_hpp

#include <sqlite3.h>
#include <iostream>

double sma(sqlite3 *database, std::string ticker, std::string date, int days);
double ema(sqlite3 *database, std::string ticker, std::string date, int days);
double price_on_day(sqlite3 *database, std::string ticker, std::string date);
double price_days_ago(sqlite3 *database, std::string ticker, std::string date, int days);
std::string date_days_ago(sqlite3 *database, std::string ticker, std::string date, int days);


#endif // ta_hpp
