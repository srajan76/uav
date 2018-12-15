#include <ilcplex/ilocplex.h>
#include <lemon/list_graph.h>
#include <lemon/hao_orlin.h>
#include <lemon/connectivity.h>
#include <optionParser.hpp>
#include "instance.hpp"
#include "model.hpp"
#include "edge.hpp"
#include "hpath.hpp"

using namespace std;

int main(int argc, char* argv[]){

    op::OptionParser opt;
    opt.add_option("h", "help", "shows option help" ); 
    opt.add_option("p", "instance_path", "instance_path", "../data/" );
    opt.add_option("f", "file", "name of the instance file", "10-3-5-0.txt" );

    // parse the options and verify that all went well
    bool correct_parsing = opt.parse_options(argc, argv);
    
    if(!correct_parsing) return EXIT_FAILURE;
    if(op::str2bool(opt["h"])) { 
        opt.show_help();
        return 0;
    }

    Instance instance;
    instance.setName(opt["f"]);
    instance.setPath(opt["p"]);
    instance.readData();
    
    int numTargets = instance.getNumTargets();

    for (int i=0; i<numTargets; ++i) {
        if (i == instance.getSource() || i == instance.getDestination()) 
            continue;
        std::cout << "target " << i << ": " << instance.getSatellitesAtTarget(i).size() << std::endl;
    }

    auto targets = instance.getTargetCoords();
    std::vector<int> targetIndexes = {0,1,2,3,4,5,6,7,8,9};
    std::vector<int> satelliteIndexes = {};
    HamiltonianPath hPath(instance);

    hPath.populatePathData(targetIndexes, satelliteIndexes, instance.getSource(), instance.getDestination());
    hPath.createEdges();

    auto edges = hPath.getEdges();
    auto edgeMap = hPath.getEdgeMap();
    std::cout << "created edges " << std::endl;
    
    return 0;
}
