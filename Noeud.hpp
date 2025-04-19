#ifndef NOEUD_HPP
#define NOEUD_HPP



#include <iostream>
#include<vector>
#include <string>
#include "Edge.hpp"

class Edge;

using namespace std;
class Noeud{

public:
	Noeud(int name);
	int getName();
	vector<Noeud*> getNeighbors();
	vector<int> getNeighborsNames();
	vector<Edge*> neighboringEdges;
private:
	int name;
	
};


#endif 