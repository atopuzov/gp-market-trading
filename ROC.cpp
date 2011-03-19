/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "beagle/GP.hpp"
#include "ROC.hpp"

#include <sstream>
#include <cmath>
#include <fstream>

using namespace Beagle;

// Ime ROC
ROC::ROC() :
  Beagle::GP::Primitive(1, "ROC")
{ }

#ifdef BEAGLE_HAVE_RTTI

// Argumenti tipa Double
const std::type_info* ROC::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
  Beagle_AssertM(inN < 1);
  return &typeid(UInt);
}

// Povratna vrjednost double
const std::type_info* ROC::getReturnType(Beagle::GP::Context& ioContext) const
{
  return &typeid(Double);
}

#endif // BEAGLE_HAVE_RTTI

// Vraca srednju vrjednost dionice kroz period
void ROC::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Double& lResult = castObjectT<Double&>(outDatum);
	UInt lArg;
	get1stArgument(lArg, ioContext); 
	Beagle::GP::aco::Context& aContext = castObjectT<Beagle::GP::aco::Context&>(ioContext);

	sqlite3 *database;		// Baza podataka  
	database = aContext.database;

	std::ostringstream interval;
	interval << lArg;

	// Closing price n days ago
	string sql = "SELECT ZADNJA FROM ZSE WHERE DIONICA=UPPER('";
	sql       += aContext.dionica;				// Dionica
	sql       += "') AND DATUM BETWEEN DATE('";
	sql       += aContext.datum;
	sql       += "','-"; 
	sql       += interval.str();				// Vremenski interval
	sql       += " day') AND DATE('";
	sql       += aContext.datum;				// Trenutni datum
	sql       += "') ORDER BY DATUM ASC LIMIT 1"; 
	// Vrati interval, pa koristi najblizi dan (ukoliko odbarani dan ispadne neradni, nema rezultata)
 
	// Closing price of current day
	string sql2 = "SELECT ZADNJA FROM ZSE WHERE DIONICA=UPPER('";
	sql2       += aContext.dionica;				// Dionica
	sql2       += "') AND DATUM = DATE('";
	sql2       += aContext.datum;
	sql2       += "')"; 


	//std::cout << sql << "\n\n" << sql2 << std::endl;

	sqlite3_stmt *preparedStatement;
	SQLITE_PREPARE_STATEMENT_TEST(preparedStatement, sql, database);
	SQLITE_STEP_TEST(preparedStatement);
	double close_ndaysago = sqlite3_column_double(preparedStatement,0);	// Closing price n days ago
	sqlite3_finalize(preparedStatement);
	
	sqlite3_stmt *preparedStatement2;	
	SQLITE_PREPARE_STATEMENT_TEST(preparedStatement2, sql2, database);
	SQLITE_STEP_TEST(preparedStatement2);
	double close_today = sqlite3_column_double(preparedStatement2,0);	// Closing price of current day
	sqlite3_finalize(preparedStatement2);
	
	lResult = (close_today/close_ndaysago-1.)*100.;
	
//	std::cout << "ROC:" << lResult << std::endl;
}
