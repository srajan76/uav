#ifndef EDGE_HPP
#define EDGE_HPP

#pragma once

class Edge {
    protected:
        int _from;
        int _to;
        double _cost;

    public: 
        Edge() : _from(), _to(), _cost() {};
        Edge(int i, int j) : _from(i), _to(j), _cost() {};
        Edge(int i, int j, double cost) : _from(i), _to(j), _cost(cost) {};

        void setCost(double cost) { _cost = cost; };
        int from() { return _from; };
        int to() { return _to; };
        double cost() { return _cost; };
};

#endif 