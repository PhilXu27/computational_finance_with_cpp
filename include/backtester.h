//
// Created by Phil Xu on 2025/5/22.
//

#ifndef BACKTESTER_H
#define BACKTESTER_H
void runRollingBacktest(
    double** returnMatrix,
    int numAssets,
    int numPeriods,
    int inSampleSize,
    int outSampleSize,
    double targetReturn,
    double tol,
    int maxIter
);
#endif //BACKTESTER_H
