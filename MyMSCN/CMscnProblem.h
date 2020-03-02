#pragma once
#include <iostream>
#include "CMatrix.h"
#include "CArray.h"
#include "CMscnSolution.h"
#include <string>
#include "CRandom.h"
#include "CAlgorithm.h"

class CMscnProblem
{
private:

	//-------------------------STA£E DO LOSOWANIA---------------------------------------------------

	const static int I_DEFAULT_MINIMUM_ARRAY;
	const static int I_DEFAULT_MAXIMUM_ARRAY;

	const static int I_DEFAULT_MINIMUM_MATRIX;
	const static int I_DEFAULT_MAXIMUM_MATRIX;

	const static int I_MIN_MATRIX_CD;
	const static int I_MAX_MATRIX_CD;

	const static int I_MIN_MATRIX_CF;
	const static int I_MAX_MATRIX_CF;

	const static int I_MIN_MATRIX_CM;
	const static int I_MAX_MATRIX_CM;

	const static int I_MIN_ARRAY_SD;
	const static int I_MAX_ARRAY_SD;

	const static int I_MIN_ARRAY_SF;
	const static int I_MAX_ARRAY_SF;

	const static int I_MIN_ARRAY_SM;
	const static int I_MAX_ARRAY_SM;

	const static int I_MIN_ARRAY_SS;
	const static int I_MAX_ARRAY_SS;

	const static int I_MIN_ARRAY_UD;
	const static int I_MAX_ARRAY_UD;

	const static int I_MIN_ARRAY_UF;
	const static int I_MAX_ARRAY_UF;

	const static int I_MIN_ARRAY_UM;
	const static int I_MAX_ARRAY_UM;

	const static int I_MIN_ARRAY_P;
	const static int I_MAX_ARRAY_P;

	const static int I_MIN_MATRIX_XDMINMAX;
	const static int I_MAX_MATRIX_XDMINMAX;

	const static int I_MIN_MATRIX_XFMINMAX;
	const static int I_MAX_MATRIX_XFMINMAX;

	const static int I_MIN_MATRIX_XMMINMAX;
	const static int I_MAX_MATRIX_XMMINMAX;

	const static int I_DEFAULT_UNITS_NUMBER;

	//-------------------------KODY B£ÊDÓW----------------------------------------------------------

	const static int I_ERROR_CODE_NULL_POINTER;

	const static int I_ERROR_CODE_WRONG_ARR_LENGTH;

	const static int I_ERROR_CODE_CONTAINS_NEGATIVE_NUMBERS;

	const static int I_ERROR_CODE_NO_ERROR;

	//-------------------------ILOŒCI PODMIOTÓW-----------------------------------------------------

	int i_d;

	int i_f;

	int i_m;

	int i_s;

	//-------------------------MACIERZE-------------------------------------------------------------

	CMatrix<double>*pc_matrix_cd;

	CMatrix<double>*pc_matrix_cf;

	CMatrix<double>*pc_matrix_cm;

	CMatrix<double> *pc_matrix_xdminmax;

	CMatrix<double> *pc_matrix_xfminmax;

	CMatrix<double> *pc_matrix_xmminmax;

	//-------------------------TABLICE--------------------------------------------------------------

	CArray<double>*pc_array_sd;

	CArray<double>*pc_array_sf;

	CArray<double>*pc_array_sm;

	CArray<double>*pc_array_ss;

	CArray<double>*pc_array_ud;

	CArray<double>*pc_array_uf;

	CArray<double>*pc_array_um;

	CArray<double>*pc_array_p;

	//---------------------METODY POMAGAJ¥CE GETQUALITY---------------------------------------------

	double d_calculate_quality_of_solution(CMscnSolution *pcSolution);

	double d_calculate_P(CMscnSolution * pcSolution);

	double d_calculate_Kt(CMscnSolution * pcSolution);

	double d_calculate_Ku(CMscnSolution * pcSolution);

	//-------------------METODY POMAGAJ¥CE CONSTRAINTSSATIDFIED-------------------------------------

	void v_check_constraints_satisfied(double *pdSolution, int iSolutionArrLength, bool *&pbLimitsArr);

	void v_check_constraints_satisfied(CMscnSolution *pcSolution, bool *&pbLimitsArr);

	bool b_check_I_limit(CMscnSolution * pcSolution);

	bool b_check_II_limit(CMscnSolution * pcSolution);

	bool b_check_III_limit(CMscnSolution * pcSolution);

	bool b_check_IV_limit(CMscnSolution * pcSolution);

	bool b_check_V_limit(CMscnSolution * pcSolution);

	bool b_check_VI_limit(CMscnSolution * pcSolution);

	//-----------INNE METODY POMOCNICZE-------------------------------------------------------------

	bool b_does_solution_contain_negative_numbers(CMscnSolution * pcSolution);

	std::string s_bool_to_string(bool bBool);

	CMatrix<double>*& pc_calculate_x_matrix_column_and_row_from_offset(int iOffset, int &iRow, int &iColumn);

	CMatrix<double>*& pc_calculate_which_x_matrix(int iOffset, int &iNewOffset);

	double d_get_max_min_for_gene(int iOffset, bool bTrueMinFalseMax);
	
	//-----------METODY POMAGAJ¥CE ZWI¥ZANE Z GENEROWANIEM PROBLEMU---------------------------------

	void v_generate_with_first_limit(CRandom *pcGenerator);

	void v_generate_with_second_limit(CRandom *pcGenerator);

	void v_generate_with_third_limit(CRandom *pcGenerator);

	void v_generate_with_fourth_limit(CRandom *pcGenerator);

	void v_generate_with_fivth_limit(CRandom *pcGenerator);

	void v_generate_with_sixth_limit(CRandom *pcGenerator);

	//-----------ZWI¥ZANE Z OBS£UG¥ PLIKU TEKSTOWEGO------------------------------------------------

	FILE *pf_file;

	const std::string S_W_MAKE_NEW_FILE_AND_WRITE = "w";

	const std::string S_R_OPEN_EXISTING_FILE_AND_READ = "r";

	bool b_open_file(const std::string &sFileName, std::string sOperation);

	bool b_close_file();

	//-----------ZWI¥ZANE Z ALGORYTMAMI SZUKAJ¥CYMI NAJLEPSZEGO ROZ.--------------------------------

	CAlgorithm * pc_actual_algorithm;

	int i_number_of_get_quality_calls;
	
public:

	CMscnProblem();

	~CMscnProblem();

	//lista 11:
	double dGetMinimumForGene(int iOffset);
	
	double dGetMaximumForGene(int iOffset);

	bool bSetAlgorithm(CAlgorithm *pcNewAlgorithm);

	CMscnSolution * pcUseAlgorithm();

	//lista 10: zad 2: vGenerateInstance
	bool bGenerateInstance(int iInstanceSeed, int iNewD, int iNewF, int iNewM, int iNewS);

	bool bGenerateInstanceTotallyRandom(unsigned int uiInstanceSeed, int iNewD, int iNewF, int iNewM, int iNewS);

	bool bGenerateInstanceFromConsts(unsigned int uiInstanceSeed, int iNewD, int iNewF, int iNewM, int iNewS);

	//lista 9: Zad 4: dGetQuality

	double dGetQuality(double *pdSolution, int iSolutionArrLength, int &wasAnException);

	double dGetQuality(CMscnSolution *pcSolution, int &wasAnException);

	//lista 9: Zad 5: bConstraintsSatisfied
	bool bConstraintsSatisfied(double *pdSolution, int iSolutionArrLength, int &iTechnicalError, bool *&pbLimitsArr);

	bool bConstraintsSatisfied(CMscnSolution* pcSolution, int &iTechnicalError, bool *&pbLimitsArr);

	//lista 9: zad 6: min / max

	double * dGetMinForElementOfMatrixXd(int iRowNumber, int iCollumnNumber);

	double * dGetMinForElementOfMatrixXf(int iRowNumber, int iCollumnNumber);

	double * dGetMinForElementOfMatrixXm(int iRowNumber, int iCollumnNumber);

	double * dGetMaxForElementOfMatrixXd(int iRowNumber, int iCollumnNumber);

	double * dGetMaxForElementOfMatrixXf(int iRowNumber, int iCollumnNumber);

	double * dGetMaxForElementOfMatrixXm(int iRowNumber, int iCollumnNumber);

	//lista 9: zad 7: zapisywanie do pliku / odczytywanie PROBLEMU----------------------------------

	bool bSaveIntoFile(const std::string &sFileName);

	bool bReadFromFile(const std::string &sFileName);


	//--------------------------------SETTERY LICZB-----------------------------------------------------

	bool bSetD(int iD);

	bool bSetF(int iF);

	bool bSetM(int iM);

	bool bSetS(int iS);

	bool bSetDFMS(int iD, int iF, int iM, int iS);

	//--------------------------------GETTERY LICZB-----------------------------------------------------

	int iGetD();

	int iGetF();

	int iGetM();

	int iGetS();

	//-------------------------SETTERY KOMÓREK W MACIERZACH-----------------------------------------

	bool bCdMatrixSetAt(double dNewVal, int iRowNumber, int iCollumnNumber);

	bool bCfMatrixSetAt(double dNewVal, int iRowNumber, int iCollumnNumber);

	bool bCmMatrixSetAt(double dNewVal, int iRowNumber, int iCollumnNumber);

	bool bXdminmaxMatrixSetAt(double dNewVal, int iRowNumber, int iCollumnNumber);

	bool bXfminmaxMatrixSetAt(double dNewVal, int iRowNumber, int iCollumnNumber);

	bool bXmminmaxMatrixSetAt(double dNewVal, int iRowNumber, int iCollumnNumber);

	//-------------------------SETTERY KOMÓREK W TABLICACH------------------------------------------

	bool bSdArraySetAt(double dNewVal, int iOffset);

	bool bSfArraySetAt(double dNewVal, int iOffset);

	bool bSmArraySetAt(double dNewVal, int iOffset);

	bool bSsArraySetAt(double dNewVal, int iOffset);

	bool bUdArraySetAt(double dNewVal, int iOffset);

	bool bUfArraySetAt(double dNewVal, int iOffset);

	bool bUmArraySetAt(double dNewVal, int iOffset);

	bool bpArraySetAt(double dNewVal, int iOffset);

	//-------------------------OBS£UGA: MACIERZY Z / DO TABLIC--------------------------------------

	bool bInterpreteArray(double *pdProblem, int iProblemArrLength);

	double *pdReturnAsArray();

	int iReturnAsArraysLength(int iD, int iF, int iM, int iS);

	//----------------------------GETTERY MACIERZY I TABLIC-----------------------------------------

	CArray<double> * pcGetArraySd();

	CArray<double> * pcGetArraySf();

	CArray<double> * pcGetArraySm();

	CArray<double> * pcGetArraySs();

	CMatrix<double> * pcGetMatrixCd();

	CMatrix<double> * pcGetMatrixCf();

	CMatrix<double> * pcGetMatrixCm();

	CArray<double> * pcGetArrayUd();

	CArray<double> * pcGetArrayUf();

	CArray<double> * pcGetArrayUm();

	CArray<double> * pcGetArrayP();

	CMatrix<double> * pcGetMatrixXdminmax();

	CMatrix<double> * pcGetMatrixXfminmax();

	CMatrix<double> * pcGetMatrixXmminmax();

	//-------------------------GETTERY I SETTERY ZMIENNYCH-----------------------------------------

	int iGetNumberOfGetQualityCalls();

	void vResetNumberOfGetQualityCalls();
	
	//-------------------------WYPISYWANIE----------------------------------------------------------

	void vPrintEverything();

	void vPrintD();

	void vPrintF();

	void vPrintM();

	void vPrintS();

	void vPrintArraySd();

	void vPrintArraySf();

	void vPrintArraySm();

	void vPrintArraySs();

	void vPrintMatrixCd();

	void vPrintMatrixCf();

	void vPrintMatrixCm();

	void vPrintArrayUd();

	void vPrintArrayUf();

	void vPrintArrayUm();

	void vPrintArrayP();

	void vPrintMatrixXdminmax();

	void vPrintMatrixXfminmax();

	void vPrintMatrixXmminmax();

	void vPrintConstrainsSatisfiedErrorsDetailed(bool *&pbLimitsArr);

	void vPrintConstrainsSatisfiedErrorsSimplified(bool *&pbLimitsArr);

	void vPrintConstrainsSatisfiedErrorsSimplifiedSimplified(bool *&pbLimitsArr);
};