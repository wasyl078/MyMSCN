#include "CDiffEvol.h"

const int CDiffEvol::I_NO_ERROR = 0;

const int CDiffEvol::I_NULL_POINTER_ERROR = 1;

//-------------------------konstruktory / destruktor--------------------------------------------

CDiffEvol::CDiffEvol(double dDiffWeight, double dCrossProb, int iSizeOfPopulation,
	int iSupposedNumberOfGetQualityCalls, CMscnProblem * pcActualProblem, int uiSeed, bool bPrintResults)
{
	//this->pc_problem = pcActualProblem;
	d_diff_weight = dDiffWeight;
	d_cross_prob = dCrossProb;
	i_seed = uiSeed;
	i_size_of_population = iSizeOfPopulation;
	b_print_results = bPrintResults;
	i_supposed_number_of_get_quality_calls = iSupposedNumberOfGetQualityCalls;
}

CDiffEvol::~CDiffEvol()
{
	//nic nie robi
}

//-------------------------ewolucja ró¿nicowa-------------------------------------------------------

CMscnSolution * CDiffEvol::run(CMscnProblem *pcProblem)
{
	pc_problem = pcProblem;
	
	if (b_is_problem_ok(pc_problem))
	{
		return pc_start_real_evolution();
	}
	else
	{
		return NULL;
	}
}

//-------------------------ewolucja ró¿nicowa - metody pomocnicze-------------------------------

CMscnSolution * CDiffEvol::pc_start_real_evolution()
{
	vec_min_qualities.clear();
	vec_max_qualities.clear();
	vec_average_qualities.clear();

	CMscnSolution **population = pc_init_population();

	CRandom c_generator(i_seed);
	int i_satisfied_counter = 0;
	int i_all_new = 0;

	int i_genotype_size = population[0]->iReturnAsArraysLength();

	pc_problem->vResetNumberOfGetQualityCalls();

	while (pc_problem->iGetNumberOfGetQualityCalls() < i_supposed_number_of_get_quality_calls)
	{
		for (int jj = 0; jj < i_size_of_population && pc_problem->iGetNumberOfGetQualityCalls() < i_supposed_number_of_get_quality_calls; jj++)
		{
			if (b_print_results)
			{
				std::cout << pc_problem->iGetNumberOfGetQualityCalls() << " ";
			}

			int i_ind_base = c_generator.iNextInt(0,i_size_of_population);
			int i_ind_0 = c_generator.iNextInt(0, i_size_of_population);
			int i_ind_1 = c_generator.iNextInt(0, i_size_of_population);

			CMscnSolution *pc_ind = population[jj];
			CMscnSolution *pc_ind_base = population[i_ind_base];
			CMscnSolution *pc_ind_0 = population[i_ind_0];
			CMscnSolution *pc_ind_1 = population[i_ind_1];

			//std::cout << " | ind: " << pc_ind << " | base: " << pc_ind_base << " | 0: " << pc_ind_0 << " | 1: " << pc_ind_1 << "\n";
			//std::cout << " | jj: " << jj << " | i_ind_base: " << i_ind_base << " | i_ind_0: " << i_ind_0 << " | i_ind_1: " << i_ind_1 << "\n";
			
			if (b_individuals_are_different_v2(pc_ind, pc_ind_base, pc_ind_0, pc_ind_1))
			{
				CMscnSolution *pc_ind_new = new CMscnSolution();
				
				double *pd_ind = pc_ind->pdRetunAsArray();
				double *pd_ind_0 = pc_ind_0->pdRetunAsArray();
				double *pd_ind_1 = pc_ind_1->pdRetunAsArray();
				double *pd_ind_base = pc_ind_base->pdRetunAsArray();
				double *pd_ind_new = new double[i_genotype_size];

				pd_ind_new[0] = pc_problem->iGetD();
				pd_ind_new[1] = pc_problem->iGetF();
				pd_ind_new[2] = pc_problem->iGetM();
				pd_ind_new[3] = pc_problem->iGetS();

				for (int i_gene_offset = 4; i_gene_offset < i_genotype_size; i_gene_offset++)
				{
					if (c_generator.dNextDouble(0, 1) < d_cross_prob)
					{
						double d_new_gene = pd_ind_base[i_gene_offset] + d_diff_weight * (pd_ind_0[i_gene_offset] - pd_ind_1[i_gene_offset]);

						double d_minimum_for_gene = pc_problem->dGetMinimumForGene(i_gene_offset);

						double d_maximum_for_gene = pc_problem->dGetMaximumForGene(i_gene_offset);

						if (d_new_gene < d_minimum_for_gene)
						{
							d_new_gene = c_generator.dNextDouble(d_minimum_for_gene, d_maximum_for_gene);
							pd_ind_new[i_gene_offset] = d_minimum_for_gene;
						}
						else if (d_new_gene > d_maximum_for_gene)
						{
							d_new_gene = c_generator.dNextDouble(d_minimum_for_gene, d_maximum_for_gene);
							pd_ind_new[i_gene_offset] = d_maximum_for_gene;
						}
						else
						{
							pd_ind_new[i_gene_offset] = d_new_gene;
						}
					}
					else
					{
						pd_ind_new[i_gene_offset] = pd_ind[i_gene_offset];
					}
				}

				pc_ind_0->bInterpreteArray(pd_ind_0, i_genotype_size);
				pc_ind_1->bInterpreteArray(pd_ind_1, i_genotype_size);
				pc_ind_new->bInterpreteArray(pd_ind_new, i_genotype_size);
				pc_ind_base->bInterpreteArray(pd_ind_base, i_genotype_size);

				int i_error;
				double d_ind_fintess = pc_problem->dGetQuality(pc_ind, i_error);
				double d_ind_new_fintess = pc_problem->dGetQuality(pc_ind_new, i_error);
				bool *pb_arr = new bool[6];
				i_all_new++;

				bool b_ind_new_satisfied = pc_problem->bConstraintsSatisfied(pc_ind_new, i_error, pb_arr);

				if (b_ind_new_satisfied)
					i_satisfied_counter++;

				if (b_ind_new_satisfied && d_ind_new_fintess > d_ind_fintess)
				{
					delete pc_ind;
					population[jj] = pc_ind_new;

					if (b_print_results)
					{
						std::cout << "#" << jj << "| new: " << d_ind_new_fintess;
						std::cout << " | +" << (d_ind_new_fintess - d_ind_fintess) << " | ";
						pc_problem->vPrintConstrainsSatisfiedErrorsSimplifiedSimplified(pb_arr);
						std::cout << "\n";
					}
				}
				else
				{
					if (b_print_results)
					{
						std::cout << "\n";
					}
					delete pc_ind_new;
				}

				delete pb_arr;
				delete pd_ind_0;
				delete pd_ind_1;
				delete pd_ind;
				delete pd_ind_base;
				delete pd_ind_new;
			}
		}
		v_update_statistics(population);
	}

	if (b_print_results)
	{
		std::cout << "\nWszystkie utworzone nowe satysfakcjonujace rozwiazania: " << i_satisfied_counter << "\n";
		std::cout << "Wszystkie utworzone w miedzyczasie nowe rowiazania: " << i_all_new << "\n";
		std::cout << "Procent satysfakcjonujacych rozwiaza: " <<
			(double)i_satisfied_counter / (double)i_all_new * 100 << "%\n";
	}

	return pc_choose_best_solution(population);
}

bool CDiffEvol::b_individuals_are_different(int iInd0, int iInd1, int iInd2, int iInd3)
{
	return iInd0 != iInd1 && iInd0 != iInd2 && iInd0 != iInd3
		&& iInd1 != iInd2 && iInd1 != iInd3 && iInd2 != iInd3;
}

bool CDiffEvol::b_individuals_are_different_v2(CMscnSolution* pcInd0, CMscnSolution* pcInd1, CMscnSolution* pcInd2, CMscnSolution* pcInd3)
{

	double *pd_ind_0 = pcInd0->pdRetunAsArray();
	double *pd_ind_1 = pcInd1->pdRetunAsArray();
	double *pd_ind_2 = pcInd2->pdRetunAsArray();
	double *pd_ind_3 = pcInd3->pdRetunAsArray();

	int i_genotype_size = pcInd0->iReturnAsArraysLength();
	int i_c_1 = 0;
	int i_c_2 = 0;
	int i_c_3 = 0;
	int i_c_4 = 0;
	int i_c_5 = 0;
	int i_c_6 = 0;

	for (int ii = 4; ii < i_genotype_size; ii++)
	{
		//#0 == #1
		if (pd_ind_0[ii] == pd_ind_1[ii])
		{
			i_c_1++;
		}
		//#0 == #2
		if (pd_ind_0[ii] == pd_ind_2[ii])
		{
			i_c_2++;
		}
		//#0 == #3
		if (pd_ind_0[ii] == pd_ind_3[ii])
		{
			i_c_3++;
		}
		//#1 == #2
		if (pd_ind_1[ii] == pd_ind_2[ii])
		{
			i_c_4++;
		}
		//#1 == #3
		if (pd_ind_1[ii] == pd_ind_3[ii])
		{
			i_c_5++;
		}
		//#2 == #3
		if (pd_ind_2[ii] == pd_ind_3[ii])
		{
			i_c_6++;
		}
	}

	delete pd_ind_0;
	delete pd_ind_1;
	delete pd_ind_2;
	delete pd_ind_3;

	//std::cout << i_c_1 << " " << i_c_2 << " " << i_c_3 << " " << i_c_4 << " " << i_c_5 << " " << i_c_6 << "\n";

	if (i_c_1 == i_genotype_size - 4 ||
		i_c_2 == i_genotype_size - 4 ||
		i_c_3 == i_genotype_size - 4 ||
		i_c_4 == i_genotype_size - 4 ||
		i_c_5 == i_genotype_size - 4 ||
		i_c_6 == i_genotype_size - 4)
	{
		return false;
	}
	else
	{
		return true;
	}
}

CMscnSolution* CDiffEvol::pc_choose_best_solution(CMscnSolution**&pcPopulation)
{
	int i_error;
	CMscnSolution * pc_current_best = NULL;
	double d_best_quality = INT_MIN;
	for (int ii = 0; ii < i_size_of_population; ii++)
	{
		CMscnSolution * pc_buf_sol = pcPopulation[ii];
		double d_buf_quality = pc_problem->dGetQuality(pc_buf_sol, i_error);

		if (d_best_quality < d_buf_quality)
		{
			delete pc_current_best;
			pc_current_best = pc_buf_sol;
			d_best_quality = d_buf_quality;
		}
		else
		{
			delete pc_buf_sol;
		}
	}
	if (b_print_results)
	{
		std::cout << "\nEvolve constraints: ";
		bool *pb_error_arr = new bool[6];
		bool b_sati = pc_problem->bConstraintsSatisfied(pc_current_best, i_error, pb_error_arr);
		pc_problem->vPrintConstrainsSatisfiedErrorsSimplifiedSimplified(pb_error_arr);
		std::cout << "\n";
	}

	return pc_current_best;
}


CMscnSolution **&CDiffEvol::pc_init_population()
{
	CMscnSolution **pc_population = new CMscnSolution*[i_size_of_population];
	CRandom c_generator(i_seed);

	for (int ii = 0; ii < i_size_of_population; ii++)
	{
		CMscnSolution *pc_buf_solution = new CMscnSolution();
		pc_buf_solution->bGenerateInstance(&c_generator,
			pc_problem->iGetD(), pc_problem->iGetF(),
			pc_problem->iGetM(), pc_problem->iGetS(),
			pc_problem->pcGetMatrixXdminmax(),
			pc_problem->pcGetMatrixXfminmax(),
			pc_problem->pcGetMatrixXmminmax());
		pc_population[ii] = pc_buf_solution;
	}

	return pc_population;
}

void CDiffEvol::v_update_statistics(CMscnSolution**& pcPopulation)
{
	double d_all_qualities = 0;
	double d_average_quality;
	double d_max_quality = DBL_MIN;
	double d_min_quality = DBL_MAX;

	double d_buf_qality;

	for (int ii = 0; ii < i_size_of_population; ii++)
	{
		d_buf_qality = pcPopulation[ii]->dGetLastCalculatedQuality();
		d_all_qualities += d_buf_qality;

		if (d_min_quality > d_buf_qality)
		{
			d_min_quality = d_buf_qality;
		}

		if (d_max_quality < d_buf_qality)
		{
			d_max_quality = d_buf_qality;
		}
	}

	d_average_quality = d_all_qualities / i_size_of_population;

	vec_min_qualities.push_back(d_min_quality);
	vec_max_qualities.push_back(d_max_quality);
	vec_average_qualities.push_back(d_average_quality);

	if (b_print_results)
	{
		std::cout << "\nMin: " << d_min_quality << " | max: " << d_max_quality
			<< " | ave: " << d_average_quality << "\n";
	}
}


//------------------------------------metody pomocnicze---------------------------------------------

bool CDiffEvol::b_is_problem_ok(CMscnProblem * pcProblem)
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

//-------------------------publiczne metody pomocnicze------------------------------------------

bool CDiffEvol::bSetActualProblem(CMscnProblem * pcNewProblem)
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

CMscnProblem * CDiffEvol::pcGetActualProblem()
{
	return pc_problem;
}

double CDiffEvol::dGetDiffWeight()
{
	return d_diff_weight;
}

void CDiffEvol::vSetDiffWeight(double dNewDiffWeight)
{
	d_diff_weight = dNewDiffWeight;
}

double CDiffEvol::dGetCrossProbability()
{
	return d_cross_prob;
}

void CDiffEvol::vSetCrossProbability(double dNewCrossProbability)
{
	d_cross_prob = dNewCrossProbability;
}

int CDiffEvol::iGetSupposedNumberOfGetQualityCalls()
{
	return i_supposed_number_of_get_quality_calls;
}

void CDiffEvol::vSetSupposedNumberOfGetQualityCalls(int iNewSupposedNumberOfGetQualityCalls)
{
	i_supposed_number_of_get_quality_calls = iNewSupposedNumberOfGetQualityCalls;
}

int CDiffEvol::iGetSizeOfPopulation()
{
	return i_size_of_population;
}

void CDiffEvol::vSetSizeOfPopulation(int iNewSizeOfPopulation)
{
	i_size_of_population = iNewSizeOfPopulation;
}
