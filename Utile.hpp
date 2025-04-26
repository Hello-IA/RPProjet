#ifndef UTILE_HPP
#define UTILE_HPP

#include <vector>
#include <stack>
#include <set>

#include "Graphe.hpp"
#include "Edge.hpp"
#include "Noeud.hpp"
#include "Prim.hpp"
#include "DegreeGrapheImpair.hpp"
#include "Coupling.hpp"


using namespace std;
Graphe unionGraphes(Graphe, vector<Edge*>);

Graphe multigraph(Graphe u);

vector<int> findEulerianCycle(Graphe h);

vector<int> removeDuplicates(vector<int> path);

vector<int> christofides(Graphe g);

// Somme des poids des arêtes d'un chemin (non utilisée)
double sumPath(Graphe*, vector<int>);

// Variante de l'algorithme de PCC de Dijkstra qui n'emprunte pas les arêtes fermées
vector<int> shortestPathDijkstra(Graphe*, Noeud* , Noeud*, double*);

// Vérifie si un cycle renvoyé par CR ou CNN est valide
bool is_valid_cycle(Graphe, vector<int>);

#endif