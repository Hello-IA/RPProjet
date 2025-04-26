#include "ExactTSPSolver.hpp"   

Noeud* getOtherNode(Edge* e, Noeud* current){
    return (e->getLinks()[0] == current) ? e->getLinks()[1] :e->getLinks()[0]; 
}

vector<int> ExactTSPSolver::solve() {
    vector<int> currentPath;
    currentPath.push_back(startNode);
    visited[startNode] = true;
    
    dfsSearch(currentPath, 1, 0.0);
    
    return bestTour;
}

void ExactTSPSolver::dfsSearch(vector<int>& currentPath, int visitedCount, double currentDistance) {
    int currentNode = currentPath.back();
    
    if (visitedCount == numNodes) {
        Edge* returnEdge = g.getEdge(currentNode, startNode);
        if (returnEdge && !returnEdge->close) {
            double totalDistance = currentDistance + returnEdge->getValue();
            
            if (totalDistance < bestDistance) {
                bestDistance = totalDistance;
                bestTour = currentPath;
                bestTour.push_back(startNode); 
            }
        } else {
            vector<int> returnPath = findShortestPath(currentNode, startNode);
            if (!returnPath.empty()) {
                double pathDistance = calculatePathDistance(returnPath);
                double totalDistance = currentDistance + pathDistance;
                
                if (totalDistance < bestDistance) {
                    bestDistance = totalDistance;
                    bestTour = currentPath;
                    bestTour.insert(bestTour.end(), returnPath.begin() + 1, returnPath.end());
                }
            }
        }
        return;
    }
    
    if (currentDistance >= bestDistance) {
        return;
    }
    
    for (int nextNodeIdx = 0; nextNodeIdx < numNodes; nextNodeIdx++) {
        if (!visited[nextNodeIdx]) {
            Edge* directEdge = g.getEdge(currentNode, nextNodeIdx);
            if (directEdge && !directEdge->close) {
                currentPath.push_back(nextNodeIdx);
                visited[nextNodeIdx] = true;
                
                dfsSearch(currentPath, visitedCount + 1, currentDistance + directEdge->getValue());
                
                visited[nextNodeIdx] = false;
                currentPath.pop_back();
            } else {
                vector<int> alternatePath = findShortestPath(currentNode, nextNodeIdx);
                if (!alternatePath.empty()) {
                    double pathDistance = calculatePathDistance(alternatePath);
                    
                    vector<int> pathBackup = currentPath;
                    
                    for (size_t i = 1; i < alternatePath.size() - 1; i++) {
                        currentPath.push_back(alternatePath[i]);
                    }
                    currentPath.push_back(nextNodeIdx);
                    
                    visited[nextNodeIdx] = true;
                    
                    dfsSearch(currentPath, visitedCount + 1, currentDistance + pathDistance);
                    
                    visited[nextNodeIdx] = false;
                    currentPath = pathBackup;
                }
            }
        }
    }
    
    for (int intermediateNodeIdx = 0; intermediateNodeIdx < numNodes; intermediateNodeIdx++) {
        if (visited[intermediateNodeIdx] && intermediateNodeIdx != currentNode) {
            for (int nextNodeIdx = 0; nextNodeIdx < numNodes; nextNodeIdx++) {
                if (!visited[nextNodeIdx]) {
                    Edge* edge1 = g.getEdge(currentNode, intermediateNodeIdx);
                    Edge* edge2 = g.getEdge(intermediateNodeIdx, nextNodeIdx);
                    
                    if (edge1 && edge2 && !edge1->close && !edge2->close) {
                        double indirectDist = edge1->getValue() + edge2->getValue();
                        
                        Edge* directEdge = g.getEdge(currentNode, nextNodeIdx);
                        double directDist = (directEdge && !directEdge->close) ? 
                                        directEdge->getValue() : numeric_limits<double>::max();
                        
                        if (indirectDist < directDist) {

                            vector<int> pathBackup = currentPath;
                            
                            currentPath.push_back(intermediateNodeIdx); 
                            currentPath.push_back(nextNodeIdx);
                            

                            visited[nextNodeIdx] = true;
                            
                            dfsSearch(currentPath, visitedCount + 1, currentDistance + indirectDist);
                            
                            visited[nextNodeIdx] = false;
                            currentPath = pathBackup;
                        }
                    }
                }
            }
        }
    }
}


vector<int> ExactTSPSolver::findShortestPath(int start, int end) {
    vector<double> dist(numNodes, numeric_limits<double>::max());
    vector<int> prev(numNodes, -1);
    vector<bool> visited(numNodes, false);
    
    dist[start] = 0;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({0, start});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        if (u == end) break;
        
        Noeud* currentNoeud = g.getNoeud(u);
        for (Edge* edge : currentNoeud->neighboringEdges) {
            if (edge->close) continue;
            
            vector<Noeud*> links = edge->getLinks();
            int v = -1;
            
            if (links[0]->getName() == u) {
                v = links[1]->getName();
            } else {
                v = links[0]->getName();
            }
            
            double alt = dist[u] + edge->getValue();
            
            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                pq.push({alt, v});
            }
        }
    }
    
    if (prev[end] == -1) return {};
    
    vector<int> path;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    
    return path;
}

double ExactTSPSolver::calculatePathDistance(const vector<int>& path) {
    double distance = 0.0;
    for (size_t i = 0; i < path.size() - 1; i++) {
        Edge* edge = g.getEdge(path[i], path[i+1]);
        if (edge) {
            distance += edge->getValue();
        }
    }
    return distance;
}


vector<int> findExactTSPSolution(Graphe& g, int startNode) {
    ExactTSPSolver solver(g, startNode);
    return solver.solve();
}


vector<int> findExactTSPSolutionFromTour(Graphe& g, vector<int>& initialTour) {
    int startNode = initialTour[0];
    ExactTSPSolver solver(g, startNode);
    return solver.solve();
}




double pathCost(Graphe g, vector<int> path){
    double cost = 0;
    bool valide = true;
    for(size_t i = 1; i < path.size(); i++){
        if(g.getEdge(path[i-1], path[i])){
            cost += g.getEdge(path[i-1], path[i])->getValue();
            valide &= !g.getEdge(path[i-1], path[i])->close;
        }
    }
    return cost;
}

