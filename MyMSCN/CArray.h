#pragma once
#include <iostream>
#include "CRandom.h"

//-------------------Moja w³asna implementacja zwyk³ej tablicy------------------

template <typename T> class CArray
{
private:

	int i_size;

	T** pt_arr;

public:
	CArray();

	CArray(int iSize);

	~CArray();

	bool bAlloc(int iSize);

	void vDealloc();

	bool bResize(int iNewSize);

	T* tGet(int iOffset);

	bool bSetAt(T* ptNewVal, int iPosition);

	void vPrintArr();

	//lista 10:
	bool bRandomizeDouble(CRandom *pcGenerator, double dMin, double dMax);

	bool bRandomizeInt(CRandom *pcGenerator, int iMin, int iMax);

	bool bContainsNulls();

	double dSumOfThatArray();

	//lista 11:
	bool bIsDifferentFrom(CArray *pcOther);
};

//-------------------konstruktory / destruktor----------------------------------

template<typename T>
inline CArray<T>::CArray()
{
	pt_arr = NULL;
	i_size = -1;
}

template<typename T>
inline CArray<T>::CArray(int iSize)
{
	pt_arr = NULL;
	i_size = -1;
	bAlloc(iSize);
}

template<typename T>
inline CArray<T>::~CArray()
{
	vDealloc();
}

//-------------------obs³uga tablicy - alokacje / dealokacje--------------------

template<typename T>
inline bool CArray<T>::bAlloc(int iSize)
{
	if (iSize <= 0)
	{
		return false;
	}
	else
	{
		vDealloc();

		pt_arr = new T*[iSize];
		for (int ii = 0; ii < iSize; ii++)
		{
			pt_arr[ii] = NULL;
		}
		i_size = iSize;
		return true;
	}
}

template<typename T>
void CArray<T>::vDealloc()
{
	if (pt_arr != NULL)
	{
		for (int ii = 0; ii < i_size; ii++)
		{
			delete pt_arr[ii];
		}
		delete[] pt_arr;
		pt_arr = NULL;
	}
	i_size = 0;
}

//-------------------obs³uga tablicy - zmiana rozmiaru--------------------------

template<typename T>
inline bool CArray<T>::bResize(int iNewSize)
{
	if (iNewSize <= 0 || iNewSize == i_size)
	{
		return false;
	}
	else
	{
		T**pt_new_arr = new T*[iNewSize];

		for (int ii = 0; ii < iNewSize || ii < i_size; ii++)
		{
			if (ii < iNewSize && ii < i_size)
			{
				pt_new_arr[ii] = pt_arr[ii];
			}
			else
			{
				if (iNewSize > i_size)
				{
					pt_new_arr[ii] = NULL;
				}
				else
				{
					delete pt_arr[ii];
				}
			}
		}
		pt_arr = pt_new_arr;
		i_size = iNewSize;
		return true;
	}
}

//-------------------wpisywanie do tablicy / odczytywanie z---------------------

template<typename T>
T* CArray<T>::tGet(int iOffset)
{
	if (iOffset >= i_size)
	{
		return NULL;
	}
	else
	{
		return pt_arr[iOffset];
	}
}

template<typename T>
bool CArray<T>::bSetAt(T* ptNewVal, int iPosition)
{
	if (iPosition < 0 || iPosition >= i_size)
	{
		return false;
	}
	else
	{
		if (pt_arr[iPosition] != NULL)
		{
			delete pt_arr[iPosition];
		}

		pt_arr[iPosition] = ptNewVal;

	}

	return false;
}

//-------------------wypisywanie tablicy: dla nieznanego T oraz double----------

template<typename T>
inline void CArray<T>::vPrintArr()
{
	std::cout << "\nT Array " << i_size << " elem. [ ";
	if (pt_arr != NULL)
	{
		for (int ii = 0; ii < i_size; ii++)
		{
			if ((ii + 1) % i_size != 0)
			{
				if (pt_arr[ii] != NULL)
				{
					std::cout << pt_arr[ii] << ", ";
				}
				else
				{
					std::cout << "NULL, ";
				}
			}
			else
			{
				if (pt_arr[ii] != NULL)
				{
					std::cout << pt_arr[ii] << " ";
				}
				else
				{
					std::cout << "NULL ";
				}
			}
		}
	}
	std::cout << "]\n\n";
}

template<>
inline void CArray<double>::vPrintArr()
{
	std::cout << "\nArray " << i_size << " elem. [ ";
	if (pt_arr != NULL)
	{
		for (int ii = 0; ii < i_size; ii++)
		{
			if ((ii + 1) % i_size != 0)
			{
				if (pt_arr[ii] != NULL)
				{
					std::cout << *(pt_arr[ii]) << ", ";
				}
				else
				{
					std::cout << "NULL, ";
				}
			}
			else
			{
				if (pt_arr[ii] != NULL)
				{
					std::cout << *(pt_arr[ii]) << " ";
				}
				else
				{
					std::cout << "NULL ";
				}
			}
		}
	}
	std::cout << "]\n\n";
}

//----------------------------------lista 10--------------------------------------------------------

template<>
inline bool CArray<double>::bRandomizeDouble(CRandom * pcGenerator, double dMin, double dMax)
{
	if (pt_arr == NULL || pcGenerator == NULL)
	{
		return false;
	}
	else
	{
		for (int ii = 0; ii < i_size; ii++)
		{
			bSetAt(new double(pcGenerator->dNextDouble(dMin, dMax)), ii);
		}
		return true;
	}
}

template<>
inline bool CArray<double>::bRandomizeInt(CRandom * pcGenerator, int iMin, int iMax)
{
	if (pt_arr == NULL || pcGenerator == NULL)
	{
		return false;
	}
	else
	{
		for (int ii = 0; ii < i_size; ii++)
		{
			bSetAt(new double(pcGenerator->iNextInt(iMin, iMax)), ii);
		}
		return true;
	}
}

template<typename T>
inline bool CArray<T>::bContainsNulls()
{
	if (pt_arr == NULL)
	{
		//jeœli nie jest zainicjalizowana, to uznaje, ¿e zawiera null'a / null'e
		return true;
	}
	else
	{
		for (int ii = 0; ii < i_size; ii++)
		{
			if (pt_arr[ii] == NULL)
			{
				return true;
			}
		}
		return false;
	}
}

template <>
inline double CArray<double>::dSumOfThatArray()
{
	double sum = 0;

	for (int ii = 0; ii < i_size; ii++)
	{
		sum += *(pt_arr[ii]);
	}
	return sum;
}

//----------------------------------lista 11--------------------------------------------------------

template<>
inline bool CArray<double>::bIsDifferentFrom(CArray *pcOther)
{
	if(pcOther == NULL || i_size != pcOther->i_size)
	{
		return true;
	}
	else
	{
		for(int ii = 0 ; ii< i_size;ii++)
		{
			if(*(pt_arr[ii]) != *(pcOther->pt_arr[ii]))
			{
				return true;
			}
		}
		return false;
	}
}