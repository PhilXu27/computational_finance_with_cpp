//
// Created by Phil Xu on 2025/5/17.
//

#ifndef READ_DATA_H
#define READ_DATA_H

#include <string>

double string_to_double( const std::string& s );
void readData(double **data,std::string fileName);
double** loadReturns(const std::string& fileName, int numAssets, int numPeriods);

#endif //READ_DATA_H
