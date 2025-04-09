#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <string>
#include <sstream>
#include <stdio.h>
#include "PerfectMatching.h"
#include "GEOM/GeomPerfectMatching.h"
using namespace std;


string SaveMatching(int node_num, PerfectMatching* pm);
void LoadString(int& node_num, int& edge_num, int*& edges, int*& weights, const string& data);

string coupling(string data);

#endif