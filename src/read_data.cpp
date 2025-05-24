
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include "csv.h"
#include "read_data.h"

//g++ -c read_data.cpp
// g++ -c csv.cpp
// g++ -o portfolioSolver csv.o read_data.o
// ./portfolioSolver


double string_to_double( const std::string& s )
{
	std::istringstream i(s);
	double x;
	if (!(i >> x))
		return 0;
	return x;
} 


void readData(double **data,string fileName)
{
	char tmp[20];
	ifstream file (strcpy(tmp, fileName.c_str()));
	Csv csv(file);
	string line;
	if (file.is_open())
	{
		int i=0;
		while (csv.getline(line) != 0) {
         	for (int j = 0; j < csv.getnfield(); j++)
            {
               double temp=string_to_double(csv.getfield(j));
               //cout << "Asset " << j << ", Return "<<i<<"="<< temp<<"\n";
               data[j][i]=temp;
            }
            i++;
		}
		
		file.close();
	}
	else {cout <<fileName <<" missing\n";exit(0);}
}
                                                                    

double** loadReturns(const std::string& fileName, int numAssets, int numPeriods){
    double** data = new double*[numAssets];
    for (int i = 0; i < numAssets; ++i)
        data[i] = new double[numPeriods];

    readData(data, fileName);
    return data;
}
