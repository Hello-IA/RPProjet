#ifndef GRAPHE_HPP
#define GRAPHE_HPP


#include <graphviz/gvc.h>
#include <iostream>
#include<vector>
#include <random>
#include <algorithm>
#include <String>
#include "Edge.hpp"
#include "Noeud.hpp"
using namespace std;

class Noeud;
class Edge;

class Graphe{

public:
	Graphe();

	void addNoeud(Noeud* name);
	void display(string path);
	void addEdge(Noeud*, Noeud*, double value);
	Noeud* getNoeud(int nb);
	void closeRoad(int k, int seed);
	vector<int> DistAleatoires(int k, int n, int seed);
private:
	vector<Noeud*> noeuds;
	vector<Edge*> edges;
};

#endif 