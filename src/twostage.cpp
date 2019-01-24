#include "instance.hpp"
#include "model.hpp"
#include "edge.hpp"
#include "scenarios.hpp"
#include "twostage.hpp"
#include "dubins.hpp"

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
    std::vector<std::vector<int>> recoursePaths;

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
