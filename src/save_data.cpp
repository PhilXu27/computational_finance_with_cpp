//
// Created by Phil Xu on 2025/5/25.
//

#include "save_data.h"
#include <fstream>
#include <iomanip>
#include <cmath>  // for std::isnan
#include <string>

void saveReturnsToCSV(const char* filename, const double* returns, int length) {
    std::ofstream file(filename);

    file << "Index,Return\n";  // optional header

    for (int t = 0; t < length; ++t) {
        if (!std::isnan(returns[t])) {
            file << t << "," << std::fixed << std::setprecision(8) << returns[t] << "\n";
        }
    }

    file.close();
}

void saveWeightsAndReturnsByTime(
    int tIndex,
    const double* weights,        // size: numAssets
    double** outSample,           // shape: [numAssets][outSampleSize]
    int numAssets,
    int outSampleOffset           // which time step of outSample to record
) {
    std::string filename = "debug_weights_and_returns_t" + std::to_string(tIndex) + ".csv";
    std::ofstream file(filename);
    if (!file) return;  // silently ignore if fail to open

    file << ",";  // top-left blank
    for (int i = 0; i < numAssets; ++i) {
        file << i;
        if (i != numAssets - 1) file << ",";
    }
    file << "\n";

    file << "Weight,";
    for (int i = 0; i < numAssets; ++i) {
        file << std::fixed << std::setprecision(8) << weights[i];
        if (i != numAssets - 1) file << ",";
    }
    file << "\n";

    file << "Return,";
    for (int i = 0; i < numAssets; ++i) {
        file << std::fixed << std::setprecision(8) << outSample[i][outSampleOffset];
        if (i != numAssets - 1) file << ",";
    }
    file << "\n";

    file.close();
}