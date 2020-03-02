#pragma once
#include "CAlgorithm.h"
#include "CMscnProblem.h"

class CDiffEvol : public CAlgorithm
{
	//--------------------prywatne zmienne / sta³e i metody pomocnicze------------------------------

	const static int I_NO_ERROR;

	const static int I_NULL_POINTER_ERROR;

	bool b_is_problem_ok(CMscnProblem * pcProblem);

	//-------------------------ewolucja ró¿nicowa - metody pomocnicze-------------------------------

	CMscnSolution *pc_start_real_evolution();

	CMscnSolution **&pc_init_population();
	
	bool b_individuals_are_different(int iInd0, int iInd1, int iInd2, int iInd3);

	bool b_individuals_are_different_v2(CMscnSolution* pcInd0, CMscnSolution* pcInd1, CMscnSolution* pcInd2, CMscnSolution* pcInd3);

	CMscnSolution *pc_choose_best_solution(CMscnSolution **&pcPopulation);

	void v_update_statistics(CMscnSolution **&pcPopulation);
	
	//--------------------prywatne zmienne potrzebne do ewolucji------------------------------------

	double d_diff_weight;

	double d_cross_prob;

	int i_size_of_population;

	int i_supposed_number_of_get_quality_calls;
	
public:

	//-------------------------konstruktory / destruktor--------------------------------------------

	CDiffEvol(double dDiffWeight, double dCrossProb, int iSizeOfPopulation,
		int iSupposedNumberOfGetQualityCalls, CMscnProblem * pcActualProblem, int iSeed, bool bPrintResults);

	~CDiffEvol();

	//-------------------------ewolucja ró¿nicowa---------------------------------------------------

	//CMscnSolution * run() override;

	CMscnSolution* run(CMscnProblem *pc_problem) override;
	
	//----------------------------------gettery i settery-------------------------------------------

	bool bSetActualProblem(CMscnProblem * pcNewProblem);

	CMscnProblem * pcGetActualProblem();

	double dGetDiffWeight();

	void vSetDiffWeight(double dNewDiffWeight);

	double dGetCrossProbability();

	void vSetCrossProbability(double dNewCrossProbability);

	int iGetSupposedNumberOfGetQualityCalls();

	void vSetSupposedNumberOfGetQualityCalls(int iNewSupposedNumberOfGetQualityCalls);

	int iGetSizeOfPopulation();

	void vSetSizeOfPopulation(int iNewSizeOfPopulation);
};