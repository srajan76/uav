#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#pragma once

#include <iostream>
#include <fstream> 
#include <string>
#include <unordered_map>
#include <tuple>
#include <set>
#include<sstream>
#include<vector>
#include<random>

#define _USE_MATH_DEFINES
using namespace std;


class Instance {
    protected:
        string _name = "_Data.txt";
        string _path = "./data/";
        tuple<double, double> _grid;
        int _numTargets = 10;
        unordered_map<int, std::tuple<double, double> > _targetCoords;
        int _numSatellitesPerTarget = 3;
        int _numSatellites = 30;
        float _radius = 2;
        unordered_map<int, set<int> > _satelliteMap; // target id -> {satellite set}
        unordered_map<int, tuple<double, double> > _satelliteCoords; 
        


    public:
        Instance();
        Instance(const Instance & in);

        void setName(string); // sets the instance name no of targets- no of statelite tagets/target - instance number
        void setPath(string);// where the text file neeeds to be saved in the data folder of UAV
        void setNumTargets(int);
        void setNumSatellitesPerTarget(int);
        void setTargetCoords(unordered_map<int, tuple<double, double> >); 
        void setSatelliteCoords(unordered_map<int, tuple<double, double> >); 
        

        string getName() const; //return the name
        string getPath() const; // return the path
        int getNumTargets() const; //returns no of targets
        int getNumSatellitesPerTarget() const;//returns no of  satellites per target
        unordered_map<int, std::tuple<double, double> > getTargetCoords() const; // get from set target coords
        unordered_map<int, std::tuple<double, double> > getSatelliteCoords() const;

        void createData(); // do all random generation
        void readData(); // read data from a txt file
        void writeData(); // write to a text file


};


#endif
