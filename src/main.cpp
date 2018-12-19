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
<<<<<<< HEAD
    instance.setName("500-3-5-0.txt");
    instance.setPath("../data/");
=======
    instance.setName(opt["f"]);
    instance.setPath(opt["p"]);
>>>>>>> 1e0d35b3f306cc666082a713b370d05218c15518
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

<<<<<<< HEAD

    //Defining Source and destination here since hpath.hpp and hpath.cpp
    // has not been fully created
    int src = 250;
    int dest = 289;
    
    //CPEX MODEL DEFINITION
    IloEnv env;
    IloModel model(env);

    //defining decision variables
    typedef IloArray<IloNumVarArray> NumVarMatrix;
    NumVarMatrix isVisit(env,instance.getNumTargets());
    for(int i=0; i< instance.getNumTargets(); i++) {
        isVisit[i] = IloNumVarArray(env, instance.getNumTargets());
    for(int j=0; j< instance.getNumTargets(); j++) 
        isVisit[i][j] = IloNumVar(env, 0,1,ILOINT);
    }

    //defining expression for objective and adding the objective to solve the model
    IloExpr obj(env);
    for(int i=0; i< instance.getNumTargets(); i++) {
        for(int j=0; j< instance.getNumTargets(); j++) {
            auto target_i = instance.getTargetCoords().at(i);
            auto target_j = instance.getTargetCoords().at(j);
            double cost = 0.0; 
            if (i == j) cost = CPX_INFBOUND;
             else cost = std::hypot(std::get<0>(target_i) - std::get<0>(target_j), std::get<1>(target_i) - std::get<1>(target_j));
            obj += cost * isVisit[i][j];
        }  
    }
    model.add(IloMinimize(env, obj));
    obj.end();
    
    //adding constraints
    for(int i=0; i< instance.getNumTargets(); i++) {
        IloExpr rowConstraint(env);
        for(int j=0; j< instance.getNumTargets(); j++) 
            rowConstraint +=  isVisit[i][j];
        if (src != dest && i== dest)
        model.add( rowConstraint ==0);
        else
        model.add( rowConstraint ==1);
        rowConstraint.end();
    }
        for(int j=0; j< instance.getNumTargets(); j++) {
        IloExpr colConstraint(env);
    
        for(int i=0; i< instance.getNumTargets(); i++) 
         colConstraint +=  isVisit[i][j];
        if (src != dest && j== src)
        model.add( colConstraint ==0);
        else
        model.add( colConstraint ==1);
        colConstraint.end();
    }
        
    
    //Initialize and inititate solving the model by CPLEX
    IloCplex cplex(model);
    cplex.extract(model);
    cplex.solve();
      

    // OUTPUT      
      env.out() << "Solution status = " << cplex.getStatus() << endl;
      env.out() << "Solution value = " << cplex.getObjValue() << endl;
      
      
      lemon::ListGraph optimal_path;
      lemon::ListGraph::Node node_optimal_path [instance.getNumTargets()];
      for (int i =0;i< instance.getNumTargets(); i++)
       node_optimal_path[i] =optimal_path.addNode();
    cout << countNodes(optimal_path) <<endl;
      for (int i =0;i< instance.getNumTargets(); i++) {
          
          for (int j =0;j< instance.getNumTargets(); j++) {
              
              if (cplex.getValue(isVisit[i][j] ==1))
              optimal_path.addEdge(node_optimal_path[i], node_optimal_path[j]);
              //cout << cplex.getValue(isVisit[i][j]) << "\t";
          }
       //cout << endl;
      }
      cout << countEdges(optimal_path) <<endl;
    env.end();

=======
    auto edges = hPath.getEdges();
    auto edgeMap = hPath.getEdgeMap();
    std::cout << "created edges " << std::endl;
    
>>>>>>> 1e0d35b3f306cc666082a713b370d05218c15518
    return 0;
}



