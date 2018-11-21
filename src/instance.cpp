#include "instance.hpp"


// define all the functions here
Instance::Instance(){}
Instance::Instance(const Instance& in){} 
void Instance::setName (std::string a){
    _name = a;
}
void Instance::setPath(std::string a){
    _path =a;
}
void Instance::setNumTargets(int z){
    _numTargets =z;
}
void Instance::setNumSatellitesPerTarget(int z){
    _numSatellitesPerTarget =z;
}
void Instance::setTargetCoords(unordered_map<int, tuple<double, double> > g){
    _targetCoords = g;
}
void Instance::setSatelliteCoords(unordered_map<int, tuple<double, double> > b){
    _satelliteCoords = b;
}
string Instance::getName() const{
    return _name; 
}
string Instance::getPath() const {
    return _path;
}
int Instance::getNumTargets() const{
    return _numTargets;
}
int Instance::getNumSatellitesPerTarget() const{
    return _numSatellitesPerTarget;
}
unordered_map<int, tuple<double, double> > Instance::getTargetCoords() const{
    return _targetCoords;
}
unordered_map<int, std::tuple<double, double> > Instance::getSatelliteCoords() const{
    return _satelliteCoords;
}




void Instance::createData(){
tuple<double,double> mytuple;
	
// creating random generation engine	
	random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(0.0, 100.0);
	normal_distribution<> dis2(0.0,5.0);

//creating random target coordinates in a 100 x 100 grid and a mapping for target to satellites
for(int x=0; x<_numTargets; x++){
	set<int> myset1;
	get<0>(mytuple)= dis(gen);
	get<1>(mytuple) = dis(gen);
	_targetCoords[x] = mytuple;
	for (int i=1; i<=_numSatellitesPerTarget; ++i) myset1.insert(i*100 + x);
	_satelliteMap[x] = myset1;

}
//creating random statellite targets for each target location that 
//is normally distributed in the vicinity of the target
for ( auto it = _satelliteMap.begin(); it != _satelliteMap.end(); ++it ){
  for (auto it2=(it->second).begin(); it2!=(it->second).end(); ++it2){
	 int k = *it2%100;
	 get<0>(mytuple) = dis2(gen)+get<0>(_targetCoords.at(k)) ;
	 get<1>(mytuple) = dis2(gen)+ get<1>(_targetCoords.at(k)) ;
	 _satelliteCoords[*it2] = mytuple;
}
}
}




void Instance::writeData(){
    ofstream File;
	File.open(_name);
	File  <<_numTargets<< "\n" << _numSatellitesPerTarget << "\n";
    for ( auto it = _targetCoords.begin(); it != _targetCoords.end(); ++it )
    File << it->first<< "\t" << get<0>(it->second)<<"\t"<< get<1>(it->second) << endl;
    for ( auto it = _satelliteCoords.begin(); it != _satelliteCoords.end(); ++it )
    File<< it->first %100<< "\t" << get<0>(it->second)<< "\t" << get<1>(it->second) << endl;

	File.close();

	File.seekp(0,std::ios::end); //to ensure the put pointer is at the end
}




void Instance::readData(){
  ifstream inFile;
    string a,b,c;
    char split_char = '\t';
    
    

    inFile.open(_name);
    if (! inFile){
        cout <<"Error \n";
        exit(1);
    }
    //get the first two lines of input
    getline(inFile, a);
    stringstream s(a);
    s >> _numTargets;
    getline(inFile, b);
    stringstream s2(b) ;
    s2 >> _numSatellitesPerTarget;

    // create a satellite Map
    for (int i =0; i < _numTargets; i++){
        set<int> s;
        for (int j =1; j<= _numSatellitesPerTarget; j++)
        s.insert(j*100 +i);
        _satelliteMap.insert({i,s});
    }
    unordered_map<int,set<int> > _dummy;
    _dummy = _satelliteMap;
 
// continue reading the rest of the file
int y =0;
 while (getline(inFile, c))
 {
     stringstream s3(c);
// read the target coordinates first and create the map _targetCoords

if (y < _numTargets){
    y++;
    vector <string> tokens;
    while( getline(s3, c, '\t'))
       tokens.push_back(c);
    _grid = make_tuple(stod(tokens[1]),stod(tokens[2])) ;
    _targetCoords.insert({stoi(tokens[0]), _grid});
     
                    }
else{ 
    //continue reading the rest of the file after creating target coords and create the map _satelliteCoords  
    vector <string> tokens;
    while( getline(s3, c, '\t'))    
    tokens.push_back(c);

    
    _grid = make_tuple(stod(tokens[1]),stod(tokens[2])) ;
    unordered_map<int, set<int>>::const_iterator got = _dummy.find (stoi(tokens[0]));
    set<int> myset = got->second;
    _satelliteCoords.insert({*myset.begin() , _grid});
    myset.erase(*myset.begin());
    _dummy[stoi(tokens[0])] = myset;
    
    
    
   }
}
    inFile.close();  

}
