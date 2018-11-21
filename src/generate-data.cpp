#include "instance.hpp"
#include "optionParser.hpp"

using namespace std;



int main(int argc, char* argv[]) {
Instance A;
A.setNumTargets(50);
cout << "No of targets is " << A.getNumTargets() <<endl;
A.setNumSatellitesPerTarget(3);
cout << "No of targets per satellite is " << A.getNumSatellitesPerTarget() <<endl;
A.createData();
A.writeData();

   
    return 0;
}
