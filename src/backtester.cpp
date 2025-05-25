//
// Created by Phil Xu on 2025/5/22.
//
#include "backtester.h"
#include "statistics.h"
#include "optimizer.h"
#include "save_data.h"
#include <iostream>
#include <sstream>
#include <iomanip> // For formatting

void runMultipleRollingBacktest(
    double** returnMatrix,
    int numAssets,
    int numPeriods,
    int inSampleSize,
    int outSampleSize,
    const std::vector<double>& targetReturns,
    double tol,
    int maxIter
) {
    std::cout << "=== Running Multiple Rolling Backtests ===" << std::endl;

    std::cout << "TargetReturn,Mean,Stdev,Sharpe" << std::endl;

    for (double targetReturn : targetReturns) {
        // Format suffix like "_r0050" for target return = 0.005
        std::ostringstream oss;
        oss << "_r" << std::fixed << std::setprecision(4) << targetReturn * 10000;

        double mean, stdev, meanAnnualized, stdevAnnualized, sharpe;
        runRollingBacktest(
            returnMatrix,
            numAssets,
            numPeriods,
            inSampleSize,
            outSampleSize,
            targetReturn,
            tol,
            maxIter,
            oss.str(),   // fileSuffix
            mean,
            stdev,
            meanAnnualized,
            stdevAnnualized,
            sharpe
        );
        std::cout << "Target Return: " << targetReturn <<
            ", Mean: " << mean <<
            ", Stdev: " << stdev <<
            ", Annualized Mean: " << meanAnnualized <<
            ", Annualized Stdev: " << stdevAnnualized <<
            ", Sharpe: " << sharpe <<
            std::endl;
    }
}

void runRollingBacktest(
    double** returnMatrix, int numAssets, int numPeriods, int inSampleSize, int outSampleSize, double targetReturn,
    double tol, int maxIter,
    const std::string& fileSuffix,  // e.g. "_r005" or "_r010"
    double& meanOut,
    double& stdevOut,
    double& meanAnnualizedOut,
    double& stdevAnnualizedOut,
    double& sharpeOut
    ) {
	double* portfolioReturns = new double[numPeriods];
    for (int t = 0; t < numPeriods; ++t)
        portfolioReturns[t] = NAN;

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
        // Compute and store OOS portfolio returns (time-aligned)
        for (int t = 0; t < outSampleSize; ++t) {
            int timeIndex = start + inSampleSize + t;
            double portRet = 0.0;
            for (int i = 0; i < numAssets; ++i)
                portRet += optimalWeights[i] * outSample[i][t];
            portfolioReturns[timeIndex] = portRet;
        }


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
    std::string filename = "oos_portfolio_returns" + fileSuffix + ".csv";
    saveReturnsToCSV(filename.c_str(), portfolioReturns, numPeriods);
    computeReturnStats(portfolioReturns, numPeriods, meanOut, stdevOut, sharpeOut, meanAnnualizedOut, stdevAnnualizedOut);
	delete[] portfolioReturns;
}