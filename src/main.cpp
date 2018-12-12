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
    instance.setName("10-3-1.txt");
    instance.setPath("../data/");
    instance.readData();
    //random check to see if data is reading ok
    //std::cout << instance.getRadius() << std::endl;

    // Create target coordinate map for the instance
    std::unordered_map<int, std::tuple<double, double>> map1 = instance.getTargetCoords();
    int src =0;
    int dest = 9;
    void printArray(double ** arr, unordered_map<int, std::tuple<double, double>> map1);
    double ** costMatrix(unordered_map<int, std::tuple<double, double>> map1);
    double ** Cij;
    
    //Checking to see if the Map is read and prints correctly
    //for (int l = 0; l < instance.getNumTargets(); ++l)
    //std::cout << map1.find(l)->first <<"\t" <<get<0>(map1.find(l)->second)<<"\t" << get<1>(map1.at(l)) << endl;
    
    
    Cij = costMatrix(map1);
    printArray(Cij, map1) ;

    IloEnv env;
    
    IloModel model(env);
    //defining decision variables
    typedef IloArray<IloNumVarArray> NumVarMatrix;
    NumVarMatrix isVisit(env,int(map1.size()));
    for(int i=0; i< int(map1.size()); i++) {
        isVisit[i] = IloNumVarArray(env, int(map1.size()));
    for(int j=0; j< int(map1.size()); j++) 
        isVisit[i][j] = IloNumVar(env, 0,1,ILOINT);
    }
    //defining expression for objective
    IloExpr obj(env);
    for(int i=0; i< int(map1.size()); i++) {
        for(int j=0; j< int(map1.size()); j++) 
            obj += Cij[i][j] * isVisit[i][j];
        
    }
    //adding the objective to solve the model
    model.add(IloMinimize(env, obj));
    obj.end();
    //adding constraints

    for(int i=0; i< int(map1.size()); i++) {
        IloExpr rowConstraint(env);
    
        for(int j=0; j< int(map1.size()); j++) 
         rowConstraint +=  isVisit[i][j];
        if (src != dest && i== dest)
        model.add( rowConstraint ==0);
        else
        model.add( rowConstraint ==1);
        rowConstraint.end();
    }
        for(int j=0; j< int(map1.size()); j++) {
        IloExpr colConstraint(env);
    
        for(int i=0; i< int(map1.size()); i++) 
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
void printArray(double ** arr, unordered_map<int, std::tuple<double, double>> _targetCoords ){
   
     for (int i =0; i < int(_targetCoords.size()); ++i){
         for (int j =0; j <int(_targetCoords.size()); ++j)
         cout << arr[i][j]<<"\t";
     cout << endl;}
 }
 double ** costMatrix(unordered_map<int, std::tuple<double, double>> _targetCoords){
     
     double ** arr = new double*[int(_targetCoords.size())];
     for (int i =0; i < int(_targetCoords.size()); ++i){
         arr[i] = new double[int(_targetCoords.size())];
         for (int j =0; j <int(_targetCoords.size()); ++j){
                 
         if ( i ==j)
         arr[i][j] =  std::numeric_limits<double>::infinity();
         else
         arr[i][j] = sqrt(pow(get<0>(_targetCoords.at(i)) - get<0>(_targetCoords.at(j)), 2) + pow(get<1>(_targetCoords.at(i)) - get<1>(_targetCoords.at(j)), 2));          
            
         }
     }
 return arr;
 }


