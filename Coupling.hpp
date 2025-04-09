#ifndef COUPLING
#define COUPLING

#include <unordered_set>
#include<unordered_map>
#include<queue>
#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>
#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"

#include <vector>
#include <cstring>


vector<Edge*> coupling_glutton(Graphe*);
vector<Edge*> coupling_perfect(Graphe*);

//void runEdmonds(Graphe*);

#endif