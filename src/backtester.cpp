//
// Created by Phil Xu on 2025/5/22.
//
#include "backtester.h"
#include "statistics.h"
#include "optimizer.h"
#include <iostream>
#include <iomanip> // For formatting

void runRollingBacktest(
    double** returnMatrix, int numAssets, int numPeriods, int inSampleSize, int outSampleSize, double targetReturn,
    double tol, int maxIter
    ) {

    for (int start = 0; start + inSampleSize + outSampleSize <= numPeriods; start += outSampleSize) {
        // Step 0: Allocate sliced in-sample and out-sample windows
        double** inSample = new double*[numAssets];
        double** outSample = new double*[numAssets];

        for (int i = 0; i < numAssets; ++i) {
            inSample[i] = &returnMatrix[i][start];
            outSample[i] = &returnMatrix[i][start + inSampleSize];
        }

        // Step 1: Estimate parameters
        double* inSampleMean = new double[numAssets];
        double** inSampleCov = new double*[numAssets];
        for (int i = 0; i < numAssets; ++i)
            inSampleCov[i] = new double[numAssets];
        // Compute average return for both windows
        computeMeanVector(inSample, numAssets, inSampleSize, inSampleMean);
        computeSampleCovariance(inSample, numAssets, inSampleSize, inSampleCov);
        // Step 2: Solve the Markowitz optimization
        double* optimalWeights = new double[numAssets];

        solveMarkowitzPortfolio(inSampleCov, inSampleMean, targetReturn, numAssets, optimalWeights, tol, maxIter);

        // Step 3: Evaluate OOS performance
        double oosReturn = 0.0;
        for (int t = 0; t < outSampleSize; ++t) {
            double portfolioReturn = 0.0;
            for (int i = 0; i < numAssets; ++i)
                portfolioReturn += optimalWeights[i] * outSample[i][t];
            oosReturn += portfolioReturn;
        }
        oosReturn /= outSampleSize;

//		// --- Sanity Check: Portfolio Constraints ---
//		double weightSum = 0.0;
//		double expectedReturnAchieved = 0.0;
//		for (int i = 0; i < numAssets; ++i) {
//    		weightSum += optimalWeights[i];
//    		expectedReturnAchieved += optimalWeights[i] * inSampleMean[i];
//		}
//		std::cout << "  [Check] Sum of Weights = " << std::fixed << std::setprecision(6) << weightSum
//        		  << ", Expected Return = " << expectedReturnAchieved << std::endl;

        delete[] inSample;
        delete[] outSample;
    }
}