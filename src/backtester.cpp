//
// Created by Phil Xu on 2025/5/22.
//
#include "backtester.h"
#include "statistics.h"
#include "optimizer.h"
#include "save_data.h"
#include <iostream>
#include <iomanip> // For formatting

void runMultipleRollingBacktest(){}

void runRollingBacktest(
    double** returnMatrix, int numAssets, int numPeriods, int inSampleSize, int outSampleSize, double targetReturn,
    double tol, int maxIter
    ) {
	double* portfolioReturns = new double[numPeriods];
    for (int t = 0; t < numPeriods; ++t)
        portfolioReturns[t] = NAN;

    for (int start = 0; start + inSampleSize + outSampleSize <= numPeriods; start += outSampleSize) {
        std::cout << "Rolling window starting at t = " << start << std::endl;

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
        // Compute and store OOS portfolio returns (time-aligned)
        for (int t = 0; t < outSampleSize; ++t) {
            int timeIndex = start + inSampleSize + t;
            double portRet = 0.0;
            for (int i = 0; i < numAssets; ++i)
                portRet += optimalWeights[i] * outSample[i][t];
            portfolioReturns[timeIndex] = portRet;
        }
//        int tStep = start + inSampleSize;  // current OOS start index in global time
//        saveWeightsAndReturnsByTime(
//            tStep,
//            optimalWeights,
//            outSample,
//            numAssets,
//            0  // we use the first day of the OOS window for debug (can change to other days)
//        );

        saveReturnsToCSV("oos_portfolio_returns.csv", portfolioReturns, numPeriods);
        // Cleanup
        delete[] inSampleMean;
        delete[] optimalWeights;
        for (int i = 0; i < numAssets; ++i)
            delete[] inSampleCov[i];
        delete[] inSampleCov;
        delete[] inSample;
        delete[] outSample;
    }
    // ======= Summary: Realized OOS Portfolio Statistics =======
    double mean, stdev, sharpe;
    computeReturnStats(portfolioReturns, numPeriods, mean, stdev, sharpe);
    std::cout << "Annualized Mean: " << mean << ", Stdev: " << stdev << ", Sharpe: " << sharpe << std::endl;
	delete[] portfolioReturns;
}