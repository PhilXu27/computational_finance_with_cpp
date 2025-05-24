#include <iostream>
#include "read_data.h"
#include "backtester.h"

int main() {
    const int numAssets = 83;
    const int numPeriods = 700;
    const int inSampleSize = 100;
    const int outSampleSize = 12;
    const int runPeriods = 2;
    const double targetReturn = 0.1;
    const int maxIter = 100;
    const double tol = 1e-6;
    const std::string filePath = "data/asset_returns.csv";

    double** returnMatrix = loadReturns(filePath, numAssets, numPeriods);
    if (!returnMatrix) {
        std::cerr << "Failed to load return data." << std::endl;
        return 1;
    }

    runRollingBacktest(
        returnMatrix, numAssets, runPeriods * outSampleSize + outSampleSize + inSampleSize, inSampleSize, outSampleSize,
        targetReturn, tol, maxIter
        );

    for (int i = 0; i < numAssets; ++i) {
        delete[] returnMatrix[i];
    }
    delete[] returnMatrix;

    return 0;
}
