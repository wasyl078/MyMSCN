#pragma once
#include <iostream>
#include <random>

class CRandom
{
private:

	//-------------------------prywatne zmienne---------------------------------

	int i_seed;

	std::random_device rd_device;

	std::mt19937 mt_mersenne_twister_engine;
	
public:

	//-------------------------konstruktory / destruktor------------------------
	CRandom();

	CRandom(int iSeed);

	~CRandom();

	//-------------------------losowanie int'ów---------------------------------

	int iNextInt(int iMin, int iMax);

	//-------------------------losowanie double'i-------------------------------

	double dNextDouble(double dMin, double dMax);

	//-------------------------losowanie unsigned long---------------------------------

	unsigned long ulNextUnsignedLong(unsigned long ulMin, unsigned long ulMax);
	
	//-------------------------gettery i settery--------------------------------

	int iGetSeed();

	void vSetSeed(int iNewSeed);
};