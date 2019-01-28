#include "instance.hpp"
#include "model.hpp"
#include "edge.hpp"
#include "scenarios.hpp"
#include "twostage.hpp"
#include "dubins.hpp"
#include "callbacks.hpp"

#include <cassert>

TwoStage::TwoStage(const Instance & instance,
    const Scenarios & scenarios) :
    _instance(instance), 
    _model(), 
    _scenarios(scenarios),
    _coords(instance.getTargetCoords()), 
    _source(), 
    _destination(), 
    _firstStageEdges(), 
    _recourseEdges(),
    _hasRecourseEdge(),
    _recoursePaths(), 
    _edgeMap(), 
    _path(), 
    _firstStageCost(), 
    _secondStageCost(), 
    _pathCost(), 
    _ub(), 
    _ubRange() {};

void TwoStage::initialize() { 
    setSource(); 
    setDestination();
    return;
};

void TwoStage::populateEdges() {
    auto coords = getCoords();
    auto satelliteCoords = getInstance().getSatelliteCoords();
    std::vector<Edge> edges;
    std::vector<Edge> recourseEdges;
    std::vector<bool> hasRecourseEdge;
    std::unordered_map<std::tuple<int, int>, int> edgeMap;
    std::vector<std::vector<int>> recoursePaths;

    for (int i=0; i<getNumVertices(); ++i) {
        for (int j=0; j<getNumVertices(); ++j) {
            if (i == j) continue;
            if (i == getSource() && j == getDestination()) continue;
            if (j == getSource()) continue;
            if (i == getDestination()) continue;
            auto ith = coords.at(i);
            auto jth = coords.at(j);
            std::vector<double> qInitial = {std::get<0>(ith), std::get<1>(ith), std::get<2>(ith)};
            std::vector<double> qFinal = {std::get<0>(jth), std::get<1>(jth), std::get<2>(jth)};
            DubinsPath path = getDubinsShortestPath(qInitial, qFinal, getInstance().getTurnRadius());
            double directCost = path.getLength();
            auto satellites = getInstance().getSatellitesAtTarget(i);
            double recourseCost;
            if (satellites.size() == 0) {
                hasRecourseEdge.push_back(false);
                recourseEdges.push_back(Edge());
                recoursePaths.push_back({});
            }
            else {
                hasRecourseEdge.push_back(true);
                auto kth = satelliteCoords.at(satellites[0]); 
                std::vector<double> qIntermediate = {std::get<0>(kth), std::get<1>(kth), std::get<2>(kth)};
                path = getDubinsShortestPath(qInitial, qIntermediate, getInstance().getTurnRadius());
                recourseCost = path.getLength();
                path = getDubinsShortestPath(qIntermediate, qFinal, getInstance().getTurnRadius());
                recourseCost += path.getLength();
                recourseEdges.push_back(Edge(i, j, recourseCost));
                recoursePaths.push_back({i, satellites[0], j});
            }
            edges.push_back(Edge(i, j, directCost));
            edgeMap.insert({std::make_tuple(i, j), edges.size()-1});
        }
    }

    _firstStageEdges = edges;
    _hasRecourseEdge = hasRecourseEdge;
    _recourseEdges = recourseEdges;
    _recoursePaths = recoursePaths;
    _edgeMap = edgeMap;
    return;
}

void TwoStage::populateConstraints() {

    Model model;
    model.getModel().setName(getInstance().getName().c_str());

    IloNumVarArray x(model.getEnv());
    model.getVariables().insert({"x", x});

    for (int i=0; i<_firstStageEdges.size(); ++i) {
        auto edge = _firstStageEdges[i];
        std::string varname = "x_" + std::to_string(edge.from()) +  "_" + std::to_string(edge.to());
        IloNumVar var(model.getEnv(), 0, 1, ILOINT, varname.c_str());
        model.getVariables().at("x").add(var);
    }

    for (int i=0; i<getNumVertices(); ++i) {
        IloExpr inExpr(model.getEnv());
        IloExpr outExpr(model.getEnv());
        for (int j=0; j<getNumVertices(); ++j) {
            if (i == j) continue; 
            auto inItr = _edgeMap.find(std::make_tuple(j, i));
            auto outItr = _edgeMap.find(std::make_tuple(i, j));
            if (inItr != _edgeMap.end()) {
                inExpr += model.getVariables().at("x")[inItr->second];
            }
            if (outItr != _edgeMap.end()) {
                outExpr += model.getVariables().at("x")[outItr->second];
            }
        }
        if (i != getDestination()) {
            std::string constrName = "out_" + std::to_string(i);
            IloRange constrOut(model.getEnv(), 1, outExpr, 1, constrName.c_str());
            model.getConstraints().push_back(constrOut);
        }
        if (i != getSource()) {
            std::string constrName = "in_" + std::to_string(i);
            IloRange constrIn(model.getEnv(), 1, inExpr, 1, constrName.c_str());
            model.getConstraints().push_back(constrIn);
        }
    }

    for (IloRange constr : model.getConstraints())  {
        model.getModel().add(constr);
    }

    setModel(model);
    return;
};




void TwoStage::solve() {
    
    populateConstraints();
    IloNumArray cost(_model.getEnv());
    for (int i=0; i<_firstStageEdges.size(); ++i) {
        auto edge = _firstStageEdges[i];
        cost.add(edge.cost());
    }

    _model.getModel().add(
        IloMinimize(_model.getEnv(), 
        IloScalProd(cost, _model.getVariables().at("x"))
    ));

    IloCplex cplex(_model.getEnv());
    cplex.extract(_model.getModel());
    cplex.exportModel("test.lp");
    cplex.setOut(_model.getEnv().getNullStream());
    cplex.use(addLazyCallback(_model.getEnv(), 
        _model,
        _firstStageEdges, 
        _edgeMap, 
        getNumVertices(), 
        getSource(), 
        getDestination()
    ));
    cplex.solve();

    double firstStageCost = 0.0;
    std::vector<std::tuple<int, int>> solutionEdges;
    for (int i=0; i<_model.getVariables().at("x").getSize(); ++i) {
        if (cplex.getValue(_model.getVariables().at("x")[i]) > 0.9) {
            int from = _firstStageEdges[i].from();
            int to = _firstStageEdges[i].to();
            solutionEdges.push_back(std::make_pair(from, to));
            firstStageCost += _firstStageEdges[i].cost();
        }
    }

    computePath(solutionEdges);
    _firstStageCost = firstStageCost;
    _secondStageCost = 0.0;
    _pathCost = _firstStageCost + _secondStageCost;

    return;
};

void TwoStage::solve(int batchId, 
    int numScenariosPerBatch) {

    auto scenarios = _scenarios.getScenarios(
        batchId, numScenariosPerBatch
    );
    assert(scenarios.size() == numScenariosPerBatch);

    populateConstraints();
    IloNumArray cost(_model.getEnv());

    for (int i=0; i<_firstStageEdges.size(); ++i) {
        auto edge = _firstStageEdges[i];
        auto recourseEdge = _recourseEdges[i];
        int from = edge.from();
        int to = edge.to();
        double directCost = edge.cost();
        double recourseCost = 0.0;
        if (_hasRecourseEdge[i]) {
            recourseCost = recourseEdge.cost() - directCost;
            double multiplier = 0;
            for (int j=0; j<scenarios.size(); ++j) {
                multiplier += scenarios[j][from];
            }
            recourseCost *= (multiplier/scenarios.size());
        }
        double effectiveCost = directCost + recourseCost;
        cost.add(effectiveCost);
    }
    
    _model.getModel().add(
        IloMinimize(_model.getEnv(), 
        IloScalProd(cost, _model.getVariables().at("x"))
    ));

    IloCplex cplex(_model.getEnv());
    cplex.extract(_model.getModel());
    cplex.exportModel("test1.lp");
    cplex.setOut(_model.getEnv().getNullStream());
    cplex.use(addLazyCallback(_model.getEnv(), 
        _model,
        _firstStageEdges, 
        _edgeMap, 
        getNumVertices(), 
        getSource(), 
        getDestination()
    ));
    cplex.solve();


    double firstStageCost = 0.0;
    double secondStageCost = 0.0;
    std::vector<std::tuple<int, int>> solutionEdges;
    std::vector<int> solutionEdgeIds;
    for (int i=0; i<_model.getVariables().at("x").getSize(); ++i) {
        if (cplex.getValue(_model.getVariables().at("x")[i]) > 0.9) {
            int from = _firstStageEdges[i].from();
            int to = _firstStageEdges[i].to();
            solutionEdges.push_back(std::make_pair(from, to));
            solutionEdgeIds.push_back(i);
            firstStageCost += _firstStageEdges[i].cost();
            auto recourseEdge = _recourseEdges[i];
            double recourseCost = 0.0;
            if (_hasRecourseEdge[i]) {
                recourseCost = recourseEdge.cost() -
                    _firstStageEdges[i].cost();
                double multiplier = 0;
                for (int j=0; j<scenarios.size(); ++j) {
                    multiplier += scenarios[j][from];
                }
                recourseCost *= (multiplier/scenarios.size());
            }
            secondStageCost += recourseCost;
        }
    }

    computePath(solutionEdges);
    computeUB(solutionEdgeIds);
    _firstStageCost = firstStageCost;
    _secondStageCost = secondStageCost;
    _pathCost = firstStageCost + secondStageCost;
    return;
};

void TwoStage::computePath(
    std::vector<std::tuple<int, int>> & edges) {

    std::vector<int> solution;
    solution.push_back(getSource());

    while (solution.back() != getDestination()) {
        auto it = std::find_if(edges.begin(), edges.end(),
                    [&solution](const std::tuple<int,int>& edge) 
                    {return std::get<0>(edge) == solution.back(); });
        solution.push_back(std::get<1>(*it));
    }

    _path = solution;
    return;
};

void TwoStage::computeUB(
    std::vector<int> & edgeIds) {
    
    std::vector<double> batchUB;
    for (int i=1; i<11; ++i) {
        auto scenarios = _scenarios.getUBScenarios(i);
        assert(scenarios.size() == 100);
        double firstStageCost = 0.0;
        double secondStageCost = 0.0;

        for (auto id : edgeIds) {
            int from = _firstStageEdges[id].from();
            firstStageCost += _firstStageEdges[id].cost();
            auto recourseEdge = _recourseEdges[id];
            double recourseCost = 0.0;
            if (_hasRecourseEdge[id]) {
                recourseCost = recourseEdge.cost() - 
                    _firstStageEdges[id].cost();
                double multiplier = 0;
                for (int j=0; j<scenarios.size(); ++j) 
                    multiplier += scenarios[j][from];
                recourseCost *= (multiplier/scenarios.size());
            }
            secondStageCost += recourseCost;
        }  
        batchUB.push_back(firstStageCost + secondStageCost);  
    }

    double sum = std::accumulate(batchUB.begin(), 
        batchUB.end(), 0.0);
    double mean = sum / batchUB.size();

    std::vector<double> diff(batchUB.size());
    std::transform(
        batchUB.begin(), 
        batchUB.end(), 
        diff.begin(), 
        [mean](double x) { return x - mean; });

    double sqSum = std::inner_product(
        diff.begin(), diff.end(), diff.begin(), 0.0);
    double stddev = std::sqrt(sqSum / (batchUB.size()-1));

    _ub = std::make_tuple(mean, stddev);
    double kappa = 2.2621; // alpha = 0.05, degrees of freedom = 9

    double lower = mean - (kappa*stddev)/std::sqrt(batchUB.size());
    double upper = mean + (kappa*stddev)/std::sqrt(batchUB.size());

    _ubRange = std::make_tuple(lower, upper);
    return;
}

void TwoStage::writeSolution(int batchId, 
    int numScenarios) {

    std::string file = _instance.getName();
    std::string path = "../output/";
    std::string filename = path + "b" 
        + std::to_string(batchId) + "-n" 
        + std::to_string(numScenarios) + "-" 
        + file;

    std::ofstream outfile;
    outfile.open(filename);

    outfile << batchId << std::endl 
        << numScenarios << std::endl 
        << _pathCost << std::endl 
        << _path.size();

    for (int i=0; i<_path.size(); ++i) {
        if (i%10 == 0) 
            outfile << std::endl;
        outfile << _path[i] << " ";
    }
    
    outfile << std::endl;

    outfile << std::get<0>(_ubRange) << " "
        << std::get<1>(_ubRange);

    outfile.close();
    return;
}