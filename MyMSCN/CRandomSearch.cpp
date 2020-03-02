#include "CRandomSearch.h"

const int CRandomSearch::I_NO_ERROR = 0;

const int CRandomSearch::I_NULL_POINTER_ERROR = 1;

//-------------------------konstruktory / destruktor------------------------------------------------

CRandomSearch::CRandomSearch(int iNumberOfResearches, CMscnProblem * pcActualProblem,
	int iSeed, bool bPrintResults)
{
	//this->pc_problem = pcActualProblem;
	i_seed = iSeed;
	i_number_of_researches = iNumberOfResearches;
	b_print_results = bPrintResults;
}

CRandomSearch::~CRandomSearch()
{
	//nic siê nie dzieje
}

//-------------------------szukanie najlepszego rozwi¹zania-----------------------------------------

CMscnSolution* CRandomSearch::run(CMscnProblem *pcProblem)
{
	pc_problem = pcProblem;

	if (b_is_problem_ok(pc_problem) && i_number_of_researches > 0)
	{
		if (b_print_results)
		{
			std::cout << "D: " << pc_problem->iGetD() << " | F: " << pc_problem->iGetF()
				<< " | M: " << pc_problem->iGetM() << " | S: " << pc_problem->iGetS() << "\n";
		}

		return pc_start_real_searching();
	}
	else
	{
		return NULL;
	}
}

CMscnSolution* CRandomSearch::pc_start_real_searching()
{
	vec_min_qualities.clear();
	vec_max_qualities.clear();
	vec_average_qualities.clear();
	vec_min_qualities.reserve(i_number_of_researches);
	vec_max_qualities.reserve(i_number_of_researches);
	vec_average_qualities.reserve(i_number_of_researches);
	double d_all_qualities = 0;
	double d_average_quality;
	double d_max_quality = INT_MIN;
	double d_min_quality = DBL_MAX;

	//ustawienie pocz¹tkowe danych w metodzie
	CMscnSolution* pc_actual_best_solution = NULL;
	double d_actual_best_quality = INT_MIN;
	int i_best_number = -1;
	int i_counter_of_satisfaying_resutls = 0;

	CRandom c_generator(i_seed);
	bool* pb_error_array = new bool[6];

	for (int ii = 0; ii < i_number_of_researches; ii++)
	{
		//utworzenie nowego rozwi¹zania
		CMscnSolution* pc_buf_solution = new CMscnSolution();
		pc_buf_solution->bGenerateInstance(&c_generator,
			pc_problem->iGetD(), pc_problem->iGetF(),
			pc_problem->iGetM(), pc_problem->iGetS(),
			pc_problem->pcGetMatrixXdminmax(),
			pc_problem->pcGetMatrixXfminmax(),
			pc_problem->pcGetMatrixXmminmax());

		int i_error_code;

		//wyliczenie quality i sprawdzenie constrains
		double d_buf_quality = pc_problem->dGetQuality(pc_buf_solution, i_error_code);

		bool b_satisfied = pc_problem->bConstraintsSatisfied(pc_buf_solution, i_error_code, pb_error_array);

		if (b_satisfied == 1)
		{
			i_counter_of_satisfaying_resutls++;
		}

		//wypisanie informacji
		if (b_print_results)
		{
			std::cout << "#" << (ii + 1) << " ";
			pc_problem->vPrintConstrainsSatisfiedErrorsSimplifiedSimplified(pb_error_array);
			std::cout << " | Quality: " << d_buf_quality;
			std::cout << " | Satisfied: " << b_satisfied << "\n";
		}

		//update'owanie statystyk

		d_all_qualities += d_buf_quality;

		if (d_min_quality > d_buf_quality)
		{
			d_min_quality = d_buf_quality;
		}

		if (d_max_quality < d_buf_quality && b_satisfied)
		{
			d_max_quality = d_buf_quality;
		}

		vec_min_qualities.push_back(d_min_quality);
		vec_max_qualities.push_back(d_max_quality);
		vec_average_qualities.push_back(d_buf_quality);

		//ewentalna zmiana najlepszych rozwi¹zañ
		if (d_buf_quality > d_actual_best_quality && b_satisfied)
		{
			delete pc_actual_best_solution;
			pc_actual_best_solution = pc_buf_solution;
			d_actual_best_quality = d_buf_quality;
			i_best_number = ii + 1;
		}
		else
		{
			delete pc_buf_solution;
		}
	}

	//zakoñczenie metody - rozwi¹zanie niekoniecznie satisfied	
	if (b_print_results)
	{
		v_print_info_about_best_solution(pc_actual_best_solution, pb_error_array, d_actual_best_quality, i_best_number);
		double d_percenage_of_satisfaying_results = (double)i_counter_of_satisfaying_resutls / (double)i_number_of_researches * 100;
		std::cout << "\nProcent satysfakcjonujacych wynikow: " << d_percenage_of_satisfaying_results << "%\n\n";
	}

	delete pb_error_array;

	return pc_actual_best_solution;
}


//------------------------------------metody pomocnicze---------------------------------------------

void CRandomSearch::v_print_info_about_best_solution(CMscnSolution *pcActualBestSolution, bool *&pbErrorArray, double dActaulBestQuality,
	int iBestNumber)
{
	if (pcActualBestSolution == NULL)
	{
		std::cout << "\n\n-----> Brak satysfakcjonujacego wyniku\n";
	}
	else
	{
		int i_best_error_code;
		bool b_best_satisfied = pc_problem->bConstraintsSatisfied(pcActualBestSolution, i_best_error_code,
			pbErrorArray);
		std::cout << "\n\n-----> Najlepszy wynik: " << dActaulBestQuality << " (#" << iBestNumber << ")\n";
		std::cout << "-----> Satisfied: " << b_best_satisfied << " | error code: " << i_best_error_code << " ";
		pc_problem->vPrintConstrainsSatisfiedErrorsSimplified(pbErrorArray);
	}
}

bool CRandomSearch::b_is_problem_ok(CMscnProblem* pcProblem)
{
	if (pcProblem != NULL && pcProblem->iGetD() > 0 && pcProblem->iGetF() > 0 &&
		pcProblem->iGetM() > 0 && pcProblem->iGetS() > 0)
	{
		if (pcProblem->pcGetArraySd()->bContainsNulls() ||
			pcProblem->pcGetArraySf()->bContainsNulls() ||
			pcProblem->pcGetArraySm()->bContainsNulls() ||
			pcProblem->pcGetArraySs()->bContainsNulls() ||
			pcProblem->pcGetMatrixCd()->bContainsNulls() ||
			pcProblem->pcGetMatrixCf()->bContainsNulls() ||
			pcProblem->pcGetMatrixCm()->bContainsNulls() ||
			pcProblem->pcGetArrayUd()->bContainsNulls() ||
			pcProblem->pcGetArrayUf()->bContainsNulls() ||
			pcProblem->pcGetArrayUm()->bContainsNulls() ||
			pcProblem->pcGetArrayP()->bContainsNulls() ||
			pcProblem->pcGetMatrixXdminmax()->bContainsNulls() ||
			pcProblem->pcGetMatrixXfminmax()->bContainsNulls() ||
			pcProblem->pcGetMatrixXmminmax()->bContainsNulls())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

//----------------------------------gettery i settery-----------------------------------------------

bool CRandomSearch::bSetActualProblem(CMscnProblem* pcNewProblem)
{
	if (b_is_problem_ok(pcNewProblem))
	{
		pc_problem = pcNewProblem;
		return true;
	}
	else
	{
		return false;
	}
}

CMscnProblem* CRandomSearch::pcGetActualProblem()
{
	return pc_problem;
}

int CRandomSearch::iGetNumberOfResearches()
{
	return i_number_of_researches;
}

void CRandomSearch::vSetNumberOfResearches(int iNewNumberOfResearches)
{
	i_number_of_researches = iNewNumberOfResearches;
}