#include "save_data.h"
#include <fstream>
#include <iomanip>
#include <cmath>  // for std::isnan
#include <string>
#include <vector>

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
    const double* weights,
    double** outSample,
    int numAssets,
    int outSampleOffset
) {
    std::string filename =
      "results/debug_weights_and_returns_t" + std::to_string(tIndex) + ".csv";
    std::ofstream file(filename);
    if (!file) return;

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
#include <fstream>
#include <iomanip>
#include <vector>

void saveSummaryToCSV(
    const char* filename,
    const std::vector<double>& targetReturns,
    const std::vector<double>& means,
    const std::vector<double>& stdevs,
    const std::vector<double>& meanAnnualized,
    const std::vector<double>& stdevAnnualized,
    const std::vector<double>& sharpes
) {
    std::ofstream file(filename);

    file << "TargetReturn,Mean,Stdev,AnnualizedMean,AnnualizedStdev,Sharpe\n";
    file << std::fixed;

    for (std::size_t i = 0; i < targetReturns.size(); ++i) {
        file << std::setprecision(3) << targetReturns[i] << ","
             << std::setprecision(4) << means[i] << ","
             << std::setprecision(4) << stdevs[i] << ","
             << std::setprecision(4) << meanAnnualized[i] << ","
             << std::setprecision(4) << stdevAnnualized[i] << ","
             << std::setprecision(2) << sharpes[i] << "\n";
    }

    file.close();
}

void saveDebugOptimizationResult(
    const char* filename,
    const double* weights,
    const double* mean,
    int numAssets,
    double achievedReturn,
    double sumWeights,
    double targetReturn
) {
    std::ofstream fout(filename);
    fout << std::fixed << std::setprecision(6);
    fout << "Weight,Mean\n";
    for (int i = 0; i < numAssets; ++i) {
        fout << weights[i] << "," << mean[i] << "\n";
    }
    fout << "\nTarget Return," << targetReturn << "\n";
    fout << "Achieved Return," << achievedReturn << "\n";
    fout << "Sum of Weights," << sumWeights << "\n";
    fout.close();
}