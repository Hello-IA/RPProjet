#ifndef EXACTTSPSOLEVER_HPP
#define EXACTTSPSOLEVER_HPP


#include <vector>
#include <limits>
#include <queue>
#include <algorithm>
#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"

using namespace std;

class ExactTSPSolver {
private:
    Graphe& g;
    vector<int> bestTour;
    double bestDistance;
    vector<bool> visited;
    int startNode;
    int numNodes;
    
public:
    ExactTSPSolver(Graphe& graph, int start) : 
        g(graph), 
        startNode(start),
        numNodes(graph.noeuds.size()),
        visited(graph.noeuds.size(), false),
        bestDistance(numeric_limits<double>::max()) {
    }
    
    vector<int> solve();
private:
    void dfsSearch(vector<int>& currentPath, int visitedCount, double currentDistance);

    vector<int> findShortestPath(int start, int end);
    
    double calculatePathDistance(const vector<int>& path);
};

vector<int> findExactTSPSolution(Graphe& g, int startNode);

vector<int> findExactTSPSolutionFromTour(Graphe& g, vector<int>& initialTour);

double pathCost(Graphe g, vector<int> path);

#endif