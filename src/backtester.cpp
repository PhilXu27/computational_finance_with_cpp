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
    std::vector<double> means;
    std::vector<double> stdevs;
    std::vector<double> meanAnnualizeds;
    std::vector<double> stdevAnnualizeds;
    std::vector<double> sharpes;

    for (double targetReturn : targetReturns) {
        std::ostringstream oss;
        oss << "_r" << static_cast<int>(targetReturn * 1000);

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
            oss.str(),
            mean,
            stdev,
            meanAnnualized,
            stdevAnnualized,
            sharpe
        );
        std::cout << std::fixed
                  << "Target Return: " << std::setprecision(3) << targetReturn
                  << ", Mean: " << std::setprecision(4) << mean
                  << ", Stdev: " << std::setprecision(4) << stdev
                  << ", Annualized Mean: " << std::setprecision(4) << meanAnnualized
                  << ", Annualized Stdev: " << std::setprecision(4) << stdevAnnualized
                  << ", Sharpe: " << std::setprecision(2) << sharpe
                  << std::endl;
        means.push_back(mean);
        stdevs.push_back(stdev);
        meanAnnualizeds.push_back(meanAnnualized);
        stdevAnnualizeds.push_back(stdevAnnualized);
        sharpes.push_back(sharpe);
    }
    saveSummaryToCSV(
        "results/summary_results.csv",
        targetReturns,
        means,
        stdevs,
        meanAnnualizeds,
        stdevAnnualizeds,
        sharpes
    );
}

void runRollingBacktest(
    double** returnMatrix, int numAssets, int numPeriods,
    int inSampleSize, int outSampleSize, double targetReturn,
    double tol, int maxIter,
    const std::string& fileSuffix,
    double& meanOut,
    double& stdevOut,
    double& meanAnnualizedOut,
    double& stdevAnnualizedOut,
    double& sharpeOut
    ) {
	double* portfolioReturns = new double[numPeriods];
    for (int t = 0; t < numPeriods; ++t)
        portfolioReturns[t] = NAN;

    for (
        int start = 0;
        start + inSampleSize + outSampleSize <= numPeriods;
        start += outSampleSize) {
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
        solveMarkowitzPortfolio(
            inSampleCov,
            inSampleMean,
            targetReturn,
            numAssets,
            optimalWeights,
            tol,
            maxIter
            );
        // Step 3: Evaluate OOS performance
        // Compute and store OOS portfolio returns (time-aligned)
        for (int t = 0; t < outSampleSize; ++t) {
            int timeIndex = start + inSampleSize + t;
            double portRet = 0.0;
            for (int i = 0; i < numAssets; ++i)
                portRet += optimalWeights[i] * outSample[i][t];
            portfolioReturns[timeIndex] = portRet;
        }

        // Save example weight files for report.
        if (start == 0 && std::abs(targetReturn - 0.05) < 1e-6) {
          double achievedReturn = 0.0, sumWeights = 0.0;
    		for (int i = 0; i < numAssets; ++i) {
        		achievedReturn += optimalWeights[i] * inSampleMean[i];
        		sumWeights += optimalWeights[i];
    		}
    		saveDebugOptimizationResult(
        		"results/debug_weights_and_returns.csv",
        		optimalWeights,
        		inSampleMean,
        		numAssets,
        		achievedReturn,
        		sumWeights,
        		targetReturn
    		);
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
    std::string filename = "results/oos_portfolio_returns" + fileSuffix + ".csv";
    saveReturnsToCSV(filename.c_str(), portfolioReturns, numPeriods);
    computeReturnStats(
        portfolioReturns,
        numPeriods,
        meanOut,
        stdevOut,
        sharpeOut,
        meanAnnualizedOut,
        stdevAnnualizedOut);
	delete[] portfolioReturns;
}