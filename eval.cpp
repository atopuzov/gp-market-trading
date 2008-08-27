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
		dionica = new String("PBZ-R-A");
		Register::Description lDescription(
			"Dionica",
			"String",
			"PBZ-R-A",
			"Dionica kojom se trguje");
		ioSystem.getRegister().addEntry("aco.dionica", dionica, lDescription);
	}
	
	// Pocetni datum trgovanja
	if(ioSystem.getRegister().isRegistered("aco.pdatum")) {
		pocetniDatum = castHandleT<String>(ioSystem.getRegister()["aco.pdatum"]);
	} else {
		pocetniDatum = new String("2006-01-01");
		Register::Description lDescription(
			"pDatum",
			"String",
			"2006-01-01",
			"Pocetni datum trgovanja");
		ioSystem.getRegister().addEntry("aco.pdatum", pocetniDatum, lDescription);
	}
	// Zavrsni datum trgovanja
	if(ioSystem.getRegister().isRegistered("aco.zdatum")) {
		zavrsniDatum = castHandleT<String>(ioSystem.getRegister()["aco.zdatum"]);
	} else {
		zavrsniDatum = new String("now");
		Register::Description lDescription(
			"zDatum",
			"String",
			"now",
			"Zavrsni datum trgovanja");
		ioSystem.getRegister().addEntry("aco.zdatum", zavrsniDatum, lDescription);
	}
}

Fitness::Handle eval::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{ 
    Beagle::GP::aco::Context& aContext = castObjectT<Beagle::GP::aco::Context&>(ioContext);
    
    aContext.dionica = dionica->getWrappedValue();
    
    //std::cout << aContext.dionica << std::endl;

    sqlite3 *database;													// Baza podataka
    int rc = sqlite3_open(baza->getWrappedValue().c_str(), &database);	// Otvori bazu podataka
    
	if( rc ) {
		sqlite3_close(database);
		std::cerr << "Ne mogu otvoriti bazu podataka! " << sqlite3_errmsg(database) << std::endl;
		throw "Ne mogu otvoriti bazu podataka!";
	}
	aContext.database = database;

	// SQL Upit
	string sql  = "SELECT DATUM,ZADNJA,KOLICINA FROM ZSE WHERE DIONICA=UPPER('";
	sql        += dionica->getWrappedValue();				// Dionica
	sql        += "') AND DATE(DATUM) BETWEEN DATE('";
	sql        += pocetniDatum->getWrappedValue();			// Pocetni datum
	sql        += "') AND DATE('";
	sql        += zavrsniDatum->getWrappedValue();			// Zavrsni datum
	sql        += "') ORDER BY DATE(DATUM) ASC";
	
	//std::cout << sql << std::endl;
	
	sqlite3_stmt *preparedStatement;	// Upit na bazu podataka
	if ( sqlite3_prepare( database, sql.c_str(), sql.size(), &preparedStatement, NULL ) != SQLITE_OK )
		throw "Ne mogu prirediti SQL upit!";
	
    bool naTrzistu = false, trgovao=false;		// Van trzista
    double provizija = 0.995;					// Faktor kojim racunalo proviziju trgovanja
    double iInvesticija = 100000.;				// Inicijalna investicija
    double dionica = 0, novaca = iInvesticija;	// Pocetne vrjedosti
    double prva = 0, zadnja = 0;				// Cjena na pocetku razdoblja i cjena na kraju razdoblja
    
    
	while ((rc = sqlite3_step(preparedStatement)) == SQLITE_ROW ) {	// Prodji kroz sve zapise
	
		aContext.datum = (const char *)sqlite3_column_text(preparedStatement,0);	// Trenutni datum
		
		double vrjednost = sqlite3_column_double(preparedStatement,1);
		setValue("C", (Beagle::Double)vrjednost, ioContext);		// Postavi cjenu
		
		double kolicina = sqlite3_column_double(preparedStatement,2);
		setValue("V", (Beagle::Double)kolicina, ioContext);		// Postavi kolicinu
		
		zadnja = vrjednost;
		//std::cout << "Vrjednost: " << vrjednost << ", datum:" << aContext.datum << std::endl;

		Bool lResult;
		inIndividual.run(lResult, ioContext);			// Pokreni pravilo
		if(lResult && !naTrzistu)  {					// Kupi
	    	dionica = (novaca*provizija)/vrjednost;
	    	//std::cout << "Kupi:   " << dionica << " Novaca: " << novaca << std::endl;
	    	novaca  = 0;
	    	naTrzistu  = true;
	    	trgovao = true;
    	} else if (!lResult && naTrzistu ) {			// Prodaj
    		novaca = (dionica*vrjednost)*provizija;
    		//std::cout << "Prodaj: " << dionica << " Novaca: " << novaca << std::endl;
    		dionica = 0;
    		naTrzistu  = false;
    	}
    	if(prva==(double)0) prva = vrjednost;
	}
	sqlite3_finalize(preparedStatement);	// Oslobodi resurse SQL upita
	if(dionica!=(double)0) novaca = dionica*zadnja*provizija;			// Prodaj sve

	double buyhold = (zadnja/prva)*provizija*(double)iInvesticija;		// Buy and hold strategija
	//std::cout << "Prva:" << prva << " Zadnja:" << zadnja << " Dionica:" << dionica << " Novaca:" << novaca << " B&h:" << buyhold << std::endl;

	sqlite3_close(database);	// Zatvori bazu
	
	if(!trgovao)
		return new FitnessSimple(0.);			// Kaznjavaju se jedinke koje nisu trgovale
	else
	return new FitnessSimple(novaca/buyhold);
}

void eval::postInit(System& ioSystem)
{
  GP::EvaluationOp::postInit(ioSystem); 
}

