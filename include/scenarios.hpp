#ifndef SCENARIOS_HPP
#define SCENARIOS_HPP

#pragma once

#include <vector>

class Scenarios {
    protected: 
        int _seed = 2019;
        int _numTargets;
        int _numScenarios = 2000;
        int _maxScenarios = 1000;
        std::vector<float> _prob;
        std::vector<std::vector<int>> _omega;

    public:
        Scenarios();
        
        void setSeed(int seed) { _seed = seed; };
        void setNumTargets(int numTargets) { _numTargets = numTargets; };
        int getNumScenarios() const { return _numScenarios; };
        int getMaxScenarios() const { return _maxScenarios; };

        void generateScenarios();
        std::vector<std::vector<int>> getScenarios(
            int batchId, int scenariosPerBatch) const;
};

#endif 