#include <ilcplex/ilocplex.h>
#include <lemon/list_graph.h>
#include <lemon/hao_orlin.h>
#include <lemon/connectivity.h>
#include <optionParser.hpp>
#include "util.hpp"
#include "instance.hpp"
#include "model.hpp"
#include "edge.hpp"
#include "scenarios.hpp"
#include "twostage.hpp"


int main(int argc, char* argv[]){

    op::OptionParser opt;
    opt.add_option("h", "help", "shows option help" ); 
    opt.add_option("p", "instance_path", "instance_path", "../data/" );
    opt.add_option("f", "file", "name of the instance file", "10-1-5-0.txt" );

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

    
    Scenarios scenarios;
    scenarios.setSeed(instance.getSeed());
    scenarios.setNumTargets(instance.getNumTargets());
    scenarios.generateScenarios();

    std::vector<double> batchLB;
    
    for (int i=0; i<10; ++i) {
        TwoStage formulation(instance, scenarios);
        formulation.initialize();
        formulation.populateEdges();
        formulation.solve(i+1, 100);
        formulation.writeSolution(i+1, 100);
        
        batchLB.push_back(formulation.getPathCost());  
    }
      

    return 0;
}



