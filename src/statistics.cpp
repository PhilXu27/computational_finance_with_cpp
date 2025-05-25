//
// Created by Phil Xu on 2025/5/22.
//
#include "statistics.h"
#include <vector>
#include <cmath>
#include <cstddef> // for std::size_t
#include <limits>
#include <iostream>
#include <iomanip> // For formatting

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

void computeReturnStats(const double* returns, int length, double& mean, double& stdev, double& sharpeRatio) {
    // Step 1: Compute mean and variance
    double sum = 0.0, sumSq = 0.0;
    int count = 0;

    for (int i = 0; i < length; ++i) {
        if (!std::isnan(returns[i])) {
            sum += returns[i];
            sumSq += returns[i] * returns[i];
            count++;
        }
    }
    double dailyMean = sum / count;
    double dailyVar = (sumSq - count * dailyMean * dailyMean) / (count - 1);

    // Step 2: Annualize (assume 252 trading days)
    mean = dailyMean * 252;
    stdev = std::sqrt(dailyVar * 252);
    sharpeRatio = mean / stdev;
}
