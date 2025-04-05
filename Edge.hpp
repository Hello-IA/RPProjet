#ifndef EDGE_HPP
#define EDGE_HPP



#include <iostream>
#include<vector>
#include <String>
#include "Noeud.hpp"

class Noeud;

using namespace std;
class Edge{

public:
	Edge(Noeud*, Noeud*, double);
	double getValue();
	vector<Noeud*> getLinks();
	bool close;
private:
	double value;
	vector<Noeud*> links;

};

#endif 