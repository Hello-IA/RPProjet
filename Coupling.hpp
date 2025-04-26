#ifndef COUPLING
#define COUPLING

#include <unordered_set>
#include<unordered_map>
#include<queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>

#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"
#include "blossom5/example.h"

// Fonction de couplage utilisée pour comparaison
vector<Edge*> coupling_glutton(Graphe*);

// Fonction de couplage utilisée pour Christofides, utilise un algorithme extérieur (voir rapport)
vector<Edge*> coupling_perfect(Graphe*);


//void runEdmonds(Graphe*);

#endif