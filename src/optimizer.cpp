#include "optimizer.h"
#include <iostream>
#include <vector>
#include <cstring> // for memset
#include <iomanip> // For formatting

void solveMarkowitzPortfolio(
    double** covariance,
    const double* mean,
    double targetReturn,
    int numAssets,
    double* weightsOut,
    double tol,
    int maxIter
) {
    int dim = numAssets + 2; // N + λ + μ

    // Allocate Q matrix
    double** Q = new double*[dim];
    for (int i = 0; i < dim; ++i)
        Q[i] = new double[dim];

    // Initialize to zero
    for (int i = 0; i < dim; ++i)
        std::memset(Q[i], 0, dim * sizeof(double));

    // Fill top-left: Σ
    for (int i = 0; i < numAssets; ++i)
        for (int j = 0; j < numAssets; ++j)
            Q[i][j] = covariance[i][j];

    // Fill top-right: -mean and -e
    for (int i = 0; i < numAssets; ++i) {
        Q[i][numAssets]     = -mean[i];  // −r̄
        Q[i][numAssets + 1] = -1.0;      // −e
    }

    // Fill bottom rows (symmetric entries)
    for (int j = 0; j < numAssets; ++j) {
        Q[numAssets][j]     = -mean[j];  // −r̄ᵀ
        Q[numAssets + 1][j] = -1.0;      // −eᵀ
    }

    // Construct RHS vector b
    double* b = new double[dim];
    for (int i = 0; i < numAssets; ++i)
        b[i] = 0.0;
    b[numAssets]     = -targetReturn;
    b[numAssets + 1] = -1.0;
    // Solution vector: x = [w; λ; μ]
	double* x = new double[dim];
	for (int i = 0; i < numAssets; ++i)
    	x[i] = 1.0 / numAssets;  // Equal weight initialization

	x[numAssets]     = 0.0;  // λ
	x[numAssets + 1] = 0.0;  // μ

    // Solve Qx = b using CG
    conjugateGradient(Q, b, x, dim, tol, maxIter);

    // Extract weights
    for (int i = 0; i < numAssets; ++i)
        weightsOut[i] = x[i];

    // Cleanup
    for (int i = 0; i < dim; ++i)
        delete[] Q[i];
    delete[] Q;
    delete[] b;
    delete[] x;
}

void conjugateGradient(
    double** Q,
    double* b,
    double* x,
    int n,
    double tol,
    int maxIter
) {
    // Allocate necessary vectors
    double* r = new double[n];  // residual
    double* p = new double[n];  // direction
    double* Qp = new double[n]; // Qp product

    // r_0 = b - Qx_0
    for (int i = 0; i < n; ++i) {
        double Qx_i = 0.0;
        for (int j = 0; j < n; ++j) {
            Qx_i += Q[i][j] * x[j];
        }
        r[i] = b[i] - Qx_i;
        p[i] = r[i];
    }

    double rs_old = 0.0;
    for (int i = 0; i < n; ++i)
        rs_old += r[i] * r[i];

    for (int iter = 0; iter < maxIter; ++iter) {
        // Qp = Q * p
        for (int i = 0; i < n; ++i) {
            Qp[i] = 0.0;
            for (int j = 0; j < n; ++j)
                Qp[i] += Q[i][j] * p[j];
        }

        // α_k = (r.Tr) / (p.TQp)
        double alpha_num = rs_old;
        double alpha_den = 0.0;

        for (int i = 0; i < n; ++i)
            alpha_den += p[i] * Qp[i];
        double alpha = alpha_num / alpha_den;

        // x_{k+1} = x_k + α p_k
        for (int i = 0; i < n; ++i)
            x[i] += alpha * p[i];

        // r_{k+1} = r_k - α Qp_k
        for (int i = 0; i < n; ++i)
            r[i] -= alpha * Qp[i];

        // Check convergence: ||r||² < tol
        double rs_new = 0.0;
        for (int i = 0; i < n; ++i)
            rs_new += r[i] * r[i];
        if (rs_new < tol)
            break;

        // β_k = (rᵗr)_{k+1} / (rᵗr)_k
        double beta = rs_new / rs_old;

        // p_{k+1} = r_{k+1} + β p_k
        for (int i = 0; i < n; ++i)
            p[i] = r[i] + beta * p[i];

        rs_old = rs_new;
    }

    // Cleanup
    delete[] r;
    delete[] p;
    delete[] Qp;
}
