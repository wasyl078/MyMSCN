#pragma once
#include "CMscnSolution.h"
#include <vector>
#include <fstream>

class CMscnProblem;

class CAlgorithm
{
protected:

	CMscnProblem *pc_problem;
	
	int i_seed;

	bool b_print_results;

	std::vector<double> vec_min_qualities;
	std::vector<double> vec_max_qualities;
	std::vector<double> vec_average_qualities;
	
public:
	//-------------------------ewolucja ró¿nicowa / przeszukiwanie losowe---------------------------

	virtual CMscnSolution * run(CMscnProblem *pc_problem) = 0;
	
	//-------------------------------zapisywanie do pliku .csv--------------------------------------

	bool bSaveIntoCsvFile(std::string sFileName, int iFirstColumn);

	//-------------------------------wypisywanie statystyk------------------------------------------

	void vPrintStatistics();
	
	//----------------------------------gettery i settery-------------------------------------------
};