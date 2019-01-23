#include "instance.hpp"
#include "model.hpp"
#include "edge.hpp"
#include "scenarios.hpp"
#include "twostage.hpp"
#include "dubins.hpp"

TwoStage::TwoStage(const Instance & instance) :
    _instance(instance), 
    _model(), 
    _scenarios(),
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
    _pathCost() {};

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

    for (int i=0; i<getNumVertices(); ++i) {
        for (int j=0; j<getNumVertices(); ++j) {
            if (i == j) continue;
            if (j == getSource()) continue;
            if (i == getDestination()) continue;
            auto ith = coords.at(i);
            auto jth = coords.at(j);
            std::vector<double> qInitial = {std::get<0>(ith), std::get<1>(ith), std::get<2>(ith)};
            std::vector<double> qFinal = {std::get<0>(jth), std::get<1>(jth), std::get<2>(jth)};
            DubinsPath path = getDubinsShortestPath(qInitial, qFinal, getInstance().getTurnRadius());
            double directCost = path.getLength();
            auto satellites = getInstance().getSatellitesAtTarget(i);
            if (satellites.size() == 0) hasRecourseEdge.push_back(false);
            else hasRecourseEdge.push_back(true);
            edges.push_back(Edge(i, j, directCost));
            edgeMap.insert({std::make_tuple(i, j), edges.size()-1});
        }
    }

    _firstStageEdges = edges;
    _hasRecourseEdge = hasRecourseEdge;
    _edgeMap = edgeMap;
    return;
}
