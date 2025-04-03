#ifndef GRAPHE_HPP
#define GRAPHE_HPP


#include <graphviz/gvc.h>
#include <iostream>
#include<vector>
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
private:
	vector<Noeud*> noeuds;
	vector<Edge*> edges;
};

#endif 