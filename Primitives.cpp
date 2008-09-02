#include "Primitives.hpp"

void AddPrimitives(Beagle::GP::PrimitiveSet::Handle lSet)
{
	lSet->insert(new LessThan);						// Manje od
	lSet->insert(new GreaterThan);					// Vece od
	lSet->insert(new IfThenElse);					// If-Then-Else
	lSet->insert(new Eq);							// Jednako
	
	lSet->insert(new Beagle::GP::Add);						// Zbrajanje
	lSet->insert(new Beagle::GP::Subtract);					// Oduzimanje
	lSet->insert(new Beagle::GP::Multiply);					// Mnozenje
	lSet->insert(new Beagle::GP::Divide);					// Djeljenje
	
	lSet->insert(new Beagle::GP::And);						// Logicko i
	lSet->insert(new Beagle::GP::Or);						// Logicko ili
	lSet->insert(new Beagle::GP::Not);						// Logicko ne
	
	lSet->insert(new Beagle::GP::TokenT<Beagle::Bool>("True",  Beagle::Bool(true ))); 	// Istina
	lSet->insert(new Beagle::GP::TokenT<Beagle::Bool>("False", Beagle::Bool(false))); 	// Laz
	
	lSet->insert(new Beagle::GP::EphemeralDouble);			// Konstanta [-1,1]
	lSet->insert(new EphemeralDay);					// Konstanta [0,365] za AVG, MIN, MAX
	lSet->insert(new ConstantDouble);
	lSet->insert(new Beagle::GP::TokenT<Beagle::Double>("C"));		// Cijena
	lSet->insert(new Beagle::GP::TokenT<Beagle::Double>("V"));		// Kolicina
	
	lSet->insert(new Avg);							// Srednja vrijednost cijene
	lSet->insert(new Min);							// Minimalna vrijednost cijene
	lSet->insert(new Max);							// Maksimalna vrijednost cijene
	lSet->insert(new ROC);							// Rate of change
}
