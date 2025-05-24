//
// Created by Phil Xu on 2025/5/22.
//

#ifndef STATISTICS_H
#define STATISTICS_H
void computeMeanVector(double** data, int numAssets, int numReturns, double* meanVectorOut);
void computeSampleCovariance(double** data, int numAssets, int numReturns, double** covarianceMatrix);


#endif //STATISTICS_H
