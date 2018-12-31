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
std::vector<IloRange> generateLazyConstraints(Model & model, HamiltonianPath & hPath, std::unordered_map<std::string, IloNumArray> &variableValues) {

    std::vector<IloRange> constraints;
    lemon::ListGraph supportGraph;
    for (int i=0; i<hPath.getNumVertices(); ++i) supportGraph.addNode();

    IloNumArray xVals = variableValues.at("x");

    for (auto i=0; i<xVals.getSize(); ++i)
        if (xVals[i] > 1E-5) {
            int from = hPath.getEdges()[i].from();
            int to = hPath.getEdges()[i].to();
            supportGraph.addEdge(supportGraph.nodeFromId(from), supportGraph.nodeFromId(to));
        }

    lemon::ListGraph::NodeMap<int> componentMap(supportGraph);
    int numComponents = connectedComponents(supportGraph, componentMap);

if (numComponents == 1)
        return constraints;
    
    std::vector<std::set<int>> components(numComponents);
	for (lemon::ListGraph::NodeIt n(supportGraph); n!=lemon::INVALID; ++n)
		components[componentMap[n]].insert(supportGraph.id(n));

    for (auto & component : components) {
        IloExpr expr(model.getEnv());
        //auto edgeIds = instance.getGamma(component);
        for (auto const & i: component /*auto const & edgeId : edgeIds*/)
            expr += model.getVariables().at("x")[i];
        IloRange constr(model.getEnv(), expr, component.size()-1);
        constraints.push_back(constr);
    }
    
    return constraints;
};
ILOLAZYCONSTRAINTCALLBACK2(addLazyCallback, Model&, model, HamiltonianPath&, hPath) {
    IloEnv env = getEnv();
    std::unordered_map<std::string, IloNumArray> variableValues;
    variableValues.insert(std::make_pair("x", IloNumArray(env, model.getVariables().at("x").getSize())));
    getValues(variableValues.at("x"), model.getVariables().at("x"));
    
    std::vector<IloRange> constraints = generateLazyConstraints(model,hPath, variableValues);

    for (IloRange constr : constraints)
        model.getModel().add(constr);

    variableValues.at("x").end();
    constraints.clear();
};
int main(int argc, char* argv[]){

    op::OptionParser opt;
    opt.add_option("h", "help", "shows option help" ); 
    opt.add_option("p", "instance_path", "instance_path", "../data/" );
    opt.add_option("f", "file", "name of the instance file", "50-3-5-0.txt" );

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
    

    

    
    std::vector<int> targetIndexes = {};
    for ( int i =0; i < instance.getNumTargets(); ++i)
    targetIndexes.push_back(i);
    std::vector<int> satelliteIndexes = {};
    HamiltonianPath hPath(instance);
    hPath.setSource(5);
    hPath.setDestination(41);
    cout << hPath.getSource()<<"\t"<<hPath.getDestination()<<endl;
    cout <<instance.getSource()<<"\t"<<instance.getDestination()<<endl;

    hPath.populatePathData(targetIndexes, satelliteIndexes, hPath.getSource(),hPath.getDestination());
    cout << hPath.getSource()<<"\t"<<hPath.getDestination()<<endl;
    cout <<instance.getSource()<<"\t"<<instance.getDestination()<<endl;
    hPath.createEdges();

   // auto edges = hPath.getEdges();
   // auto edgeMap = hPath.getEdgeMap();
  //  std::cout << "created edges " << std::endl;
    Model model = hPath.addVariablesandConstraints();
    std::cout << "added variables and constraints " << std::endl;

    

    IloCplex cplex(model.getModel());
    cplex.extract(model.getModel());
//cplex.solve();
//cplex.setParam(IloCplex::Param::Output::WriteLevel, 4);
//cplex.writeSolution(model.getModel().getName());
cplex.use(addLazyCallback(model.getEnv(), model, hPath));
//cplex.writeSolution(model.getModel().getName());
ofstream outfile;
outfile.open("ouput");
cplex.setOut(outfile);
cplex.solve();
    std::cout<<"SOLVED!!"<<std::endl;
    outfile.close();
   
    return 0;
}
