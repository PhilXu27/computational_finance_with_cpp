//
// Created by Phil Xu on 2025/5/22.
//

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

void solveMarkowitzPortfolio(
    double** covariance,      // Σ: N x N
    const double* mean,       // r̄: N
    double targetReturn,      // r̄_P
    int numAssets,            // N
    double* weightsOut,        // Output: optimal weights w (length N)
    double tol,
    int maxIter
);

#endif //OPTIMIZER_H
