#ifndef NOEUD_HPP
#define NOEUD_HPP



#include <iostream>
#include<vector>
#include <String>
#include "Edge.hpp"

class Edge;

using namespace std;
class Noeud{

public:
	Noeud(int name);
	int getName();
	vector<Edge*> neighbor;
private:
	int name;
	
};


#endif 