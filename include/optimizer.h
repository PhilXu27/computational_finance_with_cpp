#ifndef OPTIMIZER_H
#define OPTIMIZER_H
void solveMarkowitzPortfolio(
    double** covariance,
    const double* mean,
    double targetReturn,
    int numAssets,
    double* weightsOut,
    double tol,
    int maxIter
);
void conjugateGradient(double** Q, double* b, double* x, int n, double tol, int maxIter);
#endif //OPTIMIZER_H
