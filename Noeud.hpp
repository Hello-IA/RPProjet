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
	vector<Noeud*> getNeighbor();
	vector<int> getNeigborName();
	vector<Edge*> neighbor;
private:
	int name;
	
};


#endif 