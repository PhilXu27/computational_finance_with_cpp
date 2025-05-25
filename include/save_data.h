//
// Created by Phil Xu on 2025/5/25.
//

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
#endif //SAVE_DATA_H
