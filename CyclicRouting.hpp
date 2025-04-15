#ifndef CYCLICROUTING_HPP
#define CYCLICROUTING_HPP

#include "Graphe.hpp"

#include <set>
#include "Edge.hpp"
#include "Noeud.hpp" 
#include <vector>
#include <iostream>   

using namespace std;


vector<int> cyclicRouting(Graphe, vector<int>);

vector<int> filtrerParcours(Graphe g, vector<int> Cyclic);

vector<vector<int>> cyclic(Graphe g, vector<int> Cyclic, vector<int> P1, bool &current_sens);

vector<int> lastCyclic(vector<vector<int>> result, Graphe g, vector<int> Cyclic, bool previousSens);

vector<int> contracte(const vector<vector<int>>& P, const vector<int>& P_end);

int trouverIndice(const vector<int>& liste, int valeur);

#endif
