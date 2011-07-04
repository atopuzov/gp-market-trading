/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <string>
#include <fstream>
#include "sqlite3.h"

using namespace std;

int main()
{
	sqlite3 *database;
	
	int rc = sqlite3_open("se.db", &database);	// Open the database
	if( rc ) {
		fprintf(stderr, "Unable to open the database se.db! : %s\n", sqlite3_errmsg(database));
		sqlite3_close(database);
		exit(1);
	}
	
	// Find out what tickers are available
	sqlite3_stmt *ps_available_tickers;
	string sql_available_tickers = "SELECT DISTINCT(DIONICA) FROM ZSE WHERE DIONICA != 'ZSE'";
	if ( sqlite3_prepare( database, sql_available_tickers.c_str(), sql_available_tickers.size(), &ps_available_tickers, NULL ) != SQLITE_OK )
		throw "Unable top prepare SQL query!\n";

	// Set the basic gnuplot options
	ofstream cmd;
	cmd.open("draw.gnuplot");
	string gplot;
	gplot += "set terminal png\n";
	gplot += "set grid\n";
	gplot += "set border\n";
	//gplot += "set xtics 10\n";
	gplot += "set xlabel  \"Datum:\"\n";
	gplot += "set ylabel  \"Cijena:\"\n";
	gplot += "set timefmt \"%Y-%m-%d\"\n";
	gplot += "set xdata time                     \n";
	gplot += "set format x \"%m/%y\"             \n";
	cmd << gplot;
	
	// Loop trough the available tickers
	while ((rc = sqlite3_step(ps_available_tickers)) == SQLITE_ROW ) {
		string ticker = (const char*)sqlite3_column_text(ps_available_tickers,0);
		
		// Select all the data
		string sql_select_data  = "SELECT DATE(DATUM),PRVA,NAJVISA,NAJNIZA,ZADNJA FROM ZSE WHERE DIONICA='" + ticker + "';"; 		
		//string sql_select_data  = "SELECT DATE(DATUM),ZADNJA FROM ZSE WHERE DIONICA='" + ticker + "';"; 
		sqlite3_stmt *ps_select_data;
		sqlite3_prepare( database, sql_select_data.c_str(), sql_select_data.size(), &ps_select_data, NULL );
		
		// Find out what is the lowest date in the database
		sqlite3_stmt *ps_lowest_date;
		string sql_lowest_date = "SELECT MIN(DATE(DATUM)) FROM ZSE WHERE DIONICA='" + ticker + "';"; 
		sqlite3_prepare( database, sql_lowest_date.c_str(), sql_lowest_date.size(), &ps_lowest_date, NULL );
		string min_datum;
		if( (rc =sqlite3_step(ps_lowest_date)) == SQLITE_ROW ) {
			min_datum  = (const char*)sqlite3_column_text(ps_lowest_date,0);
		} else {
			min_datum  = "2000-01-01";		// Set a default date on error
		}
		sqlite3_finalize(ps_lowest_date);	// Free resources

		gplot =  "set output \"" + ticker + ".png\" \n";
		gplot += "plot [\"" + min_datum + "\":] '" + ticker + ".dat'     ";
		//gplot += "using 1:2 title \"" + ticker + "\" with lines \n";
		//gplot += "using 1:2:3:4:5 title \"" + ticker + "\" with financebars \n";
		gplot += "using 1:2:3:4:5 title \"" + ticker + "\" with candlesticks \n";
		cmd << gplot;
		string filename = ticker + ".dat";
		
		ofstream dataout;
		dataout.open(filename.c_str());
		
		// Loop trough the data and output it ta a file
		while ((rc = sqlite3_step(ps_select_data)) == SQLITE_ROW ) {
			string date  = (const char*)sqlite3_column_text(ps_select_data,0);
			string open  = (const char*)sqlite3_column_text(ps_select_data,1);
			string high  = (const char*)sqlite3_column_text(ps_select_data,2);
			string low   = (const char*)sqlite3_column_text(ps_select_data,3);
			string close = (const char*)sqlite3_column_text(ps_select_data,4);
			dataout << date << "\t" << open << "\t" << low << "\t" << high << "\t" << close << endl;
			//dataout << datum << "\t" << prva << endl;
		}
		dataout.close();					// Close the file with data
		sqlite3_finalize(ps_select_data);	// Free resources
	}
	cmd.close();							// Close the gnuplot command file
	sqlite3_finalize(ps_available_tickers);	// Free resources
	sqlite3_close(database);				// Close the database
	return 0;
}

