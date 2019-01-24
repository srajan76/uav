#include "scenarios.hpp"

#include <random> 
#include <iostream>

Scenarios::Scenarios() :
    _seed(2019),
    _numTargets(), 
    _numScenarios(2000),
    _maxBatches(10),
    _maxBatchSize(100), 
    _prob(),
    _omega() {};

void Scenarios::generateScenarios() {
    _omega.resize(_numScenarios);
    _prob.resize(_numTargets, 0.5);
    std::mt19937 generator(_seed);

    for (int i=0; i<_numScenarios; ++i) {
        for (int j=0; j<_numTargets; ++j) {
            std::bernoulli_distribution pr(_prob[j]);
            _omega[i].push_back(pr(generator));
        }
    }
    return;
};

std::vector<std::vector<int>> Scenarios::getScenarios(
    int batchId, int scenariosPerBatch) {
    
    if (batchId > _maxBatches) {
        std::cerr << "batch id should be less than or equal to" << _maxBatches << std::endl;
        exit(1);
    }

    if (scenariosPerBatch > _maxBatchSize) {
        std::cerr << "scenarios per batch should be less than or equal to " << _maxBatchSize << std::endl;
        exit(1);
    }

    int from = scenariosPerBatch * (batchId - 1);
    int to = scenariosPerBatch * batchId - 1;
    std::vector<std::vector<int>> reduced(&_omega[from], &_omega[to]);
    
    return reduced;
};