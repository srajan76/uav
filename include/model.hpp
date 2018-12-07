#ifndef MODEL_HPP
#define MODEL_HPP

#pragma once

#include <ilcplex/ilocplex.h>
#include <limits>
#include <unordered_map>
#include <vector>

class Model {
    protected:
        IloEnv _env;
        IloModel _model;
        std::unordered_map<std::string, IloNumVarArray> _variables;
        std::vector<IloRange> _constraints;

    public:
        Model() : _env(), _model(IloModel(_env)), _variables(), _constraints() {};

        void setVariables(std::unordered_map<std::string, IloNumVarArray> variables) { _variables = variables; };
        void setConstraints(std::vector<IloRange> constraints) { _constraints = constraints; };

        IloEnv& getEnv() { return _env; };
        IloModel& getModel() { return _model; };
        std::unordered_map<std::string, IloNumVarArray>& getVariables() { return _variables; };
        std::vector<IloRange>& getConstraints() { return _constraints; };
        void clearEnv() { _env.end(); };
};

void populateHPModel(HamiltonianPath & hPath) {

    Model & model = getModel(hPath);
    /* create the model using the CPLEX API */
    
    return 0;
}

#endif