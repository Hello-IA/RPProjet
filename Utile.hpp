#ifndef UTILE_HPP
#define UTILE_HPP

#include "Graphe.hpp"
#include <vector>
#include <stack>
#include <set>
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

#endif