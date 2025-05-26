#include <vector>
#include <string>
#ifndef SAVE_DATA_H
#define SAVE_DATA_H
void saveReturnsToCSV(const char* filename, const double* returns, int length);
void saveWeightsAndReturnsByTime(
    int tIndex,
    const double* weights,        // size: numAssets
    double** outSample,           // shape: [numAssets][outSampleSize]
    int numAssets,
    int outSampleOffset           // which time step of outSample to record
);
void saveSummaryToCSV(
    const char* filename,
    const std::vector<double>& targetReturns,
    const std::vector<double>& means,
    const std::vector<double>& stdevs,
    const std::vector<double>& meanAnnualized,
    const std::vector<double>& stdevAnnualized,
    const std::vector<double>& sharpes
);
void saveDebugOptimizationResult(
    const char* filename,
    const double* weights,
    const double* mean,
    int numAssets,
    double achievedReturn,
    double sumWeights,
    double targetReturn
);
#endif //SAVE_DATA_H
