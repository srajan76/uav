#include <ilcplex/ilocplex.h>
#include <lemon/list_graph.h>
#include <lemon/hao_orlin.h>
#include <lemon/connectivity.h>
#include "optionParser.hpp"
#include "instance.hpp"
#include "hpath.hpp"

using namespace std;

int main(int argc, char* argv[]){

    Instance instance;
    instance.setName("500-3-5-0.txt");
    instance.setPath("../data/");
    instance.readData();


    //random check to see if data is reading ok
    //std::cout << instance.getRadius() << std::endl;

    // Create target coordinate map for the instance
    int src = 0;
    int dest = 9;

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
    //defining expression for objective
    
    IloExpr obj(env);
    for(int i=0; i< instance.getNumTargets(); i++) {
        for(int j=0; j< instance.getNumTargets(); j++) {
            auto target_i = instance.getTargetCoords().at(i);
            auto target_j = instance.getTargetCoords().at(j);
            double cost = 0.0; 
            if (i == j) cost = 10000000;
            else cost = std::hypot(std::get<0>(target_i) - std::get<0>(target_j), std::get<1>(target_i) - std::get<1>(target_j));
            obj += cost * isVisit[i][j];
        }  
    }

    //adding the objective to solve the model
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
        
    
    
    IloCplex cplex(model);
    cplex.extract(model);
    cplex.solve();
      

     //IloNumArray vals(env);
      env.out() << "Solution status = " << cplex.getStatus() << endl;
      env.out() << "Solution value = " << cplex.getObjValue() << endl;
      //cplex.getValues(vals, vars);
      //env.out() << "Values = " << vals << endl;

    env.end();

    return 0;
}



