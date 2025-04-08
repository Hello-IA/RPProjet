#ifndef GRAPHE_HPP
#define GRAPHE_HPP


#include <graphviz/gvc.h>
#include <iostream>
#include<vector>
#include <random>
#include <algorithm>
#include <string>
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
	Edge* getEdge(int n1, int n2);
	void closeRoad(int k, int seed);
	vector<int> DistAleatoires(int k, int n, int seed);
	vector<Noeud*> noeuds;
	vector<Edge*> edges;

};

#endif 