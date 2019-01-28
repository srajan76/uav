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
        std::cout << "first stage cost : " << 
            formulation.getFirstStageCost() << std::endl;
        std::cout << "computed cost : " << 
            formulation.getPathCost() << std::endl;
        auto path = formulation.getPath();
        
        batchLB.push_back(formulation.getPathCost());
        
        std::cout << "number of vertices in optimal path : " << path.size();
        for (int i=0; i<path.size(); ++i) {
            if (i%10 == 0) 
                std::cout << std::endl;
            std::cout << path[i] << " ";
        }
    
        std::cout << std::endl;
        auto ub = formulation.getUB();
        std::cout << "mean ub: " << std::get<0>(ub) << ", " 
            << "stddev ub: " << std::get<1>(ub) << std::endl; 

        auto ubRange = formulation.getUBRange();
        std::cout << "95\% interval UB bounds : [" 
            << std::get<0>(ubRange) << ", " << std::get<1>(ubRange) 
            << "]" << std::endl << std::endl;
    }
      

    return 0;
}



