#include <iostream>
#include <vector>
#include "read_data.h"
#include "backtester.h"

int main() {
    const int numAssets = 83;
    const int numPeriods = 700;
    const int inSampleSize = 100;
    const int outSampleSize = 12;
    const int maxIter = 100;
    const double tol = 1e-6;
    const std::string filePath = "data/asset_returns.csv";

    double **returnMatrix = loadReturns(filePath, numAssets, numPeriods);
    if (!returnMatrix) {
        std::cerr << "Failed to load return data." << std::endl;
        return 1;
    }
    std::vector<double> totalReturns;
    for (int i = 1; i < 21; ++i) {
        totalReturns.push_back(0.0 + i * 0.005);
    }
    runMultipleRollingBacktest(
        returnMatrix, numAssets, numPeriods, inSampleSize, outSampleSize,
        totalReturns, tol, maxIter
    );

    for (int i = 0; i < numAssets; ++i) {
        delete[] returnMatrix[i];
    }
    delete[] returnMatrix;

    return 0;
}
