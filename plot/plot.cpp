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
	//cout << "Dostupne dionice u bazi podataka:" << endl;
	sqlite3 *database;
	
	int rc = sqlite3_open("se.db", &database);	// Otvori bazu podataka
	if( rc ) {
		fprintf(stderr, "Ne mogu otvoriti bazu podataka! : %s\n", sqlite3_errmsg(database));
		sqlite3_close(database);
		exit(1);
	}
	
	string sql;
	sqlite3_stmt *preparedStatement,*preparedStatement2;
	sql = "SELECT DISTINCT(DIONICA) FROM ZSE";
	if ( sqlite3_prepare( database, sql.c_str(), sql.size(), &preparedStatement, NULL ) != SQLITE_OK )
		throw "Ne mogu prirediti SQL upit!\n";

	ofstream cmd;
	cmd.open("crtaj");
	while ((rc = sqlite3_step(preparedStatement)) == SQLITE_ROW ) {
		string dionica = (const char*)sqlite3_column_text(preparedStatement,0);
		//string sql2  = "SELECT DATE(DATUM),PRVA,NAJVISA,NAJNIZA,ZADNJA FROM ZSE WHERE DIONICA='" + dionica + "';"; 
		string sql2  = "SELECT DATE(DATUM),PROSJECNA FROM ZSE WHERE DIONICA='" + dionica + "';"; 
		sqlite3_prepare( database, sql2.c_str(), sql2.size(), &preparedStatement2, NULL );
		string gplot;
		gplot += "set terminal png                   \n";
		//gplot += "set xtics 10                       \n";
		gplot += "set xlabel  \"Datum:\"             \n";
		gplot += "set ylabel  \"Cijena:\"            \n";
		gplot += "set timefmt \"%Y-%m-%d\"           \n";
		gplot += "set xdata time                     \n";
		gplot += "set format x \"%m/%y\"             \n";
		gplot += "set output \"" + dionica + ".png\" \n";
		gplot += "plot [\"2006-01-01\":] '" + dionica + ".dat'     ";
		gplot += "using 1:2 title \"" + dionica + "\" with lines \n";
		//gplot += "using 1:2:3:4:5 title \"" + dionica + "\" with financebars \n";
		cmd << gplot;

		string filename = dionica + ".dat";
		ofstream dataout;
		dataout.open(filename.c_str());
		while ((rc = sqlite3_step(preparedStatement2)) == SQLITE_ROW ) {
			string datum     = (const char*)sqlite3_column_text(preparedStatement2,0);
			string prva    = (const char*)sqlite3_column_text(preparedStatement2,1);
			//string najvisa = (const char*)sqlite3_column_text(preparedStatement2,2);
			//string najniza = (const char*)sqlite3_column_text(preparedStatement2,3);
			//string zadnja  = (const char*)sqlite3_column_text(preparedStatement2,4);
			//dataout << datum << "\t" << prva << "\t" << najvisa << "\t" << najniza << "\t" << zadnja << endl;
			dataout << datum << "\t" << prva << endl;
		}
		dataout.close();
		sqlite3_finalize(preparedStatement2); // Oslobodi zauzete resurse
	}
	cmd.close();
	sqlite3_finalize(preparedStatement);	// Oslobodi zauzete resurse
	sqlite3_close(database);	// Zatvori bazu podataka
	system ("wgnuplot crtaj");
	//system ("del *.dat");
	return 0;
}

