#pragma once
#include <iostream>
#include "CMatrix.h"
#include "CArray.h"
#include <string>

class CMscnSolution
{
private:
	//-------------------------ILOŒCI PODMIOTÓW ROZWI¥ZANIA-------------------------

	int i_sol_d;

	int i_sol_f;

	int i_sol_m;

	int i_sol_s;

	//-------------------------MACIERZE ROZWI¥ZANIA---------------------------------

	CMatrix<double>*pc_matrix_xd;

	CMatrix<double>*pc_matrix_xf;

	CMatrix<double>*pc_matrix_xm;

	//-----------ZWI¥ZANE Z OBS£UG¥ PLIKU TEKSTOWEGO------------------------------

	FILE *pf_file;

	const std::string S_W_MAKE_NEW_FILE_AND_WRITE = "w";

	const std::string S_R_OPEN_EXISTING_FILE_AND_READ = "r";

	bool b_open_file(const std::string &sFileName, std::string sOperation);

	bool b_close_file();

	//lista 10: zad 2:
	bool b_are_problems_parameters_ok(int iD, int iF, int iM, int iS, CMatrix<double>*pcXdminMax, CMatrix<double>*pcXfminMax, CMatrix<double>*pcXmminMax);

	//lista 11: u³atwienie
	double d_last_calculated_quality;
	
public:

	//lista 11
	bool bIsDifferentFrom(CMscnSolution *pcOtherSol);
	
	//lista 10: zad 2: vGenerateInstance
	bool bGenerateInstance(CRandom *pcGenerator, int iD, int iF, int iM, int iS, CMatrix<double>*pcXdminMax, CMatrix<double>*pcXfminMax, CMatrix<double>*pcXmminMax);

	//zad 7: zapisywanie do pliku / odczytywanie ROZWI¥ZANIA---------------------

	bool bSaveIntoFile(const std::string &sFileName);

	bool bReadFromFile(const std::string &sFileName);


	//-------------------------KONSTRUKTOR / DESTRUKTOR ROZWI¥ZANIA-----------------

	CMscnSolution();

	~CMscnSolution();

	//-------------------------SETTERY ROZWI¥ZANIA----------------------------------

	bool bSetD(int iD);

	bool bSetF(int iF);

	bool bSetM(int iM);

	bool bSetS(int iS);

	bool bSetDFMS(int iD, int iF, int iM, int iS);
	
	//-------------------------GETTERY ROZWI¥ZANIA----------------------------------

	int iGetD();

	int iGetF();

	int iGetM();

	int iGetS();

	//-------------------------SETTERY DO MACIERZY ROZWI¥ZANIA----------------------

	bool bPutNewValInMatrixXdAt(double dNewVal, int iRow, int iCollumn);

	bool bPutNewValInMatrixXfAt(double dNewVal, int iRow, int iCollumn);

	bool bPutNewValInMatrixXmAt(double dNewVal, int iRow, int iCollumn);

	//-------------------------GETTERY DO MACIERZY ROZWI¥ZANIA----------------------

	CMatrix<double> *pcGetMatrixXd();

	CMatrix<double> *pcGetMatrixXf();

	CMatrix<double> *pcGetMatrixXm();

	//-------------------------GETTERY DO D, F, M, S----------------------------

	double dGetSolD();

	double dGetSolF();

	double dGetSolM();

	double dGetSolS();

	//-------------------------INNE GETTERY I SETTERY---------------------------------

	double dGetLastCalculatedQuality();

	void vSetLastCalculatedQuality(double dNewLastCalcualtedQuality);
	
	//-------------------------OBS£UGA: MACIERZY Z / DO TABLIC----------------------

	double *pdRetunAsArray();

	int iReturnAsArraysLength();

	bool bInterpreteArray(double *pdSolution, int iSolutionArrLength);

	//-------------------------PRINTOWANIE ROZWI¥ZANIA-------------------------

	void vPrintSolution();

	void vPrintSolD();
	void vPrintSolF();
	void vPrintSolM();
	void vPrintSolS();

	void vPrintMatrixXd();
	void vPrintMatrixXf();
	void vPrintMatrixXm();
};