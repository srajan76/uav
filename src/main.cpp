#include <ilcplex/ilocplex.h>
#include <lemon/list_graph.h>
#include <lemon/hao_orlin.h>
#include <lemon/connectivity.h>
#include "optionParser.hpp"
#include "instance.hpp"

using namespace std;

int main(int argc, char* argv[]){

    Instance instance;
    instance.setName("10-3-0.txt");
    instance.setPath("../data/");
    instance.readData();

    std::cout << instance.getRadius() << std::endl;
    return 0;
}
