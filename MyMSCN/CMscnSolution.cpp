#include "CMscnSolution.h"

//-------------------------KONSTRUKTOR / DESTRUKTOR ROZWI¥ZANIA-------------------------

CMscnSolution::CMscnSolution()
{
	i_sol_d = 0;
	i_sol_f = 0;
	i_sol_m = 0;
	i_sol_s = 0;
	
	pc_matrix_xd = new CMatrix<double>(i_sol_d, i_sol_f);
	pc_matrix_xf = new CMatrix<double>(i_sol_f, i_sol_m);
	pc_matrix_xm = new CMatrix<double>(i_sol_m, i_sol_s);

	d_last_calculated_quality = -1;
}

CMscnSolution::~CMscnSolution()
{
	delete pc_matrix_xd;
	delete pc_matrix_xf;
	delete pc_matrix_xm;
}

//-------------------------SETTERY ROZWI¥ZANIA-------------------------

bool CMscnSolution::bSetD(int iD)
{
	if (iD > 0)
	{
		i_sol_d = iD;
		pc_matrix_xd->bResizeMatrix(i_sol_d, i_sol_f);
		return true;
	}
	else
	{
		return false;
	}
}

bool CMscnSolution::bSetF(int iF)
{
	if (iF > 0)
	{
		i_sol_f = iF;
		pc_matrix_xd->bResizeMatrix(i_sol_d, i_sol_f);
		pc_matrix_xf->bResizeMatrix(i_sol_f, i_sol_m);
		return true;
	}
	else
	{
		return false;
	}
}

bool CMscnSolution::bSetM(int iM)
{
	if (iM > 0)
	{
		i_sol_m = iM;
		pc_matrix_xf->bResizeMatrix(i_sol_f, i_sol_m);
		pc_matrix_xm->bResizeMatrix(i_sol_m, i_sol_s);
		return true;
	}
	else
	{
		return false;
	}
}

bool CMscnSolution::bSetS(int iS)
{
	if (iS > 0)
	{
		i_sol_s = iS;
		pc_matrix_xm->bResizeMatrix(i_sol_m, i_sol_s);
		return true;
	}
	else
	{
		return false;
	}
}

bool CMscnSolution::bSetDFMS(int iD, int iF, int iM, int iS)
{
	if(bSetD(iD) && bSetF(iF) && bSetM(iM) && bSetS(iS))
	{
		return true;
	}
	else
	{
		i_sol_d = 0;
		i_sol_f = 0;
		i_sol_m = 0;
		i_sol_s = 0;

		return false;
	}
}

//-------------------------GETTERY ROZWI¥ZANIA----------------------------------

int CMscnSolution::iGetD()
{
	return  i_sol_d;
}

int CMscnSolution::iGetF()
{
	return  i_sol_f;
}

int CMscnSolution::iGetM()
{
	return  i_sol_m;
}

int CMscnSolution::iGetS()
{
	return  i_sol_s;
}

//-------------------------SETTERY DO MACIERZY ROZWI¥ZANIA-------------------------

bool CMscnSolution::bPutNewValInMatrixXdAt(double dNewVal, int iRow, int iCollumn)
{
	double* piNewVal = new double(dNewVal);
	return pc_matrix_xd->bSetAtRowNumberCollumnNumber(piNewVal, iRow, iCollumn);
}

bool CMscnSolution::bPutNewValInMatrixXfAt(double dNewVal, int iRow, int iCollumn)
{
	double* piNewVal = new double(dNewVal);
	return pc_matrix_xf->bSetAtRowNumberCollumnNumber(piNewVal, iRow, iCollumn);
}

bool CMscnSolution::bPutNewValInMatrixXmAt(double dNewVal, int iRow, int iCollumn)
{
	double* piNewVal = new double(dNewVal);
	return pc_matrix_xm->bSetAtRowNumberCollumnNumber(piNewVal, iRow, iCollumn);
}

//-------------------------GETTERY DO MACIERZY ROZWI¥ZANIA----------------------

CMatrix<double>* CMscnSolution::pcGetMatrixXd()
{
	return pc_matrix_xd;
}

CMatrix<double>* CMscnSolution::pcGetMatrixXf()
{
	return pc_matrix_xf;
}

CMatrix<double>* CMscnSolution::pcGetMatrixXm()
{
	return pc_matrix_xm;
}

//-------------------------GETTERY DO D, F, M, S----------------------------

double CMscnSolution::dGetSolD()
{
	return i_sol_d;
}

double CMscnSolution::dGetSolF()
{
	return i_sol_f;
}

double CMscnSolution::dGetSolM()
{
	return i_sol_m;
}

double CMscnSolution::dGetSolS()
{
	return i_sol_s;
}
//-------------------------INNE GETTERY I SETTERY---------------------------------

double CMscnSolution::dGetLastCalculatedQuality()
{
	return d_last_calculated_quality;
}

void CMscnSolution::vSetLastCalculatedQuality(double dNewLastCalcualtedQuality)
{
	d_last_calculated_quality = dNewLastCalcualtedQuality;
}

//-------------------------OBS£UGA: MACIERZY Z / DO TABLIC----------------------

double* CMscnSolution::pdRetunAsArray()
{
	double* pd_sol_arr = new double[iReturnAsArraysLength()];

	pd_sol_arr[0] = i_sol_d;
	pd_sol_arr[1] = i_sol_f;
	pd_sol_arr[2] = i_sol_m;
	pd_sol_arr[3] = i_sol_s;

	for (int ii = 0; ii < i_sol_d * i_sol_f; ii++)
	{
		pd_sol_arr[ii + 4] = (*(pc_matrix_xd->tGetFromNumber(ii)));
	}

	for (int ii = 0; ii < i_sol_f * i_sol_m; ii++)
	{
		pd_sol_arr[ii + 4 + i_sol_d * i_sol_f] = (*(pc_matrix_xf->tGetFromNumber(ii)));
	}

	for (int ii = 0; ii < i_sol_m * i_sol_s; ii++)
	{
		pd_sol_arr[ii + 4 + i_sol_d * i_sol_f + i_sol_f * i_sol_m] = (*(pc_matrix_xm->tGetFromNumber(ii)));
	}

	return pd_sol_arr;
}

int CMscnSolution::iReturnAsArraysLength()
{
	return 4 + pc_matrix_xd->iReturnNumberOfElements() + pc_matrix_xf->iReturnNumberOfElements() + pc_matrix_xm->
		iReturnNumberOfElements();
}

bool CMscnSolution::bInterpreteArray(double* pdSolution, int iSolutionArrLength)
{
	if (pdSolution == NULL)
	{
		return false;
	}
	else if (iSolutionArrLength < 4)
	{
		return false;
	}
	else
	{
		int i_buf_sol_d = pdSolution[0];
		int i_buf_sol_f = pdSolution[1];
		int i_buf_sol_m = pdSolution[2];
		int i_buf_sol_s = pdSolution[3];

		if (iSolutionArrLength != 4 + i_buf_sol_d * i_buf_sol_f + i_buf_sol_f * i_buf_sol_m + i_buf_sol_m * i_buf_sol_s)
		{
			return false;
		}
		else
		{
			bSetD(i_buf_sol_d);
			bSetF(i_buf_sol_f);
			bSetM(i_buf_sol_m);
			bSetS(i_buf_sol_s);

			for (int ii = 0; ii < i_sol_d * i_sol_f; ii++)
			{
				double* pd_new_val = new double((pdSolution[ii + 4]));
				pc_matrix_xd->bSetAtNumber(pd_new_val, ii);
			}
			for (int ii = 0; ii < i_sol_f * i_sol_m; ii++)
			{
				double* pd_new_val = new double((pdSolution[4 + ii + i_sol_d * i_sol_f]));
				pc_matrix_xf->bSetAtNumber(pd_new_val, ii);
			}

			for (int ii = 0; ii < i_sol_m * i_sol_s; ii++)
			{
				double* pd_new_val = new double((pdSolution[ii + 4 + i_sol_d * i_sol_f + i_sol_f * i_sol_m]));
				pc_matrix_xm->bSetAtNumber(pd_new_val, ii);
			}
			return true;
		}
	}
}

//------------------zapisywanie / odczytywanie z pliku--------------------------

bool CMscnSolution::bSaveIntoFile(const std::string &sFileName)
{
	if (b_open_file(sFileName, S_W_MAKE_NEW_FILE_AND_WRITE))
	{
		//d
		fprintf(pf_file, "%s %d\n", "D", i_sol_d);

		//f
		fprintf(pf_file, "%s %d\n", "F", i_sol_f);

		//m
		fprintf(pf_file, "%s %d\n", "M", i_sol_m);

		//s
		fprintf(pf_file, "%s %d\n", "S", i_sol_s);

		//pc_matrix_xd
		fprintf(pf_file, "\n\nxd\n");
		pc_matrix_xd->vPrintfMatrixIntoFile(pf_file);

		//pc_matrix_xf
		fprintf(pf_file, "\nxf\n");
		pc_matrix_xf->vPrintfMatrixIntoFile(pf_file);

		//pc_matrix_xm
		fprintf(pf_file, "\nxm\n");
		pc_matrix_xm->vPrintfMatrixIntoFile(pf_file);

		b_close_file();
		return true;
	}
	else
	{
		return false;
	}
}

bool CMscnSolution::bReadFromFile(const std::string &sFileName)
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

		int i_solution_arr_length = 4 + i_buf_d * i_buf_f + i_buf_f * i_buf_m + i_buf_m * i_buf_s;

		double* pd_problem = new double[i_solution_arr_length];
		pd_problem[0] = i_buf_d;
		pd_problem[1] = i_buf_f;
		pd_problem[2] = i_buf_m;
		pd_problem[3] = i_buf_s;

		int i_counter = 4;

		//pc_matrix_xd
		fscanf(pf_file, "%s", s_buf);
		pc_matrix_xd->vFscanfMatrixFromFile(pf_file, pd_problem, i_counter, i_buf_d, i_buf_f);

		//pc_matrix_xf
		fscanf(pf_file, "%s", s_buf);
		pc_matrix_xf->vFscanfMatrixFromFile(pf_file, pd_problem, i_counter, i_buf_f, i_buf_m);

		//pc_matrix_xm
		fscanf(pf_file, "%s", s_buf);
		pc_matrix_xm->vFscanfMatrixFromFile(pf_file, pd_problem, i_counter, i_buf_m, i_buf_s);

		b_close_file();

		return bInterpreteArray(pd_problem, i_solution_arr_length);
	}
	else
	{
		return false;
	}
}



bool CMscnSolution::b_open_file(const std::string &sFileName, std::string sOperation)
{

	pf_file = fopen(sFileName.c_str(), sOperation.c_str());

	if (pf_file == NULL)
	{
		return false;	//nie uda³o siê otworzyæ
	}
	else
	{
		return true;	//uda³o siê otworzyæ plik / lub stworzyæ ca³kiem nowy plik
	}

}

bool CMscnSolution::b_close_file()
{
	if (pf_file == NULL)
	{
		return false;	//Plik nie zosta³ nigdy otwarty / zosta³ ju¿ zmkniêty
	}
	else
	{
		fclose(pf_file);
		pf_file = NULL;
		return true;
	}
}

//-------------------------PRINTOWANIE ROZWI¥ZANIA-------------------------

void CMscnSolution::vPrintSolution()
{
	std::cout << "Solution\n";
	vPrintSolD();
	vPrintSolF();
	vPrintSolM();
	vPrintSolS();

	vPrintMatrixXd();
	vPrintMatrixXf();
	vPrintMatrixXm();
}

void CMscnSolution::vPrintSolD()
{
	std::cout << "sol. d: " << i_sol_d << "\n";
}

void CMscnSolution::vPrintSolF()
{
	std::cout << "sol. f: " << i_sol_f << "\n";
}

void CMscnSolution::vPrintSolM()
{
	std::cout << "sol. m: " << i_sol_m << "\n";
}

void CMscnSolution::vPrintSolS()
{
	std::cout << "sol. s: " << i_sol_s << "\n\n";
}

void CMscnSolution::vPrintMatrixXd()
{
	std::cout << "sol pc_matrix_xd:";
	pc_matrix_xd->vPrintMatrix();
}

void CMscnSolution::vPrintMatrixXf()
{
	std::cout << "sol pc_matrix_xf:";
	pc_matrix_xf->vPrintMatrix();
}

void CMscnSolution::vPrintMatrixXm()
{
	std::cout << "sol pc_matrix_xm:";
	pc_matrix_xm->vPrintMatrix();
}

//-------------------------Lista 10: Zad 2: vGenerateInstance---------------------------------------

bool CMscnSolution::bGenerateInstance(CRandom * pcGenerator, int iD, int iF, int iM, int iS, CMatrix<double>* pcXdminMax, CMatrix<double>* pcXfminMax, CMatrix<double>* pcXmminMax)
{
	if (b_are_problems_parameters_ok(iD, iF, iM, iS, pcXdminMax, pcXfminMax, pcXmminMax) || pcGenerator == NULL)
	{
		i_sol_d = 0;
		i_sol_f = 0;
		i_sol_m = 0;
		i_sol_s = 0;
		return false;
	}
	else
	{
		pc_matrix_xd->bRandomizeWithMatrixMinMax(pcGenerator, pcXdminMax);
		pc_matrix_xf->bRandomizeWithMatrixMinMax(pcGenerator, pcXfminMax);
		pc_matrix_xm->bRandomizeWithMatrixMinMax(pcGenerator, pcXmminMax);
		return true;
	}
}

bool CMscnSolution::b_are_problems_parameters_ok(int iD, int iF, int iM, int iS, CMatrix<double>*pcXdminMax, CMatrix<double>*pcXfminMax, CMatrix<double>*pcXmminMax)
{
	return !bSetD(iD) || !bSetF(iF) || !bSetM(iM) || !bSetS(iS) ||
		pcXdminMax == NULL || pcXfminMax == NULL || pcXmminMax == NULL;
}

//-------------------------Lista 11-----------------------------------------------------------------

bool CMscnSolution::bIsDifferentFrom(CMscnSolution* pcOtherSol)
{
	if(i_sol_d != pcOtherSol->i_sol_d || 
		i_sol_f != pcOtherSol->i_sol_f ||
		i_sol_m != pcOtherSol->i_sol_m ||
		i_sol_s != pcOtherSol->i_sol_s)
	{
		return true;
	}
	else
	{
		return pc_matrix_xd->bIsDifferentFrom(pcOtherSol->pc_matrix_xd) ||
			pc_matrix_xf->bIsDifferentFrom(pcOtherSol->pc_matrix_xf) ||
			pc_matrix_xm->bIsDifferentFrom(pcOtherSol->pc_matrix_xm);
	}
}