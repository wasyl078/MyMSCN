#pragma once
#include <iostream>
#include "CRandom.h"

//-------------------Moja w³asna implementacja macierzy-------------------------

template <typename T> class CMatrix
{
private:

	T ***pt_matrix;

	int i_columns;

	int i_rows;

public:

	CMatrix();

	CMatrix(int iRows, int iColumns);

	~CMatrix();

	bool bAllockMatrix(int iRows, int iColumns);

	void vDeallocMatrix();

	bool bResizeMatrix(int iNewRows, int iNewColumns);

	bool bSetAtRowNumberCollumnNumber(T *tNew, int iRowNumber, int iColumnNumber);

	bool bSetAtNumber(T *tNew, int Offset);

	T* tGetFromRowNumberCollumnNumber(int iRowNumber, int iColumnNumber);

	T* tGetFromNumber(int iOffset);

	bool bSetMatrixFromArray(T* tArr, int iRowNumber, int iColumnNumber);

	T* ptReturnMatrixAsArray();

	int iReturnNumberOfElements();

	void vPrintMatrix();

	int iGetColumnsNumber();

	int iGetRowsNumber();

	void vPrintfMatrixIntoFile(FILE *pfFile);

	void vFscanfMatrixFromFile(FILE *pfFile, double *pd_problem, int &iCounter, int iBufRowNumber, int iBufColumnNumber);

	//lista 10:
	bool bRandomizeDouble(CRandom *pcGenerator, double dMin, double dMax);

	bool bRandomizeInt(CRandom *pcGenerator, int iMin, int iMax);

	bool bFillWithTwoNumbers(int iFirst, int iSecond);

	bool bRandomizeWithMatrixMinMax(CRandom *pcGenerator, CMatrix<double> *pcMatrixMinMax);

	bool bContainsNulls();

	//lista 11:
	bool bIsDifferentFrom(CMatrix *pcOther);
};

//-------------------konstruktory / destruktor----------------------------------

template<typename T>
CMatrix<T>::CMatrix()
{
	pt_matrix = NULL;
	i_rows = -1;
	i_columns = -1;
}

template<typename T>
CMatrix<T>::CMatrix(int iRows, int iColumns)
{
	pt_matrix = NULL;
	bAllockMatrix(iRows, iColumns);
}

template <typename T>
CMatrix<T>::~CMatrix()
{
	vDeallocMatrix();
}

//-------------------obs³uga macierzy - alokacje / dealokacje-------------------

template<typename T>
bool CMatrix<T>::bAllockMatrix(int iRows, int iColumns)
{
	if (iRows > 0 && iColumns > 0)
	{
		vDeallocMatrix();

		pt_matrix = new T**[iRows];
		for (int ii = 0; ii < iRows; ii++)
		{
			pt_matrix[ii] = new T*[iColumns];
		}
		i_columns = iColumns;
		i_rows = iRows;

		for (int ii = 0; ii < i_rows; ii++)
			for (int jj = 0; jj < i_columns; jj++)
			{
				pt_matrix[ii][jj] = NULL;
			}
		return true;
	}
	else
	{
		pt_matrix = NULL;
		i_columns = -1;
		i_rows = -1;
		return false;
	}
}

template<typename T>
void CMatrix<T>::vDeallocMatrix()
{
	if (pt_matrix != NULL)
	{
		for (int ii = 0; ii < i_rows; ii++)
		{
			for (int jj = 0; jj < i_columns; jj++)
			{
				delete pt_matrix[ii][jj];
			}
			delete pt_matrix[ii];
		}
		delete[] pt_matrix;
		pt_matrix = NULL;
	}
}

//-------------------obs³uga macierzy - zmiana rozmiaru-------------------------

template<typename T>
bool CMatrix<T>::bResizeMatrix(int iNewRows, int iNewColumns)
{
	if (iNewRows > 0 && iNewColumns > 0 && (iNewColumns != i_columns || iNewRows != i_rows))
	{
		T ***pt_new_matrix = new T**[iNewRows];
		for (int ii = 0; ii < iNewRows; ii++)
		{
			pt_new_matrix[ii] = new T*[iNewColumns];
		}

		for (int ii = 0; ii < i_rows || ii < iNewRows; ii++)
		{
			for (int jj = 0; jj < i_columns || jj < iNewColumns; jj++)
			{
				if (ii < i_rows && jj < i_columns && ii < iNewRows && jj < iNewColumns)
				{
					pt_new_matrix[ii][jj] = pt_matrix[ii][jj];
				}
				else
				{
					if (ii < i_rows && jj < i_columns)
					{
						delete pt_matrix[ii][jj];
					}
					else if (ii < iNewRows && jj < iNewColumns)
					{
						pt_new_matrix[ii][jj] = NULL;
					}
				}
			}
		}

		pt_matrix = pt_new_matrix;

		i_columns = iNewColumns;
		i_rows = iNewRows;
		return true;
	}
	else
	{
		return false;
	}
}

//-------------------wpisywanie do macierzy / odczytywanie z--------------------

template<typename T>
bool CMatrix<T>::bSetAtRowNumberCollumnNumber(T *tNew, int iRowNumber, int iColumnNumber)
{
	if (iRowNumber < 0 || iColumnNumber >= i_columns || iColumnNumber < 0 || iRowNumber >= i_rows)
	{
		return false;
	}
	else
	{
		if (pt_matrix[iRowNumber][iColumnNumber] != NULL)
		{
			delete pt_matrix[iRowNumber][iColumnNumber];
		}
		pt_matrix[iRowNumber][iColumnNumber] = tNew;
		return true;
	}
}

template<typename T>
bool CMatrix<T>::bSetAtNumber(T *tNew, int iOffset)
{
	if (pt_matrix != NULL && iOffset < i_columns * i_rows)
	{
		int i_counter = 0;
		for (int ii = 0; ii < i_rows; ii++)
		{
			for (int jj = 0; jj < i_columns; jj++)
			{
				if (i_counter == iOffset)
				{
					if (pt_matrix[ii][jj] != NULL)
					{
						delete pt_matrix[ii][jj];
					}
					pt_matrix[ii][jj] = tNew;
					return true;
				}
				i_counter++;
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}

template<typename T>
T * CMatrix<T>::tGetFromRowNumberCollumnNumber(int iRowNumber, int iColumnNumber)
{
	if (iRowNumber >= 0 && iColumnNumber < i_columns && iColumnNumber >= 0 && iRowNumber < i_rows)
		return pt_matrix[iRowNumber][iColumnNumber];
	return NULL;
}

template<typename T>
inline T * CMatrix<T>::tGetFromNumber(int iOffset)
{
	if (pt_matrix != NULL && iOffset < i_columns * i_rows)
	{
		int i_counter = 0;
		for (int ii = 0; ii < i_rows; ii++)
		{
			for (int jj = 0; jj < i_columns; jj++)
			{
				if (i_counter == iOffset)
				{
					return pt_matrix[ii][jj];
				}
				i_counter++;
			}
		}
		return NULL;
	}
	else
	{
		return NULL;
	}
}

template<typename T>
bool CMatrix<T>::bSetMatrixFromArray(T* tArr, int iRowNumber, int iColumnNumber)
{
	if (bAllockMatrix(iRowNumber, iColumnNumber))
	{
		int ii = 0;
		for (int ix = 0; ix < i_rows; ix++)
		{
			for (int iy = 0; iy < i_columns; iy++)
			{
				pt_matrix[ix][iy] = new T(tArr[ii++]);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

template<typename T>
inline T* CMatrix<T>::ptReturnMatrixAsArray()
{
	T* pt_arr = new T[i_rows*i_columns];

	int i_counter = 0;

	for (int ii = 0; ii < i_rows; ii++)
	{
		for (int jj = 0; jj < i_columns; jj++)
		{
			pt_arr[i_counter] = *pt_matrix[ii][jj];
			i_counter++;
		}
	}
	return pt_arr;
}

template<typename T>
inline int CMatrix<T>::iReturnNumberOfElements()
{
	return i_columns * i_rows;
}

//-------------------wypisywanie macierzy: dla nieznanego T oraz double---------

template<typename T>
inline void CMatrix<T>::vPrintMatrix()
{
	std::cout << "\nT Matrix cols.: " << i_columns << ", rows:" << i_rows << ":\n";
	for (int ii = 0; ii < i_rows; ii++)
	{
		for (int jj = 0; jj < i_columns; jj++)
		{
			if ((jj + 1) % i_columns != 0)
			{
				std::cout << pt_matrix[ii][jj] << " ";
			}
			else
			{
				std::cout << pt_matrix[ii][jj] << "\n";
			}
		}
	}
}

template<typename T>
inline int CMatrix<T>::iGetColumnsNumber()
{
	return i_columns;
}

template<typename T>
inline int CMatrix<T>::iGetRowsNumber()
{
	return i_rows;
}

template<>
inline void CMatrix<double>::vPrintMatrix()
{
	std::cout << "\nMatrix cols. " << i_columns << ", rows: " << i_rows << ":\n";
	for (int ii = 0; ii < i_rows; ii++)
	{
		for (int jj = 0; jj < i_columns; jj++)
		{
			if ((jj + 1) % i_columns != 0)
			{
				if (pt_matrix[ii][jj] != NULL)
				{
					std::cout << *pt_matrix[ii][jj] << " ";
				}
				else {
					std::cout << "NU ";
				}
			}
			else
			{
				if (pt_matrix[ii][jj] != NULL)
				{
					std::cout << *pt_matrix[ii][jj] << "\n";
				}
				else
				{
					std::cout << "NU\n";
				}
			}
		}
	}
	std::cout << "\n";
}

//------------zapisywanie macierzy / odczytywanie z pliku------------------------
template<>
inline void CMatrix<double>::vPrintfMatrixIntoFile(FILE *pfFile)
{
	double *pd_tab = ptReturnMatrixAsArray();

	for (int ii = 0; ii < iReturnNumberOfElements(); ii++)
	{
		if ((ii + 1) % i_columns == 0)
		{
			fprintf(pfFile, "%.2f\n", pd_tab[ii]);
		}
		else
		{
			fprintf(pfFile, "%.2f ", pd_tab[ii]);
		}
	}
	delete[] pd_tab;
}

template<>
inline void CMatrix<double>::vFscanfMatrixFromFile(FILE *pfFile, double *pd_problem, int &iCounter, int iBufRowNumber, int iBufColumnNumber)
{
	float f_buf;
	for (int ii = 0; ii < iBufRowNumber * iBufColumnNumber; ii++)
	{
		fscanf(pfFile, "%f ", &f_buf);
		(pd_problem[iCounter++]) = f_buf;
	}
}

//----------------------------------lista 10--------------------------------------------------------

template<>
inline bool CMatrix<double>::bRandomizeDouble(CRandom* pcGenerator, double dMin, double dMax)
{
	if (pt_matrix == NULL)
	{
		return false;
	}
	else
	{
		for (int ii = 0; ii < i_rows; ii++)
		{
			for (int jj = 0; jj < i_columns; jj++)
			{
				bSetAtRowNumberCollumnNumber(new double(pcGenerator->dNextDouble(dMin, dMax)), ii, jj);
			}
		}
		return true;
	}
}

template<>
inline bool CMatrix<double>::bRandomizeInt(CRandom* pcGenerator, int iMin, int iMax)
{
	if (pt_matrix == NULL)
	{
		return false;
	}
	else
	{
		for (int ii = 0; ii < i_rows; ii++)
		{
			for (int jj = 0; jj < i_columns; jj++)
			{
				bSetAtRowNumberCollumnNumber(new double(pcGenerator->iNextInt(iMin, iMax)), ii, jj);
			}
		}
		return true;
	}
}

template<>
inline bool CMatrix<double>::bFillWithTwoNumbers(int iFirst, int iSecond)
{
	if (pt_matrix == NULL)
	{
		return false;
	}
	else
	{
		for (int ii = 0; ii < iReturnNumberOfElements(); ii++)
		{
			bSetAtNumber(new double(iFirst), ii++);
			bSetAtNumber(new double(iSecond), ii);
		}
		return true;
	}
}

template<>
inline bool CMatrix<double>::bRandomizeWithMatrixMinMax(CRandom * pcGenerator, CMatrix<double> * pcMatrixMinMax)
{
	if (pt_matrix == NULL || pcMatrixMinMax == NULL || pcGenerator == NULL)
	{
		return false;
	}
	else
	{
		for (int ii = 0; ii < i_rows; ii++)
		{
			for (int jj = 0; jj < i_columns; jj++)
			{
				double d_temp_min = *(pcMatrixMinMax->tGetFromRowNumberCollumnNumber(ii, 2 * jj));
				double d_temp_max = *(pcMatrixMinMax->tGetFromRowNumberCollumnNumber(ii, 2 * jj + 1));
				bSetAtRowNumberCollumnNumber(new double(pcGenerator->dNextDouble(d_temp_min, d_temp_max)), ii, jj);
			}
		}
		return true;
	}
}

template<typename T>
inline bool CMatrix<T>::bContainsNulls()
{
	if (pt_matrix == NULL)
	{
		//jeœli nie zainicjowana macierz, to jest uznaje, ¿e zawiera null'a / null'e
		return true;
	}
	else
	{
		for (int ii = 0; ii < i_rows; ii++)
		{
			for (int jj = 0; jj < i_columns; jj++)
			{
				if (pt_matrix[ii][jj] == NULL)
				{
					return true;
				}
			}
		}
		return false;
	}
}

//----------------------------------lista 11--------------------------------------------------------
template <>
inline bool CMatrix<double>::bIsDifferentFrom(CMatrix* pcOther)
{
	if (pcOther == NULL || pcOther->i_columns != i_columns || pcOther->i_rows != i_rows)
	{
		return true;
	}
	else
	{
		for (int ii = 0; ii < i_rows; ii++)
		{
			for (int jj = 0; jj < i_columns; jj++)
			{
				double d_1 = *(pcOther->tGetFromRowNumberCollumnNumber(ii, jj));
				double d_2 = *(tGetFromRowNumberCollumnNumber(ii, jj));
				if (d_1 != d_2)
				{
					return true;
				}
			}
		}
		return false;
	}
}

