#include <iostream>
#include "CMatrix.h"
#include "CMscnProblem.h"
#include "CArray.h"
#include "CMscnSolution.h"
#include "CRandom.h"
#include <time.h>
#include "CRandomSearch.h"
#include "CDiffEvol.h"
#include <sstream>

void v_evolve_test();
void v_generate_many_tests();
void v_generate_many_evo_different_paramters_tests();

void v_generate_single_evo_test(int iTestNumber, int d, int f, int m, int s, double dDiffWeight,
	double dCrossProb, int iSizeOfPopulation, int iSupposedNumberOfGetQualityCalls,
	bool bSaveEvoResults, bool bSaveProblem, CMscnProblem *pcProblem);

void v_generate_single_search_test(int iTestNumber, int d, int f, int m, int s, int iSearchNumber,
	bool bSeaveSearchResults, bool bSaveProblem, CMscnProblem *pcProblem);

void v_generate_evo_test_with_problem(double dDiffWeight, double dCrossProb,
	int iSizeOfPopulation, int iSupposedNumberOfGetQualityCalls, CMscnProblem *pcProblem,
	int i_seed, bool bPrint, bool bSave);

void v_CRandom_test();

int main()
{
	//std::cout << "\n\n--------------GENERATOR LICZB LOSOWYCH-----------------\n\n";
	//v_CRandom_test();
	//std::cout << "\n\n----------------EWOLUCJA I SEARCH JEDEN TEST--------------------\n\n";
	//v_evolve_test();
	std::cout << "\n\n---------------EWOLUCJA I SEARCH WIELE TESTOW-------------------\n\n";
	v_generate_many_tests();
	std::cout << "\n\n---------------EWOLUCJA WIELE TESTOW ROZNE PARAMETRY-------------------\n\n";
	v_generate_many_evo_different_paramters_tests();

	getchar();
	getchar();
	return 0;
}

void v_CRandom_test()
{
	CRandom c_generator;

	std::cout << "Seed: " << c_generator.iGetSeed();
	std::cout << "\n\nNextInt(0, 100): \n";
	for (int ii = 0; ii < 1000; ii++)
		std::cout << c_generator.iNextInt(0, 100) << "\n";

	std::cout << "\n\nNextInt(50, 100): \n";
	for (int ii = 0; ii < 100; ii++)
		std::cout << c_generator.iNextInt(50, 100) << "\n";

	std::cout << "\n\nNextDouble(0, 1): \n";
	for (int ii = 0; ii < 100; ii++)
		std::cout << c_generator.dNextDouble(0, 1) << "\n";

	std::cout << "\n\nNextDouble(0, 10): \n";
	for (int ii = 0; ii < 100; ii++)
		std::cout << c_generator.dNextDouble(0, 10) << "\n";

	std::cout << c_generator.iNextInt(10, 10);
}

void v_evolve_test()
{
	int i_seed = 1578872084;
	int i_error;
	CMscnProblem problem;
	problem.bGenerateInstance(i_seed, 2, 3, 4, 5);
	CMscnSolution * sol = NULL;

	problem.vPrintMatrixXdminmax();
	problem.vPrintMatrixXfminmax();
	problem.vPrintMatrixXmminmax();

	CRandomSearch search(10000, &problem, i_seed, true);
	CDiffEvol evo(0.5, 0.5, 100, 10000, &problem, i_seed, false);

	problem.bSetAlgorithm(&search);
	sol = problem.pcUseAlgorithm();
	std::cout << "Quality search: " << problem.dGetQuality(sol, i_error) << "\n\n";
	std::cout << "tech: " << i_error << "\n";

	problem.bSetAlgorithm(&evo);
	sol = problem.pcUseAlgorithm();
	std::cout << "Quality Evo: " << problem.dGetQuality(sol, i_error) << "\n\n";
	std::cout << "Seed: " << i_seed << "\n\n";

	//sol = problem.pcUseAlgorithm();
	//evo.bSaveIntoCsvFile("statyevo.csv", 1);
	//search.bSaveIntoCsvFile("statysearch.csv", 1);

	getchar();
	getchar();
}

void v_generate_many_tests()
{
	int i_evo_calls = 10000;
	int i_search = 1000;
	int i_seed = 1578878233;
	bool b_save_results = false;
	bool b_save_problem = false;
	CMscnProblem *pc_problem = new CMscnProblem;
	
	//problem 2,3,4,5
	pc_problem->bGenerateInstance(i_seed, 2, 3, 4, 5);
	v_generate_single_evo_test(1, 2, 3, 4, 5, 0.5, 0.5, 100, i_evo_calls, b_save_results, b_save_problem, pc_problem);
	v_generate_single_search_test(1, 2, 3, 4, 5, i_search, b_save_results, b_save_problem, pc_problem);

	//problem 3, 6, 9, 12
	pc_problem->bGenerateInstance(i_seed, 3, 6, 9, 12);
	v_generate_single_evo_test(2, 3, 6, 9, 12, 0.5, 0.5, 100, i_evo_calls, b_save_results, b_save_problem, pc_problem);
	v_generate_single_search_test(2, 3, 6, 9, 12, i_search, b_save_results, b_save_problem, pc_problem);

	//problem 1, 5, 10, 15
	pc_problem->bGenerateInstance(i_seed, 1, 5, 10, 15);
	v_generate_single_evo_test(3, 1, 5, 10, 15, 0.5, 0.5, 100, i_evo_calls, b_save_results, b_save_problem, pc_problem);
	v_generate_single_search_test(3, 1, 5, 10, 15, i_search, b_save_results, b_save_problem, pc_problem);

	//problem 5, 7, 9, 11
	pc_problem->bGenerateInstance(i_seed, 5, 7, 9, 11);
	v_generate_single_evo_test(4, 5, 7, 9, 11, 0.5, 0.5, 100, i_evo_calls, b_save_results, b_save_problem, pc_problem);
	v_generate_single_search_test(4, 5, 7, 9, 11, i_search, b_save_results, b_save_problem, pc_problem);

	//problem 6, 3, 5, 4
	pc_problem->bGenerateInstance(i_seed, 6, 3, 5, 4);
	v_generate_single_evo_test(5, 6, 3, 5, 4, 0.5, 0.5, 100, i_evo_calls, b_save_results, b_save_problem, pc_problem);
	v_generate_single_search_test(5, 6, 3, 5, 4, i_search, b_save_results, b_save_problem, pc_problem);

	//problem 9,4,3, 1
	pc_problem->bGenerateInstance(i_seed, 9, 4, 3, 1);
	v_generate_single_evo_test(6, 9, 4, 3, 1, 0.5, 0.5, 100, i_evo_calls, b_save_results, b_save_problem, pc_problem);
	v_generate_single_search_test(6, 9, 4, 3, 1, i_search, b_save_results, b_save_problem, pc_problem);

	//problem 2,3,4,10
	pc_problem->bGenerateInstance(i_seed, 2, 3, 4, 10);
	v_generate_single_evo_test(7, 2, 3, 4, 10, 0.5, 0.5, 100, i_evo_calls, b_save_results, b_save_problem, pc_problem);
	v_generate_single_search_test(7, 2, 3, 4, 10, i_search, b_save_results, b_save_problem, pc_problem);

	//problem 5, 5, 5, 5
	pc_problem->bGenerateInstance(i_seed, 5, 5, 5, 5);
	v_generate_single_evo_test(8, 5, 5, 5, 5, 0.5, 0.5, 100, i_evo_calls, b_save_results, b_save_problem, pc_problem);
	v_generate_single_search_test(8, 5, 5, 5, 5, i_search, b_save_results, b_save_problem, pc_problem);

	//problem 10, 10, 10, 10
	pc_problem->bGenerateInstance(i_seed, 10, 10, 10, 10);
	v_generate_single_evo_test(9, 10, 10, 10, 10, 0.5, 0.5, 100, i_evo_calls, b_save_results, b_save_problem, pc_problem);
	v_generate_single_search_test(9, 10, 10, 10, 10, i_search, b_save_results, b_save_problem, pc_problem);
	
	delete pc_problem;
}

void v_generate_single_evo_test(int iTestNumber, int d, int f, int m, int s, double dDiffWeight,
	double dCrossProb, int iSizeOfPopulation, int iSupposedNumberOfGetQualityCalls,
	bool bSaveEvoResults, bool bSaveProblem, CMscnProblem *pcProblem)
{
	std::cout << iTestNumber << ": (EVO): " << d << " " << f << " " << m << " " << s << "\n";

	//ustawianie testu
	int i_seed = time(NULL);
	int i_error;
	CMscnProblem *problem = pcProblem;
	CMscnSolution * sol = NULL;
	CDiffEvol *evo = new CDiffEvol(dDiffWeight, dCrossProb, iSizeOfPopulation,
		iSupposedNumberOfGetQualityCalls, problem, i_seed, false);;

	//obliczanie quality
	problem->bSetAlgorithm(evo);
	sol = problem->pcUseAlgorithm();
	std::cout << "Evo: " << problem->dGetQuality(sol, i_error) << "\n\n";

	//zapisywanie do plików
	if (bSaveEvoResults)
	{
		std::stringstream ss_evo;
		ss_evo << iTestNumber << ". Evo(" << d << ", " << f << ", " << m << ", " << s << ")(";
		ss_evo << dDiffWeight << ", " << dCrossProb << ")(" << iSizeOfPopulation << ", " << iSupposedNumberOfGetQualityCalls;
		ss_evo << ")(seed " << i_seed << ").csv";
		std::string s_evo = ss_evo.str();
		evo->bSaveIntoCsvFile(s_evo, 1);
	}

	if (bSaveProblem)
	{
		std::stringstream ss_problem;
		ss_problem << iTestNumber << ". Problem(" << d << ", " << f << ", " << m << ", " << s;
		ss_problem << ")(seed " << i_seed << ").txt";
		std::string s_problem = ss_problem.str();
		problem->bSaveIntoFile(s_problem);
	}

	//usuwanie pozosta³oœci
	delete sol;
	delete evo;
}

void v_generate_single_search_test(int iTestNumber, int d, int f, int m, int s, int iSearchNumber,
	bool bSaveProblem, bool bSaveSearchResults, CMscnProblem *pcProblem)
{
	std::cout << iTestNumber << ": (SEARCH): " << d << " " << f << " " << m << " " << s << "\n";

	//ustawianie testu
	int i_seed = time(NULL);
	int i_error;
	CMscnProblem *problem = pcProblem;
	CRandomSearch *search = new CRandomSearch(iSearchNumber, problem, i_seed, false);;
	CMscnSolution * sol = NULL;

	//obliczanie quality
	problem->bSetAlgorithm(search);
	sol = problem->pcUseAlgorithm();
	std::cout << "Search: " << problem->dGetQuality(sol, i_error) << "\n\n\n";

	//zapisywanie do plików
	if (bSaveSearchResults)
	{
		std::stringstream ss_searc;
		ss_searc << iTestNumber << ". Search(" << d << ", " << f << ", " << m << ", " << s << ")(";
		ss_searc << iSearchNumber << ")(seed " << i_seed << ").csv";
		std::string s_searc = ss_searc.str();
		search->bSaveIntoCsvFile(s_searc, 1);
	}

	if (bSaveProblem)
	{
		std::stringstream ss_problem;
		ss_problem << iTestNumber << ". Problem(" << d << ", " << f << ", " << m << ", " << s;
		ss_problem << ")(seed " << i_seed << ".txt";
		std::string s_problem = ss_problem.str();
		problem->bSaveIntoFile(s_problem);
	}

	//usuwanie pozosta³oœci
	delete search;
	delete sol;
}

void v_generate_many_evo_different_paramters_tests()
{
	int i_evo_calls = 10000;
	int i_seed = 1578872084;
	bool b_save = false;
	bool b_print = false;

	//ustawianie testu - problem 3, 6, 9, 12
	int i_error;
	CMscnProblem *problem = new CMscnProblem;
	problem->bGenerateInstance(i_seed, 3, 6, 9, 12);
	CDiffEvol *evo = NULL;

	//obliczanie quality
	v_generate_evo_test_with_problem(0.5, 0.5, 100, i_evo_calls, problem, i_seed, b_print, b_save);

	v_generate_evo_test_with_problem(0.3, 0.3, 100, i_evo_calls, problem, i_seed, b_print, b_save);

	v_generate_evo_test_with_problem(0.8, 0.8, 100, i_evo_calls, problem, i_seed, b_print, b_save);

	v_generate_evo_test_with_problem(1, 1, 100, i_evo_calls, problem, i_seed, b_print, b_save);

	v_generate_evo_test_with_problem(0.3, 0.8, 100, i_evo_calls, problem, i_seed, b_print, b_save);

	v_generate_evo_test_with_problem(0.8, 0.3, 100, i_evo_calls, problem, i_seed, b_print, b_save);

	v_generate_evo_test_with_problem(0.9, 0.5, 100, i_evo_calls, problem, i_seed, b_print, b_save);

	v_generate_evo_test_with_problem(0.5, 0.9, 100, i_evo_calls, problem, i_seed, b_print, b_save);

	//usuwanie pozosta³oœci
	delete problem;
}

void v_generate_evo_test_with_problem(double dDiffWeight, double dCrossProb,
	int iSizeOfPopulation, int iSupposedNumberOfGetQualityCalls, CMscnProblem *pcProblem,
	int iSeed, bool bPrint, bool bSave)
{
	int i_error;
	CMscnSolution * pc_solution = NULL;
	CDiffEvol *pc_evo = NULL;

	//obliczanie quality
	pc_evo = new CDiffEvol(dDiffWeight, dCrossProb, iSizeOfPopulation, iSupposedNumberOfGetQualityCalls, pcProblem, iSeed, bPrint);
	pcProblem->bSetAlgorithm(pc_evo);
	pc_solution = pcProblem->pcUseAlgorithm();
	std::cout << "Evo " << dDiffWeight << ", " << dCrossProb << ": qua:" << pcProblem->dGetQuality(pc_solution, i_error) << "\n\n";

	//zapsianie evo do pliku:
	if (bSave)
	{
		std::stringstream ss_evo;
		ss_evo << "Evo(" << pcProblem->iGetD() << ", " << pcProblem->iGetF() << ", " << pcProblem->iGetM() << ", " << pcProblem->iGetS() << ")(";
		ss_evo << dDiffWeight << ", " << dCrossProb << ")(" << iSizeOfPopulation << ", " << iSupposedNumberOfGetQualityCalls;
		ss_evo << ")(seed " << iSeed << ").csv";
		std::string s_evo = ss_evo.str();
		pc_evo->bSaveIntoCsvFile(s_evo, 1);
	}
	delete pc_evo;
	delete pc_solution;
}