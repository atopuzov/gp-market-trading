#include "beagle/GP.hpp"
#include "eval.hpp"


#include <cmath>

using namespace Beagle;

/*!
 *  \brief Construct a new symbolic regression evaluation operator.
 */
eval::eval() :
  GP::EvaluationOp("eval")
{ }

void eval::initialize(Beagle::System& ioSystem)
{
	Beagle::GP::EvaluationOp::initialize(ioSystem);
	
	// Ime baze podataka
	if(ioSystem.getRegister().isRegistered("aco.baza")) {
		baza = castHandleT<String>(ioSystem.getRegister()["aco.baza"]);
	} else {
		baza = new String("se.db");
		Register::Description lDescription(
			"Baza podataka",
			"String",
			"se.db",
			"Ime datoteke s bazom podataka.");
		ioSystem.getRegister().addEntry("aco.baza", baza, lDescription);
	}
	
	// Ime dionice
	if(ioSystem.getRegister().isRegistered("aco.dionica")) {
		dionica = castHandleT<String>(ioSystem.getRegister()["aco.dionica"]);
	} else {
		dionica = new String("ZABA-R-A");
		Register::Description lDescription(
			"Dionica",
			"String",
			"ZABA-R-A",
			"Dionica");
		ioSystem.getRegister().addEntry("aco.dionica", dionica, lDescription);
	}
}


Fitness::Handle eval::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{ 
    Bool lResult;

    sqlite3 *database;		// Baza podataka
//    (specContext)ioContext.special = 10;
    
    return new FitnessSimple(1);
    
    int rc = sqlite3_open(baza->getWrappedValue().c_str(), &database);	// Otvori bazu podataka
	if( rc ) {
		fprintf(stderr, "Ne mogu otvoriti bazu podataka! : %s\n", sqlite3_errmsg(database));
		sqlite3_close(database);
		exit(1);
	}
	string sql;
	sqlite3_stmt *preparedStatement;
	
	sql  = "SELECT ZADNJA FROM ZSE WHERE DIONICA='";
	//sql += dionica->getWrappedValue();
	sql += "ERNT-R-A";
//	sql += "' ORDER BY date(DATUM) ASC";
	sql += "' AND date(DATUM) > date('2005-02-01') ORDER BY date(DATUM) ASC";
	
	if ( sqlite3_prepare( database, sql.c_str(), sql.size(), &preparedStatement, NULL ) != SQLITE_OK )
		throw "Ne mogu prirediti SQL upit!\n";

    Bool stanje = false;	// Van trzista
    Double dionica = 0, novaca = 10000;
    Double prva = 0, zadnja = 0;
	while ((rc = sqlite3_step(preparedStatement)) == SQLITE_ROW ) {
		double vrjednost = (double)sqlite3_column_int(preparedStatement,0);

		zadnja = vrjednost;
		setValue("C", (Beagle::Double)vrjednost, ioContext);
		inIndividual.run(lResult, ioContext);
		
		//std::cout << "Vrjednost:" << vrjednost << std::endl;

	    if(lResult && !stanje)  {		// Kupi
	    	double d = novaca/vrjednost;
	    	novaca  =  0;
	    	dionica += d;
	    	stanje = true;
//	    	std::cout << "Kupi:" << d << std::endl;
    	} else if (!lResult && stanje ) {			// Prodaj
    		double n = dionica*vrjednost;
    		dionica = 0;
    		novaca += n;
    		stanje = false;
//	    	std::cout << "Prodaj:" << dionica << std::endl;
    	} else if (!lResult and !stanje ) {
	    	//std::cout << "Van burze i van burze"  << std::endl;
    	} else if (lResult && stanje ) {
	    	//std::cout << "Na burze i na burzu"  << std::endl;
    	}
    	if(prva==(double)0) {
			prva = vrjednost;
			if(dionica==(double)0) {
				double d = novaca/vrjednost;
		    	novaca  =  0;
		    	dionica += d;
		    	stanje = true;
	    	}
		}
	}
	sqlite3_close(database);	// Zatvori bazu
	novaca += dionica*zadnja;
//	double bh = (zadnja-prva)*(double)10000;
//	std::cout << "Prva: " << prva << " zadnja:" << zadnja <<  " BH:" << bh <<std::endl;
//	std::cout << "Igra: " << novaca << std::endl;
//  return new FitnessSimple((double)novaca);
}

void eval::postInit(System& ioSystem)
{
  GP::EvaluationOp::postInit(ioSystem); 
}

