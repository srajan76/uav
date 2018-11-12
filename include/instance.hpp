#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#pragma once

#include <iostream>
#include <fstream> 
#include <string>
#include <unordered_map>
#include <tuple>
#include <set>

#define _USE_MATH_DEFINES



class Instance {
    protected:
        std::string _name = "";
        std::string _path = "./data/";
        std::tuple<double, double> _grid;
        int _numTargets = 10;
        std::unordered_map<int, std::tuple<double, double> > _targetCoords;
        int _numSatellitesPerTarget = 3;
        int _numSatellites = 30;
        float _radius = 2;
        std::unordered_map<int, std::set<int> > _satelliteMap; // target id -> {satellite set}
        std::unordered_map<int, std::tuple<double, double> > _satelliteCoords; 
        


    public:
        Instance();
        Instance(const Instance& in);

        void setName(std::string);
        void setPath(std::string);
        void setNumTargets(int);
        void setTargetCoords(std::unordered_map<int, std::tuple<double, double> >);
        

        std::string getName() const;
        std::string getPath() const;
        int getNumTargets() const;
        std::unordered_map<int, std::tuple<double, double> > getTargetCoords() const; 

        void createData();
        void readData();
        void writeData();

};


#endif
