#ifndef HPATH_HPP
#define HPATH_HPP

#pragma once

#include "instance.hpp"
#include "model.hpp"

class HamiltonianPath {
    protected: 
        const Instance & _instance;
        const Model & _model;
        std::unordered_map<int, std::tuple<double, double>> _vertexCoords; 
        std::unordered_map<int, bool> _isTarget;
        std::unordered_map<int, bool> _isSatellite;
        std::unordered_map<int, int> _vertexToTargetMap;
        std::unordered_map<int, int> _vertexToSatelliteMap;
        int _source;
        int _destination;
        std::vector<int> _path;
        double _pathCost;
    
    public: 
        HamiltonianPath(const Instance & instance, const Model & model) : _instance(instance), _model(model), _vertexCoords(), 
            _isTarget(), _isSatellite(), _vertexToTargetMap(), _vertexToSatelliteMap(), 
            _source(0), _destination(1), _path(), _pathCost(0.0) {};

        const Instance & getInstance() const { return _instance; };
        const Model & getModel() const { return _model; }
        std::unordered_map<int, std::tuple<double, double>> getVertexCoords() const { return _vertexCoords; };
        std::unordered_map<int, bool> getIsTarget() const { return _isTarget; };
        std::unordered_map<int, bool> getIsSatellite() const { return _isSatellite; };
        std::unordered_map<int, int> getVertexToTargetMap() const { return _vertexToTargetMap; };
        std::unordered_map<int, int> getVertexToSatelliteMap() const { return _vertexToSatelliteMap; };
        int getSource() const { return _source; };
        int getDestination() const { return _destination; };
        std::vector<int> getPath() const { return _path; };
        double getPathCost() const { return _pathCost; };


};

#endif