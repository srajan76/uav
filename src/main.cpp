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
    opt.add_option("s", "num_scenarios", "number of scenarios per batch", "100");
    opt.add_option("r", "probability", "pr of success", "0.5");
    opt.add_option("d", "deterministic", "solve deterministic");

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

    double prob = op::str2double(opt["r"]);

    Scenarios scenarios;
    scenarios.setSeed(instance.getSeed());
    scenarios.setNumTargets(instance.getNumTargets());
    scenarios.generateScenarios(prob);

    std::vector<double> batchLB;
    std::vector<double> ubUpper;
    std::vector<double> ubLower;
    
    
    if (op::str2bool(opt["d"])) {
        TwoStage formulation(instance, scenarios);
        formulation.initialize();
        formulation.populateEdges();
        formulation.solveDeterministic();
        auto ubRange = formulation.getUBRange();
        double detMean = (std::get<0>(ubRange) + std::get<1>(ubRange))/2.0;
        std::cout << detMean << std::endl;
        return 0;
    }
    

    for (int i=0; i<10; ++i) {
        TwoStage formulation(instance, scenarios);
        formulation.initialize();
        formulation.populateEdges();
        formulation.solve(i+1, op::str2int(opt["s"]));
        batchLB.push_back(formulation.getPathCost());  
        auto ubRange = formulation.getUBRange();
        ubLower.push_back(std::get<0>(ubRange));
        ubUpper.push_back(std::get<1>(ubRange));
    }

    int index = std::distance(
        std::begin(ubUpper), 
        std::max_element(
        std::begin(ubUpper), 
        std::end(ubUpper))
    );
    double maxUpperBound = ubUpper[index];
    double maxLowerBound = ubLower[index];


    double sum = std::accumulate(batchLB.begin(), 
        batchLB.end(), 0.0);
    double mean = sum / batchLB.size();

    std::vector<double> diff(batchLB.size());
    std::transform(
        batchLB.begin(), 
        batchLB.end(), 
        diff.begin(), 
        [mean](double x) { return x - mean; });

    double sqSum = std::inner_product(
        diff.begin(), diff.end(), diff.begin(), 0.0);
    double stddev = std::sqrt(sqSum / (batchLB.size()-1));

    double kappa = 2.2621; // alpha = 0.05, degrees of freedom = 9

    double lower = mean - (kappa*stddev)/std::sqrt(batchLB.size());
    double upper = mean + (kappa*stddev)/std::sqrt(batchLB.size());

    std::tuple<double, double> lbRange = std::make_tuple(lower, upper);

    double meanUpper = (maxLowerBound + maxUpperBound)/2.0;
    double meanLower = (lower + upper)/2.0;
    double epLower = (meanLower - lower);
    double epUpper = (meanUpper - maxLowerBound);
    double gapEstimate;
    if (meanUpper > meanLower)
        gapEstimate = (meanUpper - meanLower + 
            epLower + epUpper);
    else 
        gapEstimate =  (epLower + epUpper);
 
    // std::cout << meanLower << " " << epLower << " " <<  
    // meanUpper << " " << epUpper << " " << 
    // gapEstimate << " " << std::endl;

    std::cout << meanUpper << std::endl;
    
    return 0;
}



