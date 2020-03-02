#include "CMscnProblem.h"

//-------------------Moja w³asna implementacja problemnu MSCN---------------------------------------


//-------------------prywatne sta³e statyczne do losowania------------------------------------------

const int CMscnProblem::I_DEFAULT_MINIMUM_ARRAY = 0;
const int CMscnProblem::I_DEFAULT_MAXIMUM_ARRAY = 100;

const int CMscnProblem::I_DEFAULT_MINIMUM_MATRIX = 0;
const int CMscnProblem::I_DEFAULT_MAXIMUM_MATRIX = 100;

const int CMscnProblem::I_MIN_MATRIX_CD = 0;
const int CMscnProblem::I_MAX_MATRIX_CD = 10;

const int CMscnProblem::I_MIN_MATRIX_CF = 0;
const int CMscnProblem::I_MAX_MATRIX_CF = 10;

const int CMscnProblem::I_MIN_MATRIX_CM = 0;
const int CMscnProblem::I_MAX_MATRIX_CM = 10;

const int CMscnProblem::I_MIN_ARRAY_SD = 5000;
const int CMscnProblem::I_MAX_ARRAY_SD = 2000;

const int CMscnProblem::I_MIN_ARRAY_SF = 5000;
const int CMscnProblem::I_MAX_ARRAY_SF = 20000;

const int CMscnProblem::I_MIN_ARRAY_SM = 5000;
const int CMscnProblem::I_MAX_ARRAY_SM = 20000;

const int CMscnProblem::I_MIN_ARRAY_SS = 5000;
const int CMscnProblem::I_MAX_ARRAY_SS = 20000;

const int CMscnProblem::I_MIN_ARRAY_UD = 0;
const int CMscnProblem::I_MAX_ARRAY_UD = 20;

const int CMscnProblem::I_MIN_ARRAY_UF = 0;
const int CMscnProblem::I_MAX_ARRAY_UF = 20;

const int CMscnProblem::I_MIN_ARRAY_UM = 0;
const int CMscnProblem::I_MAX_ARRAY_UM = 20;

const int CMscnProblem::I_MIN_ARRAY_P = 500;
const int CMscnProblem::I_MAX_ARRAY_P = 1000;

const int CMscnProblem::I_MIN_MATRIX_XDMINMAX = 30;
const int CMscnProblem::I_MAX_MATRIX_XDMINMAX = 40;

const int CMscnProblem::I_MIN_MATRIX_XFMINMAX = 10;
const int CMscnProblem::I_MAX_MATRIX_XFMINMAX = 20;

const int CMscnProblem::I_MIN_MATRIX_XMMINMAX = 0;
const int CMscnProblem::I_MAX_MATRIX_XMMINMAX = 10;

const int CMscnProblem::I_DEFAULT_UNITS_NUMBER = 10000;

//-------------------------prywatne sta³e statyczne-------------------------------------------------

const int CMscnProblem::I_ERROR_CODE_NULL_POINTER = 1;

const int CMscnProblem::I_ERROR_CODE_WRONG_ARR_LENGTH = -2;

const int CMscnProblem::I_ERROR_CODE_CONTAINS_NEGATIVE_NUMBERS = -3;

const int CMscnProblem::I_ERROR_CODE_NO_ERROR = 0;

//-------------------------konstruktor / destruktor-------------------------------------------------

CMscnProblem::CMscnProblem()
{
	i_d = 0;
	i_f = 0;
	i_m = 0;
	i_s = 0;
	pc_matrix_cd = new CMatrix<double>(i_d, i_f);
	pc_matrix_cf = new CMatrix<double>(i_f, i_m);
	pc_matrix_cm = new CMatrix<double>(i_m, i_s);
	pc_array_sd = new CArray<double>(i_d);
	pc_array_sf = new CArray<double>(i_f);
	pc_array_sm = new CArray<double>(i_m);
	pc_array_ss = new CArray<double>(i_s);
	pc_array_ud = new CArray<double>(i_d);
	pc_array_uf = new CArray<double>(i_f);
	pc_array_um = new CArray<double>(i_m);
	pc_array_p = new CArray<double>(i_s);
	pc_matrix_xdminmax = new CMatrix<double>(i_d, 2 * i_f);
	pc_matrix_xfminmax = new CMatrix<double>(i_f, 2 * i_m);
	pc_matrix_xmminmax = new CMatrix<double>(i_m, 2 * i_s);
	pf_file = NULL;
}

CMscnProblem::~CMscnProblem()
{
	delete pc_matrix_cd;
	delete pc_matrix_cf;
	delete pc_matrix_cm;
	delete pc_array_sd;
	delete pc_array_sf;
	delete pc_array_sm;
	delete pc_array_ss;
	delete pc_array_ud;
	delete pc_array_uf;
	delete pc_array_um;
	delete pc_array_p;
	delete pc_matrix_xdminmax;
	delete pc_matrix_xfminmax;
	delete pc_matrix_xmminmax;
	b_close_file();
}

//-------------------------Lista 9: Zad 4: dGetQuality----------------------------------------------

double CMscnProblem::dGetQuality(double* pdSolution, int iSolutionArrLength, int& wasAnException)
{
	if (pdSolution == NULL)
	{
		wasAnException = I_ERROR_CODE_NULL_POINTER;
		return -1;
	}
	else
	{
		CMscnSolution c_solution;
		c_solution.bInterpreteArray(pdSolution, iSolutionArrLength);
		return dGetQuality(&c_solution, wasAnException);
	}
}

double CMscnProblem::dGetQuality(CMscnSolution* pcSolution, int& wasAnException)
{
	double d_solution_quality = -1;
	
	// -1: ma adres NULL
	if (pcSolution == NULL)
	{
		wasAnException = I_ERROR_CODE_NULL_POINTER;
	}
	// -2: ma nieprawid³ow¹ d³ugoœæ
	else if (pcSolution->iReturnAsArraysLength() != 4 + i_d * i_f + i_f * i_m + i_m * i_s)
	{
		wasAnException = I_ERROR_CODE_WRONG_ARR_LENGTH;
	}
	// -3: zawiera wartoœci ujemne
	else if (b_does_solution_contain_negative_numbers(pcSolution))
	{
		wasAnException = I_ERROR_CODE_CONTAINS_NEGATIVE_NUMBERS;
	}
	//nie wyst¹pi³ ¿aden b³¹d - mo¿na obliczyæ quality
	else
	{
		wasAnException = I_ERROR_CODE_NO_ERROR;
		d_solution_quality = d_calculate_quality_of_solution(pcSolution);
		pcSolution->vSetLastCalculatedQuality(d_solution_quality);
	}
	return d_solution_quality;
}

double CMscnProblem::d_calculate_quality_of_solution(CMscnSolution* pcSolution)
{
	i_number_of_get_quality_calls++;
	double P = d_calculate_P(pcSolution);
	double Kt = d_calculate_Kt(pcSolution);
	double Ku = d_calculate_Ku(pcSolution);
	return P - Kt - Ku;
}

double CMscnProblem::d_calculate_P(CMscnSolution* pcSolution)
{
	double d_P = 0;

	for (int m = 0; m < i_m; m++)
	{
		for (int s = 0; s < i_s; s++)
		{
			double d1 = *(pc_array_p->tGet(s));
			double d2 = *(pcSolution->pcGetMatrixXm()->tGetFromRowNumberCollumnNumber(m, s));
			d_P += d1 * d2;
		}
	}
	//std::cout << "P: " << d_P << "\n";
	return d_P;
}

double CMscnProblem::d_calculate_Kt(CMscnSolution* pcSolution)
{
	double d_Kt = 0;

	//I czêœæ
	double d_sum_1 = 0;
	for (int d = 0; d < i_d; d++)
	{
		for (int f = 0; f < i_f; f++)
		{
			double d_buf_1 = *(pc_matrix_cd->tGetFromRowNumberCollumnNumber(d, f));
			double d_buf_2 = *(pcSolution->pcGetMatrixXd()->tGetFromRowNumberCollumnNumber(d, f));
			d_sum_1 += d_buf_1 * d_buf_2;
		}
	}

	//II czêœæ
	double d_sum_2 = 0;
	for (int f = 0; f < i_f; f++)
	{
		for (int m = 0; m < i_m; m++)
		{
			double d_buf_1 = *(pc_matrix_cf->tGetFromRowNumberCollumnNumber(f, m));
			double d_buf_2 = *(pcSolution->pcGetMatrixXf()->tGetFromRowNumberCollumnNumber(f, m));
			d_sum_2 += d_buf_1 * d_buf_2;
		}
	}

	//III czêœæ
	double d_sum_3 = 0;
	for (int m = 0; m < i_m; m++)
	{
		for (int s = 0; s < i_s; s++)
		{
			double d_buf_1 = *(pc_matrix_cm->tGetFromRowNumberCollumnNumber(m, s));
			double d_buf_2 = *(pcSolution->pcGetMatrixXm()->tGetFromRowNumberCollumnNumber(m, s));
			d_sum_3 += d_buf_1 * d_buf_2;
		}
	}

	d_Kt = d_sum_1 + d_sum_2 + d_sum_3;
	//std::cout << "Kt: [ " << d_sum_1 << ", " << d_sum_2 << ", " << d_sum_3 << " ] -> " << d_Kt << "\n";
	return d_Kt;
}

double CMscnProblem::d_calculate_Ku(CMscnSolution* pcSolution)
{
	double d_Ku = 0;

	//I czêœæ
	double d_sum_1 = 0;
	for (int d = 0; d < i_d; d++)
	{
		double i_X = 0;
		for (int f = 0; f < i_f; f++)
		{
			i_X += *(pcSolution->pcGetMatrixXd()->tGetFromRowNumberCollumnNumber(d, f));
		}

		if (i_X > 0)
		{
			d_sum_1 += *(pc_array_ud->tGet(d));
		}
	}

	//II czêœæ
	double d_sum_2 = 0;
	for (int f = 0; f < i_f; f++)
	{
		double i_X = 0;
		for (int m = 0; m < i_m; m++)
		{
			i_X += *(pcSolution->pcGetMatrixXf()->tGetFromRowNumberCollumnNumber(f, m));
		}

		if (i_X > 0)
		{
			d_sum_2 += *(pc_array_uf->tGet(f));
		}
	}

	//III czêœæ
	double d_sum_3 = 0;
	for (int m = 0; m < i_m; m++)
	{
		double i_X = 0;
		for (int s = 0; s < i_s; s++)
		{
			i_X += *(pcSolution->pcGetMatrixXm()->tGetFromRowNumberCollumnNumber(m, s));
		}

		if (i_X > 0)
		{
			d_sum_3 += *(pc_array_um->tGet(m));
		}
	}
	d_Ku = d_sum_1 + d_sum_2 + d_sum_3;

	//std::cout << "Ku: [ " << d_sum_1 << ", " << d_sum_2 << ", " << d_sum_3 << " ] -> " << d_Ku << "\n";
	return d_Ku;
}

//-------------------------Lista 9: Zad 5: bConstraintsSatisfied------------------------------------

bool CMscnProblem::bConstraintsSatisfied(double* pdSolution, int iSolutionArrLength, int& iTechnicalError,
	bool*& pbLimitsArr)
{
	CMscnSolution c_sol;

	if (c_sol.bInterpreteArray(pdSolution, iSolutionArrLength))
	{
		return bConstraintsSatisfied(&c_sol, iTechnicalError, pbLimitsArr);
	}
	else
	{
		iTechnicalError = I_ERROR_CODE_NULL_POINTER;
		return false;
	}
}

bool CMscnProblem::bConstraintsSatisfied(CMscnSolution* pcSolution, int& iTechnicalError, bool*& pbLimitsArr)
{
	// -1: ma adres NULL
	if (pcSolution == NULL)
	{
		iTechnicalError = I_ERROR_CODE_NULL_POINTER;
	}
	// -2: ma nieprawid³ow¹ d³ugoœæ
	else if (pcSolution->iReturnAsArraysLength() != 4 + i_d * i_f + i_f * i_m + i_m * i_s)
	{
		iTechnicalError = I_ERROR_CODE_WRONG_ARR_LENGTH;
	}
	// -3: zawiera wartoœci ujemne
	else if (b_does_solution_contain_negative_numbers(pcSolution))
	{
		iTechnicalError = I_ERROR_CODE_CONTAINS_NEGATIVE_NUMBERS;
	}
	//nie wyst¹pi³ ¿aden b³¹d - mo¿na obliczyæ quality
	else
	{
		iTechnicalError = I_ERROR_CODE_NO_ERROR;
		v_check_constraints_satisfied(pcSolution, pbLimitsArr);
	}

	return pbLimitsArr[0] && pbLimitsArr[1] && pbLimitsArr[2] &&
		pbLimitsArr[3] && pbLimitsArr[4] && pbLimitsArr[5] && iTechnicalError == I_ERROR_CODE_NO_ERROR;
}

void CMscnProblem::v_check_constraints_satisfied(double* pdSolution, int iSolutionArrLength, bool*& pbLimitsArr)
{
	CMscnSolution* pc_sol = new CMscnSolution();
	pc_sol->bInterpreteArray(pdSolution, iSolutionArrLength);
	v_check_constraints_satisfied(pc_sol, pbLimitsArr);
}

void CMscnProblem::v_check_constraints_satisfied(CMscnSolution* pcSolution, bool*& pbLimitsArr)
{
	//false - zm³amane
	//true - OK

	pbLimitsArr[0] = b_check_I_limit(pcSolution);
	pbLimitsArr[1] = b_check_II_limit(pcSolution);
	pbLimitsArr[2] = b_check_III_limit(pcSolution);
	pbLimitsArr[3] = b_check_IV_limit(pcSolution);
	pbLimitsArr[4] = b_check_V_limit(pcSolution);
	pbLimitsArr[5] = b_check_VI_limit(pcSolution);
}

bool CMscnProblem::b_check_I_limit(CMscnSolution* pcSolution)
{
	for (int d = 0; d < i_d; d++)
	{
		double d_sum = 0;
		for (int f = 0; f < i_f; f++)
		{
			d_sum += *(pcSolution->pcGetMatrixXd()->tGetFromRowNumberCollumnNumber(d, f));
		}
		if (d_sum > *(pc_array_sd->tGet(d)))
		{
			return false;
		}
	}
	return true;
}

bool CMscnProblem::b_check_II_limit(CMscnSolution* pcSolution)
{
	for (int f = 0; f < i_f; f++)
	{
		double d_sum = 0;
		for (int m = 0; m < i_m; m++)
		{
			d_sum += *(pcSolution->pcGetMatrixXf()->tGetFromRowNumberCollumnNumber(f, m));
		}
		if (d_sum > *(pc_array_sf->tGet(f)))
		{
			return false;
		}
	}
	return true;
}

bool CMscnProblem::b_check_III_limit(CMscnSolution* pcSolution)
{
	for (int m = 0; m < i_m; m++)
	{
		double d_sum = 0;
		for (int s = 0; s < i_s; s++)
		{
			d_sum += *(pcSolution->pcGetMatrixXm()->tGetFromRowNumberCollumnNumber(m, s));
		}
		//B£¥D NAPRAWI£EM Z LISTY
		if (d_sum > *(pc_array_sm->tGet(m)))
		{
			return false;
		}
	}
	return true;
}

bool CMscnProblem::b_check_IV_limit(CMscnSolution* pcSolution)
{
	for (int s = 0; s < i_s; s++)
	{
		double d_sum = 0;
		for (int m = 0; m < i_m; m++)
		{
			d_sum += *(pcSolution->pcGetMatrixXm()->tGetFromRowNumberCollumnNumber(m, s));
		}
		if (d_sum > *(pc_array_ss->tGet(s)))
		{
			return false;
		}
	}
	return true;
}

bool CMscnProblem::b_check_V_limit(CMscnSolution* pcSolution)
{
	//B£¥D NAPRAWI£EM Z LISTY
	for (int f = 0; f < i_f; f++)
	{
		double d_sum_left = 0;
		double d_sum_right = 0;

		for (int d = 0; d < i_d; d++)
		{
			d_sum_left += *(pcSolution->pcGetMatrixXd()->tGetFromRowNumberCollumnNumber(d, f));
		}

		for (int m = 0; m < i_m; m++)
		{
			d_sum_right += *(pcSolution->pcGetMatrixXf()->tGetFromRowNumberCollumnNumber(f, m));
		}

		if (d_sum_left < d_sum_right)
		{
			return false;
		}
	}
	return true;
}

bool CMscnProblem::b_check_VI_limit(CMscnSolution* pcSolution)
{
	for (int m = 0; m < i_m; m++)
	{
		double d_sum_left = 0;
		double d_sum_right = 0;

		for (int f = 0; f < i_f; f++)
		{
			d_sum_left += *(pcSolution->pcGetMatrixXf()->tGetFromRowNumberCollumnNumber(f, m));
		}

		for (int s = 0; s < i_s; s++)
		{
			d_sum_right += *(pcSolution->pcGetMatrixXm()->tGetFromRowNumberCollumnNumber(m, s));
		}

		if (d_sum_left < d_sum_right)
		{
			return false;
		}
	}
	return true;
}

void CMscnProblem::vPrintConstrainsSatisfiedErrorsDetailed(bool*& pbLimitsArr)
{
	std::cout << "Limity szczegolowo: \n";
	std::cout << "I war: " << s_bool_to_string(pbLimitsArr[0]) << "\n";
	std::cout << "II war: " << s_bool_to_string(pbLimitsArr[1]) << "\n";
	std::cout << "III war: " << s_bool_to_string(pbLimitsArr[2]) << "\n";
	std::cout << "IV war: " << s_bool_to_string(pbLimitsArr[3]) << "\n";
	std::cout << "V war: " << s_bool_to_string(pbLimitsArr[4]) << "\n";
	std::cout << "VI war: " << s_bool_to_string(pbLimitsArr[5]) << "\n";
}

void CMscnProblem::vPrintConstrainsSatisfiedErrorsSimplified(bool*& pbLimitsArr)
{
	std::cout << "[ Limity: ";
	std::cout << s_bool_to_string(pbLimitsArr[0]) << " | ";
	std::cout << s_bool_to_string(pbLimitsArr[1]) << " | ";
	std::cout << s_bool_to_string(pbLimitsArr[2]) << " | ";
	std::cout << s_bool_to_string(pbLimitsArr[3]) << " | ";
	std::cout << s_bool_to_string(pbLimitsArr[4]) << " | ";
	std::cout << s_bool_to_string(pbLimitsArr[5]) << " ]\n";
}

void CMscnProblem::vPrintConstrainsSatisfiedErrorsSimplifiedSimplified(bool*& pbLimitsArr)
{
	std::cout << "[";
	std::cout << pbLimitsArr[0] << "|";
	std::cout << pbLimitsArr[1] << "|";
	std::cout << pbLimitsArr[2] << "|";
	std::cout << pbLimitsArr[3] << "|";
	std::cout << pbLimitsArr[4] << "|";
	std::cout << pbLimitsArr[5] << "]";
}

//-------------------------Lista 9: Zad 6: min / max------------------------------------------------

double* CMscnProblem::dGetMinForElementOfMatrixXd(int iRowNumber, int iCollumnNumber)
{
	return pc_matrix_xdminmax->tGetFromRowNumberCollumnNumber(iRowNumber, 2 * iCollumnNumber);
}

double* CMscnProblem::dGetMinForElementOfMatrixXf(int iRowNumber, int iCollumnNumber)
{
	return pc_matrix_xfminmax->tGetFromRowNumberCollumnNumber(iRowNumber, 2 * iCollumnNumber);
}

double* CMscnProblem::dGetMinForElementOfMatrixXm(int iRowNumber, int iCollumnNumber)
{
	return pc_matrix_xmminmax->tGetFromRowNumberCollumnNumber(iRowNumber, 2 * iCollumnNumber);
}

double* CMscnProblem::dGetMaxForElementOfMatrixXd(int iRowNumber, int iCollumnNumber)
{
	return pc_matrix_xdminmax->tGetFromRowNumberCollumnNumber(iRowNumber, 2 * iCollumnNumber + 1);
}

double* CMscnProblem::dGetMaxForElementOfMatrixXf(int iRowNumber, int iCollumnNumber)
{
	return pc_matrix_xfminmax->tGetFromRowNumberCollumnNumber(iRowNumber, 2 * iCollumnNumber + 1);
}

double* CMscnProblem::dGetMaxForElementOfMatrixXm(int iRowNumber, int iCollumnNumber)
{
	return pc_matrix_xmminmax->tGetFromRowNumberCollumnNumber(iRowNumber, 2 * iCollumnNumber + 1);
}

//-------------------------Lista 9: Zad 7: zapisywanie / odczytywanie z pliku-----------------------

bool CMscnProblem::bSaveIntoFile(const std::string& sFileName)
{
	if (b_open_file(sFileName, S_W_MAKE_NEW_FILE_AND_WRITE))
	{
		//d
		fprintf(pf_file, "%s %d\n", "D", i_d);

		//f
		fprintf(pf_file, "%s %d\n", "F", i_f);

		//m
		fprintf(pf_file, "%s %d\n", "M", i_m);

		//s
		fprintf(pf_file, "%s %d\n", "S", i_s);

		//sd
		fprintf(pf_file, "\nsd\n");
		for (int ii = 0; ii < i_d; ii++)
		{
			fprintf(pf_file, "%.2f ", *(pc_array_sd->tGet(ii)));
		}

		//sf
		fprintf(pf_file, "\n\nsf\n");
		for (int ii = 0; ii < i_f; ii++)
		{
			fprintf(pf_file, "%.2f ", *(pc_array_sf->tGet(ii)));
		}

		//sm
		fprintf(pf_file, "\n\nsm\n");
		for (int ii = 0; ii < i_m; ii++)
		{
			fprintf(pf_file, "%.2f ", *(pc_array_sm->tGet(ii)));
		}

		//ss
		fprintf(pf_file, "\n\nss\n");
		for (int ii = 0; ii < i_s; ii++)
		{
			double d_buf = *(pc_array_ss->tGet(ii));
			fprintf(pf_file, "%.2f ", d_buf);
		}

		//pc_matrix_cd
		fprintf(pf_file, "\n\ncd\n");
		pc_matrix_cd->vPrintfMatrixIntoFile(pf_file);

		//pc_matrix_cf
		fprintf(pf_file, "\ncf\n");
		pc_matrix_cf->vPrintfMatrixIntoFile(pf_file);

		//pc_matrix_cm
		fprintf(pf_file, "\ncm\n");
		pc_matrix_cm->vPrintfMatrixIntoFile(pf_file);

		//ud
		fprintf(pf_file, "\nud\n");
		for (int ii = 0; ii < i_d; ii++)
		{
			fprintf(pf_file, "%.2f ", *(pc_array_ud->tGet(ii)));
		}

		//uf
		fprintf(pf_file, "\n\nuf\n");
		for (int ii = 0; ii < i_f; ii++)
		{
			fprintf(pf_file, "%.2f ", *(pc_array_uf->tGet(ii)));
		}

		//um
		fprintf(pf_file, "\n\num\n");
		for (int ii = 0; ii < i_m; ii++)
		{
			fprintf(pf_file, "%.2f ", *(pc_array_um->tGet(ii)));
		}

		//p
		fprintf(pf_file, "\n\np\n");
		for (int ii = 0; ii < i_s; ii++)
		{
			fprintf(pf_file, "%.2f ", *(pc_array_p->tGet(ii)));
		}

		//pc_matrix_xdminmax
		fprintf(pf_file, "\n\nxdminmax\n");
		pc_matrix_xdminmax->vPrintfMatrixIntoFile(pf_file);

		//pc_matrix_xfminmax
		fprintf(pf_file, "\nxfminmax\n");
		pc_matrix_xfminmax->vPrintfMatrixIntoFile(pf_file);

		//pc_matrix_xmminmax
		fprintf(pf_file, "\nxmminmax\n");
		pc_matrix_xmminmax->vPrintfMatrixIntoFile(pf_file);

		b_close_file();
		return true;
	}
	else
	{
		return false;
	}
}

bool CMscnProblem::bReadFromFile(const std::string& sFileName)
{
	if (b_open_file(sFileName, S_R_OPEN_EXISTING_FILE_AND_READ))
	{
		//d
		int i_buf;
		char s_buf[80];
		fscanf(pf_file, "%s", s_buf);
		fscanf(pf_file, "%d", &i_buf);
		int i_buf_d = i_buf;

		//f
		fscanf(pf_file, "%s", s_buf);
		fscanf(pf_file, "%d", &i_buf);
		int i_buf_f = i_buf;

		//m
		fscanf(pf_file, "%s", s_buf);
		fscanf(pf_file, "%d", &i_buf);
		int i_buf_m = i_buf;

		//s
		fscanf(pf_file, "%s", s_buf);
		fscanf(pf_file, "%d", &i_buf);
		int i_buf_s = i_buf;

		int i_problem_arr_length = 4 + i_buf_d + i_buf_f + i_buf_m + i_buf_s
			+ i_buf_d * i_buf_f + i_buf_f * i_buf_m + i_buf_m * i_buf_s
			+ i_buf_d + i_buf_f + i_buf_m + i_buf_s
			+ 2 * i_buf_d * i_buf_f + 2 * i_buf_f * i_buf_m + 2 * i_buf_m * i_buf_s;

		double* pd_problem = new double[i_problem_arr_length];
		pd_problem[0] = i_buf_d;
		pd_problem[1] = i_buf_f;
		pd_problem[2] = i_buf_m;
		pd_problem[3] = i_buf_s;

		float f_buf;
		int i_counter = 4;
		//sd
		fscanf(pf_file, "%s", s_buf);
		for (int ii = 0; ii < i_buf_d; ii++)
		{
			fscanf(pf_file, "%f ", &f_buf);
			pd_problem[i_counter++] = f_buf;
		}

		//sf
		fscanf(pf_file, "%s", s_buf);
		for (int ii = 0; ii < i_buf_f; ii++)
		{
			fscanf(pf_file, "%f ", &f_buf);
			pd_problem[i_counter++] = f_buf;
		}

		//sm
		fscanf(pf_file, "%s", s_buf);
		for (int ii = 0; ii < i_buf_m; ii++)
		{
			fscanf(pf_file, "%f ", &f_buf);
			pd_problem[i_counter++] = f_buf;
		}

		//ss
		fscanf(pf_file, "%s", s_buf);
		for (int ii = 0; ii < i_buf_s; ii++)
		{
			fscanf(pf_file, "%f ", &f_buf);
			pd_problem[i_counter++] = f_buf;
		}

		//pc_matrix_cd
		fscanf(pf_file, "%s", s_buf);
		pc_matrix_cd->vFscanfMatrixFromFile(pf_file, pd_problem, i_counter, i_buf_d, i_buf_f);

		//pc_matrix_cf
		fscanf(pf_file, "%s", s_buf);
		pc_matrix_cf->vFscanfMatrixFromFile(pf_file, pd_problem, i_counter, i_buf_f, i_buf_m);

		//pc_matrix_cm
		fscanf(pf_file, "%s", s_buf);
		pc_matrix_cm->vFscanfMatrixFromFile(pf_file, pd_problem, i_counter, i_buf_m, i_buf_s);

		//ud
		fscanf(pf_file, "%s", s_buf);
		for (int ii = 0; ii < i_buf_d; ii++)
		{
			fscanf(pf_file, "%f ", &f_buf);
			pd_problem[i_counter++] = f_buf;
		}

		//uf
		fscanf(pf_file, "%s", s_buf);
		for (int ii = 0; ii < i_buf_f; ii++)
		{
			fscanf(pf_file, "%f ", &f_buf);
			pd_problem[i_counter++] = f_buf;
		}

		//um
		fscanf(pf_file, "%s", s_buf);
		for (int ii = 0; ii < i_buf_m; ii++)
		{
			fscanf(pf_file, "%f ", &f_buf);
			pd_problem[i_counter++] = f_buf;
		}

		//p
		fscanf(pf_file, "%s", s_buf);
		for (int ii = 0; ii < i_buf_s; ii++)
		{
			fscanf(pf_file, "%f ", &f_buf);
			pd_problem[i_counter++] = f_buf;
		}

		//pc_matrix_xdminmax
		fscanf(pf_file, "%s", s_buf);
		pc_matrix_xdminmax->vFscanfMatrixFromFile(pf_file, pd_problem, i_counter, i_buf_d, 2 * i_buf_f);

		//pc_matrix_xfminmax
		fscanf(pf_file, "%s", s_buf);
		pc_matrix_xfminmax->vFscanfMatrixFromFile(pf_file, pd_problem, i_counter, i_buf_f, 2 * i_buf_m);

		//pc_matrix_xmminmax
		fscanf(pf_file, "%s", s_buf);
		pc_matrix_xmminmax->vFscanfMatrixFromFile(pf_file, pd_problem, i_counter, i_buf_m, 2 * i_buf_s);

		b_close_file();

		return bInterpreteArray(pd_problem, i_problem_arr_length);;
	}
	else
	{
		return false;
	}
}

bool CMscnProblem::b_open_file(const std::string& sFileName, std::string sOperation)
{
	pf_file = fopen(sFileName.c_str(), sOperation.c_str());

	if (pf_file == NULL)
	{
		return false; //nie uda³o siê otworzyæ
	}
	else
	{
		return true; //uda³o siê otworzyæ plik / lub stworzyæ ca³kiem nowy plik
	}
}

bool CMscnProblem::b_close_file()
{
	if (pf_file == NULL)
	{
		return false; //Plik nie zosta³ nigdy otwarty / zosta³ ju¿ zmkniêty
	}
	else
	{
		fclose(pf_file);
		pf_file = NULL;
		return true;
	}
}

//-----------METODY POMAGAJ¥CE CONSTRAINTSSATIDFIED I GETQUALITY------------------------------------

bool CMscnProblem::b_does_solution_contain_negative_numbers(CMscnSolution* pcSolution)
{
	int i_solution_arr_length = pcSolution->iReturnAsArraysLength();

	double* pd_solution_array = pcSolution->pdRetunAsArray();

	for (int ii = 0; ii < i_solution_arr_length; ii++)
	{
		if (pd_solution_array[ii] < 0)
		{
			delete pd_solution_array;
			return true;
		}
	}
	delete pd_solution_array;
	return false;
}

std::string CMscnProblem::s_bool_to_string(bool bBool)
{
	if (bBool)
	{
		return "OK";
	}
	else
	{
		return "NIE";
	}
}

//-------zmiany liczb c, d, f, m -> aktualizuj¹c: pc_matrix_cd, pc_matrix_cf, pc_matrix_cm, sd, sf, sm, ss----

bool CMscnProblem::bSetD(int iD)
{
	if (iD > 0)
	{
		i_d = iD;

		//zmiana macierzy
		pc_matrix_cd->bResizeMatrix(iD, i_f);
		pc_matrix_xdminmax->bResizeMatrix(iD, 2 * i_f);

		//zmiana tablicy
		pc_array_sd->bResize(iD);
		pc_array_ud->bResize(iD);
		return true;
	}
	else
	{
		return false;
	}
}

bool CMscnProblem::bSetF(int iF)
{
	if (iF > 0)
	{
		i_f = iF;

		//zmiana macierzy
		pc_matrix_cf->bResizeMatrix(iF, i_m);
		pc_matrix_cd->bResizeMatrix(i_d, iF);
		pc_matrix_xdminmax->bResizeMatrix(i_d, 2 * iF);
		pc_matrix_xfminmax->bResizeMatrix(iF, 2 * i_m);

		//zmiana tablic
		pc_array_sf->bResize(iF);
		pc_array_uf->bResize(iF);
		return true;
	}
	else
	{
		return false;
	}
}

bool CMscnProblem::bSetM(int iM)
{
	if (iM > 0)
	{
		i_m = iM;

		//zmiana macierzy
		pc_matrix_cm->bResizeMatrix(iM, i_s);
		pc_matrix_cf->bResizeMatrix(i_f, iM);
		pc_matrix_xfminmax->bResizeMatrix(i_f, 2 * iM);
		pc_matrix_xmminmax->bResizeMatrix(iM, 2 * i_s);

		//zmiana tablic
		pc_array_sm->bResize(iM);
		pc_array_um->bResize(iM);
		return true;
	}
	else
	{
		return false;
	}
}

bool CMscnProblem::bSetS(int iS)
{
	if (iS > 0)
	{
		i_s = iS;

		//zmiana macierzy
		pc_matrix_cm->bResizeMatrix(i_m, iS);
		pc_matrix_xmminmax->bResizeMatrix(i_m, 2 * iS);

		//zmiana tablic
		pc_array_ss->bResize(iS);
		pc_array_p->bResize(iS);
		return true;
	}
	else
	{
		return false;
	}
}

bool CMscnProblem::bSetDFMS(int iD, int iF, int iM, int iS)
{
	if (bSetD(iD) && bSetF(iF) && bSetM(iM) && bSetS(iS))
	{
		return true;
	}
	else
	{
		i_d = 0;
		i_f = 0;
		i_m = 0;
		i_s = 0;
		return false;
	}
}


//--------------------------------GETTERY LICZB-----------------------------------------------------

int CMscnProblem::iGetD()
{
	return i_d;
}

int CMscnProblem::iGetF()
{
	return i_f;
}

int CMscnProblem::iGetM()
{
	return i_m;
}

int CMscnProblem::iGetS()
{
	return i_s;
}

//-------------------ustawianie poszczególnych komórek w macierzach---------------------------------

bool CMscnProblem::bCdMatrixSetAt(double iNewVal, int iRowNumber, int iCollumnNumber)
{
	double* piNewVal = new double(iNewVal);
	return pc_matrix_cd->bSetAtRowNumberCollumnNumber(piNewVal, iRowNumber, iCollumnNumber);
}

bool CMscnProblem::bCfMatrixSetAt(double iNewVal, int iRowNumber, int iCollumnNumber)
{
	double* piNewVal = new double(iNewVal);
	return pc_matrix_cf->bSetAtRowNumberCollumnNumber(piNewVal, iRowNumber, iCollumnNumber);
}

bool CMscnProblem::bCmMatrixSetAt(double iNewVal, int iRowNumber, int iCollumnNumber)
{
	double* piNewVal = new double(iNewVal);
	return pc_matrix_cm->bSetAtRowNumberCollumnNumber(piNewVal, iRowNumber, iCollumnNumber);
}

bool CMscnProblem::bXdminmaxMatrixSetAt(double iNewVal, int iRowNumber, int iCollumnNumber)
{
	double* piNewVal = new double(iNewVal);
	return pc_matrix_xdminmax->bSetAtRowNumberCollumnNumber(piNewVal, iRowNumber, iCollumnNumber);
}

bool CMscnProblem::bXfminmaxMatrixSetAt(double iNewVal, int iRowNumber, int iCollumnNumber)
{
	double* piNewVal = new double(iNewVal);
	return pc_matrix_xfminmax->bSetAtRowNumberCollumnNumber(piNewVal, iRowNumber, iCollumnNumber);
}

bool CMscnProblem::bXmminmaxMatrixSetAt(double iNewVal, int iRowNumber, int iCollumnNumber)
{
	double* piNewVal = new double(iNewVal);
	return pc_matrix_xmminmax->bSetAtRowNumberCollumnNumber(piNewVal, iRowNumber, iCollumnNumber);
}


//-------------------ustawianie poszczególnych komórek w tablicach----------------------------------

bool CMscnProblem::bSdArraySetAt(double dNewVal, int iOffset)
{
	double* pdNewVal = new double(dNewVal);
	return pc_array_sd->bSetAt(pdNewVal, iOffset);
}

bool CMscnProblem::bSfArraySetAt(double dNewVal, int iOffset)
{
	double* pdNewVal = new double(dNewVal);
	return pc_array_sf->bSetAt(pdNewVal, iOffset);
}

bool CMscnProblem::bSmArraySetAt(double dNewVal, int iOffset)
{
	double* pdNewVal = new double(dNewVal);
	return pc_array_sm->bSetAt(pdNewVal, iOffset);
}

bool CMscnProblem::bSsArraySetAt(double dNewVal, int iOffset)
{
	double* pdNewVal = new double(dNewVal);
	return pc_array_ss->bSetAt(pdNewVal, iOffset);
}

bool CMscnProblem::bUdArraySetAt(double dNewVal, int iOffset)
{
	double* pdNewVal = new double(dNewVal);
	return pc_array_ud->bSetAt(pdNewVal, iOffset);
}

bool CMscnProblem::bUfArraySetAt(double dNewVal, int iOffset)
{
	double* pdNewVal = new double(dNewVal);
	return pc_array_uf->bSetAt(pdNewVal, iOffset);
}

bool CMscnProblem::bUmArraySetAt(double dNewVal, int iOffset)
{
	double* pdNewVal = new double(dNewVal);
	return pc_array_um->bSetAt(pdNewVal, iOffset);
}

bool CMscnProblem::bpArraySetAt(double dNewVal, int iOffset)
{
	double* pdNewVal = new double(dNewVal);
	return pc_array_p->bSetAt(pdNewVal, iOffset);
}

//-------------------------OBS£UGA: MACIERZY Z / DO TABLIC------------------------------------------

bool CMscnProblem::bInterpreteArray(double* pdProblem, int iProblemArrLength)
{
	if (pdProblem == NULL)
	{
		std::cout << "NULL\n";
		return false;
	}
	else if (iProblemArrLength < 4)
	{
		std::cout << "size\n";
		return false;
	}
	else
	{
		int i_buf_d = pdProblem[0];
		int i_buf_f = pdProblem[1];
		int i_buf_m = pdProblem[2];
		int i_buf_s = pdProblem[3];

		if (iProblemArrLength != 4 + i_buf_d + i_buf_f + i_buf_m + i_buf_s
			+ i_buf_d * i_buf_f + i_buf_f * i_buf_m + i_buf_m * i_buf_s
			+ i_buf_d + i_buf_f + i_buf_m + i_buf_s
			+ 2 * i_buf_d * i_buf_f + 2 * i_buf_f * i_buf_m + 2 * i_buf_m * i_buf_s)
		{
			std::cout << "should be size: " << iProblemArrLength << "\n";
			std::cout << "is: " << 4 + i_buf_d + i_buf_f + i_buf_m + i_buf_s
				+ i_buf_d * i_buf_f + i_buf_f * i_buf_m + i_buf_m * i_buf_s
				+ i_buf_d + i_buf_f + i_buf_m + i_buf_s
				+ 2 * i_buf_d * i_buf_f + 2 * i_buf_f * i_buf_m + 2 * i_buf_m * i_buf_s << "\n";
			return false;
		}
		else
		{
			bSetD(i_buf_d);
			bSetF(i_buf_f);
			bSetM(i_buf_m);
			bSetS(i_buf_s);

			int i_counter = 4;;

			//sd
			for (int ii = 0; ii < i_d; ii++)
			{
				pc_array_sd->bSetAt(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += i_d;

			//sf
			for (int ii = 0; ii < i_f; ii++)
			{
				pc_array_sf->bSetAt(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += i_f;

			//sm
			for (int ii = 0; ii < i_m; ii++)
			{
				pc_array_sm->bSetAt(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += i_m;

			//ss
			for (int ii = 0; ii < i_s; ii++)
			{
				pc_array_ss->bSetAt(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += i_s;

			//pc_matrix_cd
			for (int ii = 0; ii < pc_matrix_cd->iReturnNumberOfElements(); ii++)
			{
				pc_matrix_cd->bSetAtNumber(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += pc_matrix_cd->iReturnNumberOfElements();

			//pc_matrix_cf
			for (int ii = 0; ii < pc_matrix_cf->iReturnNumberOfElements(); ii++)
			{
				pc_matrix_cf->bSetAtNumber(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += pc_matrix_cf->iReturnNumberOfElements();

			//pc_matrix_cm
			for (int ii = 0; ii < pc_matrix_cm->iReturnNumberOfElements(); ii++)
			{
				pc_matrix_cm->bSetAtNumber(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += pc_matrix_cm->iReturnNumberOfElements();

			//ud
			for (int ii = 0; ii < i_d; ii++)
			{
				pc_array_ud->bSetAt(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += i_d;

			//uf
			for (int ii = 0; ii < i_f; ii++)
			{
				pc_array_uf->bSetAt(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += i_f;

			//um
			for (int ii = 0; ii < i_m; ii++)
			{
				pc_array_um->bSetAt(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += i_m;

			//p
			for (int ii = 0; ii < i_s; ii++)
			{
				pc_array_p->bSetAt(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += i_s;

			//pc_matrix_xdminmax
			for (int ii = 0; ii < pc_matrix_xdminmax->iReturnNumberOfElements(); ii++)
			{
				pc_matrix_xdminmax->bSetAtNumber(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += pc_matrix_xdminmax->iReturnNumberOfElements();

			//pc_matrix_xfminmax
			for (int ii = 0; ii < pc_matrix_xfminmax->iReturnNumberOfElements(); ii++)
			{
				pc_matrix_xfminmax->bSetAtNumber(new double(pdProblem[i_counter + ii]), ii);
			}
			i_counter += pc_matrix_xfminmax->iReturnNumberOfElements();

			//pc_matrix_xmminmax
			for (int ii = 0; ii < pc_matrix_xmminmax->iReturnNumberOfElements(); ii++)
			{
				pc_matrix_xmminmax->bSetAtNumber(new double(pdProblem[i_counter + ii]), ii);
			}

			i_counter += pc_matrix_xmminmax->iReturnNumberOfElements();
			return true;
		}
	}
}

int CMscnProblem::iReturnAsArraysLength(int iD, int iF, int iM, int iS)
{
	return 4 + iD + iF + iM + iS
		+ iD * iF + iF * iM + iM * iS
		+ iD + iF + iM + iS
		+ 2 * iD * iF + 2 * iF * iM + 2 * iM * iS;
}

double* CMscnProblem::pdReturnAsArray()
{
	double* pd_pro_arr = new double[iReturnAsArraysLength(i_d, i_f, i_m, i_s)];

	pd_pro_arr[0] = i_d;
	pd_pro_arr[1] = i_f;
	pd_pro_arr[2] = i_m;
	pd_pro_arr[3] = i_s;

	int i_counter = 4;;

	//sd
	for (int ii = 0; ii < i_d; ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_array_sd->tGet(ii));
	}
	i_counter += i_d;

	//sf
	for (int ii = 0; ii < i_f; ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_array_sf->tGet(ii));
	}
	i_counter += i_f;

	//sm
	for (int ii = 0; ii < i_m; ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_array_sm->tGet(ii));
	}
	i_counter += i_m;

	//ss
	for (int ii = 0; ii < i_s; ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_array_ss->tGet(ii));
	}
	i_counter += i_s;

	//pc_matrix_cd
	for (int ii = 0; ii < pc_matrix_cd->iReturnNumberOfElements(); ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_matrix_cd->tGetFromNumber(ii));
	}
	i_counter += pc_matrix_cd->iReturnNumberOfElements();

	//pc_matrix_cf
	for (int ii = 0; ii < pc_matrix_cf->iReturnNumberOfElements(); ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_matrix_cf->tGetFromNumber(ii));
	}
	i_counter += pc_matrix_cf->iReturnNumberOfElements();

	//pc_matrix_cm
	for (int ii = 0; ii < pc_matrix_cm->iReturnNumberOfElements(); ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_matrix_cm->tGetFromNumber(ii));
	}
	i_counter += pc_matrix_cm->iReturnNumberOfElements();

	//ud
	for (int ii = 0; ii < i_d; ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_array_ud->tGet(ii));
	}
	i_counter += i_d;

	//uf
	for (int ii = 0; ii < i_f; ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_array_uf->tGet(ii));
	}
	i_counter += i_f;

	//um
	for (int ii = 0; ii < i_m; ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_array_um->tGet(ii));
	}
	i_counter += i_m;

	//p
	for (int ii = 0; ii < i_s; ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_array_p->tGet(ii));
	}
	i_counter += i_s;

	//pc_matrix_xdminmax
	for (int ii = 0; ii < pc_matrix_xdminmax->iReturnNumberOfElements(); ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_matrix_xdminmax->tGetFromNumber(ii));
	}
	i_counter += pc_matrix_xdminmax->iReturnNumberOfElements();

	//pc_matrix_xfminmax
	for (int ii = 0; ii < pc_matrix_xfminmax->iReturnNumberOfElements(); ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_matrix_xfminmax->tGetFromNumber(ii));
	}
	i_counter += pc_matrix_xfminmax->iReturnNumberOfElements();

	//pc_matrix_xmminmax
	for (int ii = 0; ii < pc_matrix_xmminmax->iReturnNumberOfElements(); ii++)
	{
		pd_pro_arr[ii + i_counter] = *(pc_matrix_xmminmax->tGetFromNumber(ii));
	}
	i_counter += pc_matrix_xmminmax->iReturnNumberOfElements();

	return pd_pro_arr;
}

//----------------------------GETTERY MACIERZY I TABLIC---------------------------------------------

CArray<double>* CMscnProblem::pcGetArraySd()
{
	return pc_array_sd;
}

CArray<double>* CMscnProblem::pcGetArraySf()
{
	return pc_array_sf;
}

CArray<double>* CMscnProblem::pcGetArraySm()
{
	return pc_array_sm;
}

CArray<double>* CMscnProblem::pcGetArraySs()
{
	return pc_array_ss;
}

CMatrix<double>* CMscnProblem::pcGetMatrixCd()
{
	return pc_matrix_cd;
}

CMatrix<double>* CMscnProblem::pcGetMatrixCf()
{
	return pc_matrix_cf;
}

CMatrix<double>* CMscnProblem::pcGetMatrixCm()
{
	return pc_matrix_cm;
}

CArray<double>* CMscnProblem::pcGetArrayUd()
{
	return pc_array_ud;
}

CArray<double>* CMscnProblem::pcGetArrayUf()
{
	return pc_array_uf;
}

CArray<double>* CMscnProblem::pcGetArrayUm()
{
	return pc_array_um;
}

CArray<double>* CMscnProblem::pcGetArrayP()
{
	return pc_array_p;
}

CMatrix<double>* CMscnProblem::pcGetMatrixXdminmax()
{
	return pc_matrix_xdminmax;
}

CMatrix<double>* CMscnProblem::pcGetMatrixXfminmax()
{
	return pc_matrix_xfminmax;
}

CMatrix<double>* CMscnProblem::pcGetMatrixXmminmax()
{
	return pc_matrix_xmminmax;
}

//-------------------------GETTERY I SETTERY ZMIENNYCH-----------------------------------------

int CMscnProblem::iGetNumberOfGetQualityCalls()
{
	return i_number_of_get_quality_calls;
}

void CMscnProblem::vResetNumberOfGetQualityCalls()
{
	i_number_of_get_quality_calls = 0;
}

//-------------------wypisywanie--------------------------------------------------------------------

void CMscnProblem::vPrintEverything()
{
	std::cout << "PROBLEM:\n";
	vPrintD();
	vPrintArraySd();

	vPrintF();
	vPrintArraySf();

	vPrintM();
	vPrintArraySm();

	vPrintS();
	vPrintArraySs();

	vPrintMatrixCd();
	vPrintMatrixCf();
	vPrintMatrixCm();

	vPrintArrayUd();
	vPrintArrayUf();
	vPrintArrayUm();
	vPrintArrayP();

	vPrintMatrixXdminmax();
	vPrintMatrixXfminmax();
	vPrintMatrixXmminmax();
}

void CMscnProblem::vPrintD()
{
	std::cout << "d: " << i_d << "\n";
}

void CMscnProblem::vPrintF()
{
	std::cout << "f: " << i_f << "\n";
}

void CMscnProblem::vPrintM()
{
	std::cout << "m: " << i_m << "\n";
}

void CMscnProblem::vPrintS()
{
	std::cout << "s: " << i_s << "\n";
}

void CMscnProblem::vPrintArraySd()
{
	std::cout << "sd:";
	pc_array_sd->vPrintArr();
}

void CMscnProblem::vPrintArraySf()
{
	std::cout << "sf:";
	pc_array_sf->vPrintArr();
}

void CMscnProblem::vPrintArraySm()
{
	std::cout << "sm:";
	pc_array_sm->vPrintArr();
}

void CMscnProblem::vPrintArraySs()
{
	std::cout << "ss:";
	pc_array_ss->vPrintArr();
}

void CMscnProblem::vPrintMatrixCd()
{
	std::cout << "pc_matrix_cd:";
	pc_matrix_cd->vPrintMatrix();
}

void CMscnProblem::vPrintMatrixCf()
{
	std::cout << "pc_matrix_cf:";
	pc_matrix_cf->vPrintMatrix();
}

void CMscnProblem::vPrintMatrixCm()
{
	std::cout << "pc_matrix_cm:";
	pc_matrix_cm->vPrintMatrix();
}

void CMscnProblem::vPrintArrayUd()
{
	std::cout << "ud:";
	pc_array_ud->vPrintArr();
}

void CMscnProblem::vPrintArrayUf()
{
	std::cout << "uf:";
	pc_array_uf->vPrintArr();
}

void CMscnProblem::vPrintArrayUm()
{
	std::cout << "um:";
	pc_array_um->vPrintArr();
}

void CMscnProblem::vPrintArrayP()
{
	std::cout << "p:";
	pc_array_p->vPrintArr();
}

void CMscnProblem::vPrintMatrixXdminmax()
{
	std::cout << "pc_matrix_xdminmax:";
	pc_matrix_xdminmax->vPrintMatrix();
}

void CMscnProblem::vPrintMatrixXfminmax()
{
	std::cout << "pc_matrix_xfminmax:";
	pc_matrix_xfminmax->vPrintMatrix();
}

void CMscnProblem::vPrintMatrixXmminmax()
{
	std::cout << "pc_matrix_xmminmax:";
	pc_matrix_xmminmax->vPrintMatrix();
}

//-------------------------Lista 10: Zad 2: vGenerateInstance---------------------------------------

bool CMscnProblem::bGenerateInstance(int iInstanceSeed, int iNewD, int iNewF, int iNewM, int iNewS)
{
	if (!bSetD(iNewD) || !bSetF(iNewF) || !bSetM(iNewM) || !bSetS(iNewS))
	{
		i_d = 0;
		i_f = 0;
		i_m = 0;
		i_s = 0;
		return false;
	}
	else
	{
		CRandom* pc_generator;
		pc_generator = new CRandom(iInstanceSeed);

		pc_matrix_cd->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);
		pc_matrix_cf->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);
		pc_matrix_cm->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);

		pc_array_ud->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);
		pc_array_uf->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);
		pc_array_um->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);

		pc_array_sd->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_UNITS_NUMBER / i_d);
		int i_sd_sum = pc_array_sd->dSumOfThatArray();
		pc_array_sf->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, i_sd_sum / i_f);
		int i_sf_sum = pc_array_sf->dSumOfThatArray();
		pc_array_sm->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, i_sf_sum / i_m);
		int i_sm_sum = pc_array_sm->dSumOfThatArray();
		pc_array_ss->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, i_sm_sum / i_s);

		pc_array_p->bRandomizeInt(pc_generator, I_MIN_ARRAY_P, I_MAX_ARRAY_P);

		pc_matrix_xdminmax->bFillWithTwoNumbers(I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);
		pc_matrix_xfminmax->bFillWithTwoNumbers(I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);
		pc_matrix_xmminmax->bFillWithTwoNumbers(I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);

		v_generate_with_first_limit(pc_generator);
		v_generate_with_second_limit(pc_generator);
		v_generate_with_third_limit(pc_generator);
		v_generate_with_fourth_limit(pc_generator);
		v_generate_with_fivth_limit(pc_generator);
		v_generate_with_sixth_limit(pc_generator);

		delete pc_generator;
		return true;
	}
}

bool CMscnProblem::bGenerateInstanceTotallyRandom(unsigned int uiInstanceSeed, int iNewD, int iNewF, int iNewM, int iNewS)
{
	if (!bSetD(iNewD) || !bSetF(iNewF) || !bSetM(iNewM) || !bSetS(iNewS))
	{
		i_d = 0;
		i_f = 0;
		i_m = 0;
		i_s = 0;
		return false;
	}
	else
	{
		CRandom* pc_generator;
		pc_generator = new CRandom(uiInstanceSeed);

		pc_matrix_cd->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);
		pc_matrix_cf->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);
		pc_matrix_cm->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);

		pc_array_ud->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);
		pc_array_uf->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);
		pc_array_um->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);

		pc_array_sd->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);
		pc_array_sf->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);
		pc_array_sm->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);
		pc_array_ss->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);

		pc_array_p->bRandomizeInt(pc_generator, I_DEFAULT_MINIMUM_ARRAY, I_DEFAULT_MAXIMUM_ARRAY);

		pc_matrix_xdminmax->bFillWithTwoNumbers(I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);
		pc_matrix_xfminmax->bFillWithTwoNumbers(I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);
		pc_matrix_xmminmax->bFillWithTwoNumbers(I_DEFAULT_MINIMUM_MATRIX, I_DEFAULT_MAXIMUM_MATRIX);

		delete pc_generator;
		return true;
	}
}

bool CMscnProblem::bGenerateInstanceFromConsts(unsigned int uiInstanceSeed, int iNewD, int iNewF, int iNewM, int iNewS)
{
	if (!bSetD(iNewD) || !bSetF(iNewF) || !bSetM(iNewM) || !bSetS(iNewS))
	{
		i_d = 0;
		i_f = 0;
		i_m = 0;
		i_s = 0;
		return false;
	}
	else
	{
		CRandom* pc_generator;
		pc_generator = new CRandom(uiInstanceSeed);

		pc_matrix_cd->bRandomizeInt(pc_generator, I_MIN_MATRIX_CD, I_MAX_MATRIX_CD);
		pc_matrix_cf->bRandomizeInt(pc_generator, I_MIN_MATRIX_CF, I_MAX_MATRIX_CD);
		pc_matrix_cm->bRandomizeInt(pc_generator, I_MIN_MATRIX_CM, I_MAX_MATRIX_CD);

		pc_array_ud->bRandomizeInt(pc_generator, I_MIN_ARRAY_UD, I_MAX_ARRAY_UD);
		pc_array_uf->bRandomizeInt(pc_generator, I_MIN_ARRAY_UF, I_MAX_ARRAY_UD);
		pc_array_um->bRandomizeInt(pc_generator, I_MIN_ARRAY_UM, I_MAX_ARRAY_UD);

		pc_array_sd->bRandomizeInt(pc_generator, I_MIN_ARRAY_SD, I_MAX_ARRAY_SD);
		pc_array_sf->bRandomizeInt(pc_generator, I_MIN_ARRAY_SF, I_MAX_ARRAY_SF);
		pc_array_sm->bRandomizeInt(pc_generator, I_MIN_ARRAY_SM, I_MAX_ARRAY_SM);
		pc_array_ss->bRandomizeInt(pc_generator, I_MIN_ARRAY_SS, I_MAX_ARRAY_SS);

		pc_array_p->bRandomizeInt(pc_generator, I_MIN_ARRAY_P, I_MAX_ARRAY_P);

		pc_matrix_xdminmax->bFillWithTwoNumbers(I_MIN_MATRIX_XDMINMAX, I_MAX_MATRIX_XDMINMAX);
		pc_matrix_xfminmax->bFillWithTwoNumbers(I_MIN_MATRIX_XFMINMAX, I_MAX_MATRIX_XDMINMAX);
		pc_matrix_xmminmax->bFillWithTwoNumbers(I_MIN_MATRIX_XMMINMAX, I_MAX_MATRIX_XDMINMAX);

		delete pc_generator;
		return true;
	}
}

void CMscnProblem::v_generate_with_first_limit(CRandom* pcGenerator)
{
	for (int d = 0; d < i_d; d++)
	{
		double d_max = *(pc_array_sd->tGet(d)) / i_f;

		for (int f = 0; f < i_f; f++)
		{
			pc_matrix_xdminmax->bSetAtRowNumberCollumnNumber(new double(d_max), d, 2 * f + 1);
		}
	}
}

void CMscnProblem::v_generate_with_second_limit(CRandom* pcGenerator)
{
	for (int f = 0; f < i_f; f++)
	{
		double d_max = *(pc_array_sf->tGet(f)) / i_m;

		for (int m = 0; m < i_m; m++)
		{
			pc_matrix_xfminmax->bSetAtRowNumberCollumnNumber(new double(d_max), f, 2 * m + 1);
		}
	}
}

void CMscnProblem::v_generate_with_third_limit(CRandom* pcGenerator)
{
	for (int m = 0; m < i_m; m++)
	{
		double d_max = *(pc_array_sm->tGet(m)) / i_s;

		for (int s = 0; s < i_s; s++)
		{
			pc_matrix_xmminmax->bSetAtRowNumberCollumnNumber(new double(d_max), m, 2 * s + 1);
		}
	}
}

void CMscnProblem::v_generate_with_fourth_limit(CRandom* pcGenerator)
{
	for (int s = 0; s < i_s; s++)
	{
		double d_max = *(pc_array_ss->tGet(s)) / i_m;

		for (int m = 0; m < i_m; m++)
		{
			if (*(pc_matrix_xmminmax->tGetFromRowNumberCollumnNumber(m, 2 * s + 1)) > d_max)
			{
				pc_matrix_xmminmax->bSetAtRowNumberCollumnNumber(new double(d_max), m, 2 * s + 1);
			}
		}
	}
}

void CMscnProblem::v_generate_with_fivth_limit(CRandom* pcGenerator)
{
	for (int f = 0; f < i_f; f++)
	{
		double d_sum_right = 0;

		for (int m = 0; m < i_m; m++)
		{
			d_sum_right += *(pc_matrix_xfminmax->tGetFromRowNumberCollumnNumber(f, 2 * m + 1));
		}

		double d_min = d_sum_right / i_d;

		for (int d = 0; d < i_d; d++)
		{
			double d_buf_max = *(pc_matrix_xdminmax->tGetFromRowNumberCollumnNumber(d, 2 * f + 1));
			if (d_buf_max < d_min)
			{
				pc_matrix_xdminmax->bSetAtRowNumberCollumnNumber(new double(d_buf_max), d, 2 * f);
			}
			else
			{
				pc_matrix_xdminmax->bSetAtRowNumberCollumnNumber(new double(d_min), d, 2 * f);
			}
		}
	}
}

void CMscnProblem::v_generate_with_sixth_limit(CRandom* pcGenerator)
{
	for (int m = 0; m < i_m; m++)
	{

		double d_sum_right = 0;

		for (int s = 0; s < i_s; s++)
		{
			d_sum_right += *(pc_matrix_xmminmax->tGetFromRowNumberCollumnNumber(m, 2 * s + 1));
		}

		double d_min = d_sum_right / i_f;

		for (int f = 0; f < i_f; f++)
		{
			double d_buf_max = *(pc_matrix_xfminmax->tGetFromRowNumberCollumnNumber(f, 2 * m + 1));
			if (d_buf_max < d_min)
			{
				pc_matrix_xfminmax->bSetAtRowNumberCollumnNumber(new double(d_buf_max), f, 2 * m);
			}
			else
			{
				pc_matrix_xfminmax->bSetAtRowNumberCollumnNumber(new double(d_min), f, 2 * m);
			}
		}
	}
}

//-------------------------Lista 11:----------------------------------------------------------------

double CMscnProblem::dGetMinimumForGene(int iOffset)
{
	return d_get_max_min_for_gene(iOffset, true);
}

double CMscnProblem::dGetMaximumForGene(int iOffset)
{
	return d_get_max_min_for_gene(iOffset, false);
}

double CMscnProblem::d_get_max_min_for_gene(int iOffset, bool bTrueMinFalseMax)
{
	int i_row, i_column;

	CMatrix<double>*pc_actual_matrix = pc_calculate_x_matrix_column_and_row_from_offset(iOffset, i_row, i_column);

	if (pc_actual_matrix != NULL)
	{
		if (bTrueMinFalseMax)
		{
			return *(pc_actual_matrix->tGetFromRowNumberCollumnNumber(i_row, 2 * i_column));
		}
		else
		{
			return *(pc_actual_matrix->tGetFromRowNumberCollumnNumber(i_row, 2 * i_column + 1));
		}
	}
	else
	{
		return -1;
	}
}


CMatrix<double> *& CMscnProblem::pc_calculate_x_matrix_column_and_row_from_offset(int iOffset, int& iRow, int& iColumn)
{
	int i_off;

	CMatrix<double>*pc_actual_matrix = pc_calculate_which_x_matrix(iOffset, i_off);
	int i_actual_mat_row = pc_actual_matrix->iGetRowsNumber();
	int i_actual_mat_col = pc_actual_matrix->iGetColumnsNumber() / 2;

	if (pc_actual_matrix != NULL)
	{
		iColumn = i_off % i_actual_mat_col;
		iRow = i_off / i_actual_mat_col;
	}
	else
	{
		iRow = -1;
		iColumn = -1;
	}

	return pc_actual_matrix;
}

CMatrix<double> *& CMscnProblem::pc_calculate_which_x_matrix(int iOffset, int &iNewOffset)
{
	if (iOffset < 4 || iOffset >= 4 + i_d * i_f + i_f * i_m + i_m * i_s)
	{
		iNewOffset = -1;
	}
	else if (iOffset >= 4 && iOffset < 4 + i_d * i_f)
	{
		iNewOffset = iOffset - 4;
		return pc_matrix_xdminmax;
	}
	else if (iOffset >= 4 + i_d * i_f && iOffset < 4 + i_d * i_f + i_f * i_m)
	{
		iNewOffset = iOffset - (4 + i_d * i_f);
		return pc_matrix_xfminmax;
	}
	else if (iOffset >= 4 + i_d * i_f + i_f * i_m && iOffset < 4 + i_d * i_f + i_f * i_m + i_m * i_s)
	{
		iNewOffset = iOffset - (4 + i_d * i_f + i_f * i_m);
		return pc_matrix_xmminmax;
	}
}

//-------------------------Lista 11: ALGORYTMY------------------------------------------------------

bool CMscnProblem::bSetAlgorithm(CAlgorithm *pcNewAlgorithm)
{
	if (pcNewAlgorithm != NULL)
	{
		pc_actual_algorithm = pcNewAlgorithm;
		return true;
	}
	else
	{
		return false;
	}
}

CMscnSolution * CMscnProblem::pcUseAlgorithm()
{
	return pc_actual_algorithm->run(this);
}
