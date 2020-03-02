#pragma once
#include <iostream>
#include <climits>
#include "CAlgorithm.h"
#include "CMscnProblem.h"
#include <fstream>

class CRandomSearch : public CAlgorithm
{
private:

	//--------------------prywatne zmienne / sta³e i metody pomocnicze-----------------------------

	const static int I_NO_ERROR;

	const static int I_NULL_POINTER_ERROR;

	bool b_is_problem_ok(CMscnProblem * pcProblem);

	CMscnSolution* pc_start_real_searching();

	void v_print_info_about_best_solution(CMscnSolution *pcActualBestSolution, 
		bool *&pbErrorArray, double dActaulBestQuality, int iBestNumber);

	int i_number_of_researches;
	
public:

	//-------------------------konstruktory / destruktor--------------------------------------------

	CRandomSearch(int iNumberOfResearches, CMscnProblem * pcActualProblem, 
		int iSeed, bool bPrintResults);

	~CRandomSearch();

	//-------------------------szukanie najlepszego rozwi¹zania-------------------------------------

	//CMscnSolution * run() override;

	CMscnSolution* run(CMscnProblem *pc_problem) override;

	//----------------------------------gettery i settery-------------------------------------------

	bool bSetActualProblem(CMscnProblem * pcNewProblem);

	CMscnProblem * pcGetActualProblem();

	int iGetNumberOfResearches();

	void vSetNumberOfResearches(int iNewNumberOfResearches);
};