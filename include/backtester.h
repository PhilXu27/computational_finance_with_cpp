#ifndef BACKTESTER_H
#define BACKTESTER_H

#include <string>
void runRollingBacktest(
    double** returnMatrix,
    int numAssets,
    int numPeriods,
    int inSampleSize,
    int outSampleSize,
    double targetReturn,
    double tol,
    int maxIter,
    const std::string& fileSuffix,
    double& meanOut,
    double& stdevOut,
    double& meanAnnualizedOut,
    double& stdevAnnualizedOut,
    double& sharpeOut
);

void runMultipleRollingBacktest(
    double** returnMatrix,
    int numAssets,
    int numPeriods,
    int inSampleSize,
    int outSampleSize,
    const std::vector<double>& targetReturns,
    double tol,
    int maxIter
);
#endif //BACKTESTER_H
