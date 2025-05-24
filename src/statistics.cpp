//
// Created by Phil Xu on 2025/5/22.
//
#include "statistics.h"
#include <vector>
#include <cmath>

void computeMeanVector(double** data, int numAssets, int numReturns, double* meanVectorOut) {
    for (int i = 0; i < numAssets; ++i) {
        double sum = 0.0;
        for (int j = 0; j < numReturns; ++j) {
            sum += data[i][j];
        }
        meanVectorOut[i] = sum / numReturns;
    }
}


void computeSampleCovariance(double** data, int numAssets, int numReturns, double** covarianceMatrix) {
    // Step 1: Compute sample means for each asset
    std::vector<double> meanReturns(numAssets, 0.0);
    for (int i = 0; i < numAssets; ++i) {
        for (int k = 0; k < numReturns; ++k) {
            meanReturns[i] += data[i][k];
        }
        meanReturns[i] /= numReturns;
    }

    // Step 2: Compute covariance matrix
    for (int i = 0; i < numAssets; ++i) {
        for (int j = 0; j < numAssets; ++j) {
            double cov = 0.0;
            for (int k = 0; k < numReturns; ++k) {
                cov += (data[i][k] - meanReturns[i]) * (data[j][k] - meanReturns[j]);
            }
            covarianceMatrix[i][j] = cov / (numReturns - 1);
        }
    }
}