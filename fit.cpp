/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#include "fit.hpp"
#include <cfloat>
#include <cmath>
#include <algorithm>

using namespace trading;

FitnessTrading::FitnessTrading() :
	Beagle::FitnessSimple(false)
{	}


FitnessTrading::FitnessTrading(float inFitness) :
	Beagle::FitnessSimple(true)
{
	setValue(inFitness);
}

FitnessTrading::FitnessTrading(float inFitness1, float inFitness2) :
	Beagle::FitnessSimple(true)
{
	setValue(inFitness1, inFitness2);
}

void FitnessTrading::setValue(float inFitness)
{
	setValue(inFitness, 0);
}

void FitnessTrading::setValue(float inFitness1, float inFitness2)
{
	// Training set fitness
	if(Beagle::isFinite(inFitness1)) mFitness = inFitness1;
	else mFitness = FLT_MIN;
	// Validation set fitness
	if(Beagle::isFinite(inFitness2)) vsFitness = inFitness2;
	else vsFitness = FLT_MIN;
	setValid();
}

void FitnessTrading::read(PACC::XML::ConstIterator inIter)
{
	if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Fitness"))
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Fitness> expected!");

	std::string lType = inIter->getAttribute("type").c_str();
	if(lType != "trading") throw Beagle_IOExceptionNodeM(*inIter, "fitness type mismatch!");

	for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() == PACC::XML::eData) {
			if(lChild->getValue() == "FitnessTS") {			// Fitness on training set
				PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
				if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no normalized fitness value present!");
				if(lChild2->getType() != PACC::XML::eString)
					throw Beagle_IOExceptionNodeM(*lChild2, "no normalized fitness value present!");
				mFitness = Beagle::str2dbl(lChild2->getValue().c_str());
			} else if(lChild->getValue() == "FitnessVS") {	// Fitness on validation set
				PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
				if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no adjusted fitness value present!");
				if(lChild2->getType() != PACC::XML::eString)
					throw Beagle_IOExceptionNodeM(*lChild2, "no adjusted fitness value present!");
				vsFitness = Beagle::str2dbl(lChild2->getValue().c_str());
			}
		}
	}
}

void FitnessTrading::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	ioStreamer.openTag("Fitness", inIndent);
	ioStreamer.insertAttribute("type", "trading");
	ioStreamer.openTag("FitnessTS", false);		// Fitness on training set
	ioStreamer.insertStringContent(Beagle::dbl2str(mFitness).c_str());
	ioStreamer.closeTag();
	ioStreamer.openTag("FitnessVS", false);		// Fitness on validation set
	ioStreamer.insertStringContent(Beagle::dbl2str(vsFitness).c_str());
	ioStreamer.closeTag();
	ioStreamer.closeTag();
}
