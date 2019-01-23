#ifndef SCENARIOS_HPP
#define SCENARIOS_HPP

#pragma once

#include <vector>

class Scenarios {
    protected: 
        int _numTargets;
        int _numScenarios = 2000;
        int _maxBatches = 10;
        int _maxBatchSize = 100;
        std::vector<std::vector<int>> _omega;

    public:
        Scenarios();
        
        void setNumTargets(int numTargets) { _numTargets = numTargets; };
        int getNumScenarios() const { return _numScenarios; };
        int getMaxBatches() const { return _maxBatches; };
        int getMaxBatchSize() const { return _maxBatchSize; };

        void generateScenarios();
        std::vector<std::vector<int>> getScenarios(int batchId, int scenariosPerBatch);
};

#endif 