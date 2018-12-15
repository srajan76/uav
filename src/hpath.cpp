#include "instance.hpp"
#include "model.hpp"
#include "edge.hpp"
#include "hpath.hpp"

HamiltonianPath::HamiltonianPath(const Instance & instance) :
    _instance(instance), _model(Model()), 
    _vertexCoords(), _isTarget(), _isSatellite(), 
    _vertexToTargetMap(), _vertexToSatelliteMap(), 
    _source(0), _destination(1), _edges(), 
    _edgeMap(), _path(), _pathCost(0.0) {};


void HamiltonianPath::populatePathData(std::vector<int> & targets, 
    std::vector<int> satellites, 
    int sourceTarget, 
    int destinationTarget) {

    auto targetCoords = getInstance().getTargetCoords();
    auto satelliteCoords = getInstance().getSatelliteCoords();
    std::unordered_map<int, std::tuple<double, double>> vertexCoords; 
    std::unordered_map<int, int> vertexToTargetMap;
    std::unordered_map<int, int> vertexToSatelliteMap;

    int numVertices = targets.size() + satellites.size();
    std::unordered_map<int, bool> isTarget, isSatellite;
    for (int i=0; i<numVertices; ++i) {
        isTarget.insert({i, false});
        isSatellite.insert({i, false});
    }

    int vertexIndex = 0;
    for (int target : targets) {
        if (target == sourceTarget) setSource(vertexIndex);
        if (target == destinationTarget) setDestination(vertexIndex);
        vertexCoords.insert({vertexIndex, targetCoords.at(target)});
        isTarget[vertexIndex] = true;
        vertexToTargetMap.insert({vertexIndex, target});
        vertexIndex++;
    }

    for (int satellite : satellites) {
        vertexCoords.insert({vertexIndex, satelliteCoords.at(satellite)});
        isSatellite[vertexIndex] = true;
        vertexToSatelliteMap.insert({vertexIndex, satellite});
        vertexIndex++;
    }

    setVertexCoords(vertexCoords);
    setIsTarget(isTarget);
    setIsSatellite(isSatellite);
    setVertexToTargetMap(vertexToTargetMap);
    setVertexToSatelliteMap(vertexToSatelliteMap);
    return;
};

void HamiltonianPath::createEdges() {

    auto vertexCoords = getVertexCoords();
    std::vector<Edge> edges;
    std::unordered_map<std::tuple<int, int>, int> edgeMap;

    for (int i=0; i<getNumVertices(); ++i) {
        for (int j=i+1; j<getNumVertices(); ++j) {
            auto ith = vertexCoords.at(i);
            auto jth = vertexCoords.at(j);
            if (i == getSource() && j == getDestination()) 
                continue;
            double cost = std::hypot(std::get<0>(ith) - std::get<0>(jth), std::get<1>(ith) - std::get<1>(jth));
            edges.push_back(Edge(i, j, cost));
            edgeMap.insert({std::make_tuple(i, j), edges.size()-1});
            edgeMap.insert({std::make_tuple(j, i), edges.size()-1});
        }
    }

    setEdges(edges);
    setEdgeMap(edgeMap);
    return;

};