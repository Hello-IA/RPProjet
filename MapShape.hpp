#ifndef MAP_SHAPE_HPP
#define MAP_SHAPE_HPP


#include <iostream>
#include<vector>
#include <String>
using namespace std;

class Noeud;
class Edge;

class Graphe{

public:
	Graphe();

	void addNoeud(Noeud* name);
	void display(string path);
	void addEdge(Noeud*, Noeud*, int value);
private:
	vector<Noeud*> noeuds;
	vector<Edge*> edges;
};

class Noeud{

public:
	Noeud(int name);
	int getName();
private:
	int name;
	vector<Edge*> edges;
};

class Edge{

public:
	Edge(Noeud*, Noeud*, int);
	int getValue();
	vector<Noeud*> getLinks();

private:
	int value;
	vector<Noeud*> links;

};

#endif // MAP_SHAPE_HPP