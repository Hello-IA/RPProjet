#ifndef RANDOMKCCTP_HPP
#define RANDOMKCCTP_HPP

#include "Graphe.hpp"
#include "Noeud.hpp"

#include <cstdlib> 
#include <ctime>  
#include <random>
#include <vector>
#include <utility> 
#include <cmath>
using namespace std;

class Graphe;
class Noeud;
vector<pair<double, double>> points;
Graphe initCTP(int nbNoeud, int min, int max, int seed = 0);
Graphe initKCCTP(int nbNoeud, int k, int min, int max, int seed);
#endif 