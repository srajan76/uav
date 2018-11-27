#include "instance.hpp"
using namespace std;

void Instance::createData() {
    mt19937 generator(getSeed());
    setGrid(std::make_tuple(100.0, 100.0));
    
    // creating target coordinate generator engine 
    double minCoord = 0.0 + getRadius();
    double maxXCoord = getXGridSize() - getRadius();
    double maxYCoord = getYGridSize() - getRadius();
    std::uniform_real_distribution<> xCoordinateGenerator(minCoord, maxXCoord);
    std::uniform_real_distribution<> yCoordinateGenerator(minCoord, maxYCoord);

    // creating satellite coordinate generator engine
    std::uniform_real_distribution<> angleGenerator(0, 2*M_PI);
    std::uniform_real_distribution<> radiusGenerator(0, getRadius());

    std::unordered_map<int, std::tuple<double, double>> targetCoords;
    std::unordered_map<int, int> satelliteMap; 
    std::unordered_map<int, std::tuple<double, double>> satelliteCoords; 

    for (auto i=0; i<getNumTargets(); ++i) {
        std::tuple<double, double> coord = std::make_tuple(xCoordinateGenerator(generator), yCoordinateGenerator(generator));
        targetCoords.insert({i, coord});
    }

    int satelliteCount = 0;

    for (auto const & targetCoord : targetCoords) {
        int targetIndex = targetCoord.first;
        double targetXCoord = std::get<0>(targetCoord.second);
        double targetYCoord = std::get<1>(targetCoord.second);
        for (auto i=0; i<getNumSatellitesPerTarget(); ++i) {
            double angle = angleGenerator(generator);
            double radius = radiusGenerator(generator);
            std::tuple<double, double> coord = std::make_tuple(targetXCoord + radius * std::cos(angle), 
                targetYCoord + radius * std::sin(angle));
            satelliteCoords.insert({satelliteCount, coord});
            satelliteMap.insert({satelliteCount, targetIndex});
            satelliteCount += 1;
        }
    }

    setTargetCoords(targetCoords);
    setSatelliteCoords(satelliteCoords);
    setSatelliteMap(satelliteMap);

    return;
}

void Instance::writeData() {
    ofstream outfile;
    std::string file = getPath() + getName();
    outfile.open(file);

    auto targetCoords = getTargetCoords(); 
    auto satelliteCoords = getSatelliteCoords(); 
    auto satelliteMap = getSatelliteMap(); 
    
    outfile << getSeed() << std::endl << 
        getNumTargets() << std::endl << 
        getNumSatellitesPerTarget() << std::endl << 
        getRadius() << std::endl << std::endl;

    for (auto i=0; i<getNumTargets(); ++i) { 
        int targetIndex = i;
        std::tuple<double, double> coord = targetCoords.at(i);
        outfile << targetIndex << " " << 
            std::setprecision(4) << fixed << std::get<0>(coord) << " " << 
            std::setprecision(4) << fixed << std::get<1>(coord) << std::endl;
    }
    outfile << std::endl;

    for (auto i=0; i<getNumSatellites(); ++i) { 
        int targetIndex = satelliteMap.at(i);
        std::tuple<double, double> coord = satelliteCoords.at(i);
        outfile << targetIndex << " " << 
            std::setprecision(4) << fixed << std::get<0>(coord) << " " << 
            std::setprecision(4) << fixed << std::get<1>(coord) << std::endl;
    }
    
    outfile.close();
    return;
}




void Instance::readData() {
    ifstream infile;
    std::string file = getPath() + getName();
    infile.open(file);
    if (!infile) {
        std::cerr << "file does not exist, quitting program" << std::endl;
        exit(1);
    }

    int numTargets, numSatellitesPerTarget, numSatellites;
    int seed; 
    float radius;
    std::unordered_map<int, std::tuple<double, double>> targetCoords;
    std::unordered_map<int, int> satelliteMap; 
    std::unordered_map<int, std::tuple<double, double>> satelliteCoords; 

    infile >> seed >> numTargets >> numSatellitesPerTarget >> radius; 
    numSatellites = numTargets * numSatellitesPerTarget;

    for (auto i=0; i<numTargets; ++i) {
        int index; 
        double x, y;
        infile >> index >> x >> y;
        targetCoords.insert({index, std::make_tuple(x, y)});
    }

    for (auto i=0; i<numSatellites; ++i) {
        int targetIndex; 
        double x, y;
        infile >> targetIndex >> x >> y;
        satelliteCoords.insert({i, std::make_tuple(x, y)});
        satelliteMap.insert({i, targetIndex});
    }

    infile.close();
    setNumTargets(numTargets);
    setNumSatellitesPerTarget(numSatellitesPerTarget);
    setNumSatellites();
    setSeed(seed);
    setRadius(radius);

    setTargetCoords(targetCoords);
    setSatelliteCoords(satelliteCoords);
    setSatelliteMap(satelliteMap);
    
    return;
}
