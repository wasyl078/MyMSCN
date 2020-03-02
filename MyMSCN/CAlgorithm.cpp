#include "CAlgorithm.h"

//-------------------------------zapisywanie do pliku .csv--------------------------------------

bool CAlgorithm::bSaveIntoCsvFile(std::string sFileName, int iFirstCoulmn)
{

	if (iFirstCoulmn < 1 || vec_min_qualities.empty())
	{
		return false;
	}
	else
	{
		std::ofstream myfile;
		myfile.open(sFileName.c_str());
		for (int ii = 0; ii < vec_min_qualities.size(); ii++)
		{
			for (int jj = 1; jj < iFirstCoulmn; jj++)
			{
				myfile << ",";
			}
			myfile << vec_min_qualities[ii];
			myfile << ",";
			myfile << vec_max_qualities[ii];
			myfile << ",";
			myfile << vec_average_qualities[ii];
			myfile << "\n";
		}
		myfile.close();
	}
	return true;
}

//-------------------------------wypisywanie statystyk------------------------------------------

void CAlgorithm::vPrintStatistics()
{
	std::cout << "MIMIMUM | MAXIMUM | AVERAGE/ALL\n";

	for(int ii = 0 ;ii<vec_min_qualities.size();ii++)
	{
		std::cout << vec_min_qualities[ii] << " | " << vec_max_qualities[ii] << " | "
		<< vec_average_qualities[ii] << "\n";
	}
	
}

//-------------------------publiczne metody pomocnicze------------------------------------------