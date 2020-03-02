#include "CRandom.h"

//-------------------------konstruktory / destruktor------------------------

//konstruktor domyœlny ustawia seed na podstawie aktualnego czasu
CRandom::CRandom()
{
	mt_mersenne_twister_engine = std::mt19937(rd_device());
}

CRandom::CRandom(int iSeed)
{
	mt_mersenne_twister_engine = std::mt19937(rd_device());
	mt_mersenne_twister_engine.seed(iSeed);
}

CRandom::~CRandom()
{
	//nic siê nie dzieje tu
}

//-------------------------losowanie int'ów---------------------------------

int CRandom::iNextInt(int iMin, int iMax)
{
	return ulNextUnsignedLong((unsigned long) iMin, (unsigned long) iMax);
}

//-------------------------losowanie double'i-------------------------------

double CRandom::dNextDouble(double dMin, double dMax)
{
	if (dMin == dMax)
	{
		return dMax;
	}
	else
	{
		std::uniform_real_distribution<double> dist(dMin, dMax);
		return dist(mt_mersenne_twister_engine);
	}
	
}

//-------------------------losowanie unsigned long---------------------------------

unsigned long CRandom::ulNextUnsignedLong(unsigned long ulMin, unsigned long ulMax)
{
	if(ulMin == ulMax)
	{
		return ulMin;
	}
	else
	{
		std::uniform_int_distribution<std::mt19937::result_type> dist(ulMin, ulMax - 1);
		return dist(mt_mersenne_twister_engine);
	}
}


//-------------------------gettery i settery--------------------------------

int CRandom::iGetSeed()
{
	return i_seed;
}

void CRandom::vSetSeed(int iNewSeed)
{
	i_seed = iNewSeed;
	mt_mersenne_twister_engine.seed(iNewSeed);
}