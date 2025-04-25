#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"
#include "RandomkCCTP.hpp"
#include "Prim.hpp"
#include "DegreeGrapheImpair.hpp"
#include "Coupling.hpp"
#include "CyclicRouting.hpp"
#include "CNN.hpp"
#include "Utile.hpp"
#include <iostream>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>


#include <iostream>
#include <vector>
#include <limits>

#include <fstream>

Noeud* getOtherNode(Edge* e, Noeud* current){
    return (e->getLinks()[0] == current) ? e->getLinks()[1] :e->getLinks()[0]; 
}
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
    
    vector<int> solve() {
        vector<int> currentPath;
        currentPath.push_back(startNode);
        visited[startNode] = true;
        
        // Lancer la recherche en profondeur
        dfsSearch(currentPath, 1, 0.0);
        
        return bestTour;
    }
    
private:
    void dfsSearch(vector<int>& currentPath, int visitedCount, double currentDistance) {
        int currentNode = currentPath.back();
        
        // Si tous les nœuds sont visités, vérifier le retour au point de départ
        if (visitedCount == numNodes) {
            Edge* returnEdge = g.getEdge(currentNode, startNode);
            if (returnEdge && !returnEdge->close) {
                double totalDistance = currentDistance + returnEdge->getValue();
                
                if (totalDistance < bestDistance) {
                    bestDistance = totalDistance;
                    bestTour = currentPath;
                    bestTour.push_back(startNode); // Ajouter le retour au point de départ
                }
            } else {
                // Si l'arête directe est fermée, chercher un chemin alternatif
                vector<int> returnPath = findShortestPath(currentNode, startNode);
                if (!returnPath.empty()) {
                    double pathDistance = calculatePathDistance(returnPath);
                    double totalDistance = currentDistance + pathDistance;
                    
                    if (totalDistance < bestDistance) {
                        bestDistance = totalDistance;
                        bestTour = currentPath;
                        // Ajouter le chemin de retour (sans le premier nœud qui est déjà le dernier de currentPath)
                        bestTour.insert(bestTour.end(), returnPath.begin() + 1, returnPath.end());
                    }
                }
            }
            return;
        }
        
        // Élagage: si la distance actuelle dépasse déjà la meilleure trouvée, abandonnez
        if (currentDistance >= bestDistance) {
            return;
        }
        
        // Essayer tous les nœuds non visités
        for (int nextNodeIdx = 0; nextNodeIdx < numNodes; nextNodeIdx++) {
            if (!visited[nextNodeIdx]) {
                // Essayer l'arête directe
                Edge* directEdge = g.getEdge(currentNode, nextNodeIdx);
                if (directEdge && !directEdge->close) {
                    // Ajouter ce nœud au chemin
                    currentPath.push_back(nextNodeIdx);
                    visited[nextNodeIdx] = true;
                    
                    // Continuer la recherche
                    dfsSearch(currentPath, visitedCount + 1, currentDistance + directEdge->getValue());
                    
                    // Backtrack
                    visited[nextNodeIdx] = false;
                    currentPath.pop_back();
                } else {
                    // Si l'arête directe est fermée, chercher des chemins alternatifs
                    vector<int> alternatePath = findShortestPath(currentNode, nextNodeIdx);
                    if (!alternatePath.empty()) {
                        // Calculer la distance du chemin alternatif
                        double pathDistance = calculatePathDistance(alternatePath);
                        
                        // Sauvegarder l'état actuel pour le backtracking
                        vector<int> pathBackup = currentPath;
                        
                        // Ajouter tous les nœuds intermédiaires (sauf le premier qui est déjà le dernier de currentPath)
                        for (size_t i = 1; i < alternatePath.size() - 1; i++) {
                            currentPath.push_back(alternatePath[i]);
                        }
                        currentPath.push_back(nextNodeIdx);
                        
                        // Marquer le prochain nœud comme visité
                        visited[nextNodeIdx] = true;
                        
                        // Continuer la recherche
                        dfsSearch(currentPath, visitedCount + 1, currentDistance + pathDistance);
                        
                        // Backtrack
                        visited[nextNodeIdx] = false;
                        currentPath = pathBackup;
                    }
                }
            }
        }
        
        // Optimisation: essayer également de passer par des nœuds déjà visités
        for (int intermediateNodeIdx = 0; intermediateNodeIdx < numNodes; intermediateNodeIdx++) {
            // Considérer seulement des nœuds déjà visités comme intermédiaires
            if (visited[intermediateNodeIdx] && intermediateNodeIdx != currentNode) {
                for (int nextNodeIdx = 0; nextNodeIdx < numNodes; nextNodeIdx++) {
                    // Aller vers un nœud non visité
                    if (!visited[nextNodeIdx]) {
                        Edge* edge1 = g.getEdge(currentNode, intermediateNodeIdx);
                        Edge* edge2 = g.getEdge(intermediateNodeIdx, nextNodeIdx);
                        
                        if (edge1 && edge2 && !edge1->close && !edge2->close) {
                            double indirectDist = edge1->getValue() + edge2->getValue();
                            
                            // Vérifier si le chemin indirect est meilleur qu'un éventuel chemin direct
                            Edge* directEdge = g.getEdge(currentNode, nextNodeIdx);
                            double directDist = (directEdge && !directEdge->close) ? 
                                            directEdge->getValue() : numeric_limits<double>::max();
                            
                            if (indirectDist < directDist) {
                                // Sauvegarder l'état actuel
                                vector<int> pathBackup = currentPath;
                                
                                // Ajouter les nœuds au chemin
                                currentPath.push_back(intermediateNodeIdx);  // Revisiter un nœud
                                currentPath.push_back(nextNodeIdx);
                                
                                // Marquer le nouveau nœud comme visité
                                visited[nextNodeIdx] = true;
                                
                                // Continuer la recherche
                                dfsSearch(currentPath, visitedCount + 1, currentDistance + indirectDist);
                                
                                // Backtrack
                                visited[nextNodeIdx] = false;
                                currentPath = pathBackup;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Trouve le plus court chemin entre deux nœuds en utilisant l'algorithme de Dijkstra
    vector<int> findShortestPath(int start, int end) {
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
            
            // Parcourir tous les voisins
            Noeud* currentNoeud = g.getNoeud(u);
            for (Edge* edge : currentNoeud->neighboringEdges) {
                if (edge->close) continue;
                
                vector<Noeud*> links = edge->getLinks();
                int v = -1;
                
                // Trouver l'autre extrémité de l'arête
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
        
        // Si pas de chemin trouvé
        if (prev[end] == -1) return {};
        
        // Reconstruire le chemin
        vector<int> path;
        for (int at = end; at != -1; at = prev[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        
        return path;
    }
    
    double calculatePathDistance(const vector<int>& path) {
        double distance = 0.0;
        for (size_t i = 0; i < path.size() - 1; i++) {
            Edge* edge = g.getEdge(path[i], path[i+1]);
            if (edge) {
                distance += edge->getValue();
            }
        }
        return distance;
    }
};

// Fonction principale à appeler avec votre Graphe
vector<int> findExactTSPSolution(Graphe& g, int startNode) {
    ExactTSPSolver solver(g, startNode);
    return solver.solve();
}

// Si vous avez déjà un tour initial comme point de départ
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
            //cout << "arret (" << path[i-1] << " " << path[i] << ")" << " a pour valeur : " << g.getEdge(path[i-1], path[i])->getValue() << ((g.getEdge(path[i-1], path[i])->close) ? "Close" : "Open") << endl;
        }
    }
    cout << ((valide) ? "Sais un chemain valide" : "se nais pas un chemain") << endl;
    return cost;
}

     
int main() {
    //SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    agseterr(AGMAX); 
    vector<double> list_opt;
    vector<double> list_cr;
    vector<double> list_cnn;
    vector<int> p1;
    vector<int> p2;
    vector<int> Christofides;
    Graphe g;
    double cout_opt = 0;
    double cout_cnn = 0;
    double cout_cr =0;
    for(int i = 0; i< 50; i++){
        cout<< "Tour : " << i << endl;
        g = initKCCTP(10, 8, 0, 50, i);
        //g.display("output.png");
        //Graphe g = initKCCTP(6, 0, 0, 20, 50);
        
        Christofides = christofides(g);
        //cout << "Christofides : ";
        /*
        for(int n: Christofides){
            cout << n << " ";   
        } */

        p1 = cyclicRouting(g, Christofides);
        cout_cr = pathCost(g, p1);
        cout << "CR : ";
        for(int i : p1) 
            cout << i << " ";
        cout << " cout de CR " << cout_cr << endl;

        list_cr.push_back(cout_cr);
        //cout << "hih"<< endl;

        
        p2 = compressAndExplore(g, Christofides);
        cout_cnn= pathCost(g, p2);
        cout << "CNN : ";
        for(int i : p2) 
            cout << i << " ";
        cout << " cout de CNN " << cout_cnn << endl;

        

        //cout << "haha" <<endl;
        list_cnn.push_back(cout_cnn);
        
        vector<int> cycle_opt = findExactTSPSolutionFromTour(g, Christofides);
        cout_opt = pathCost(g, cycle_opt);
        cout << "Cycle OPT : ";
        for(int c : cycle_opt) 
        cout << c << " ";
        cout << " cout de OPT " << cout_opt << endl;




        
        //list_opt.push_back(cout_opt);


        Christofides.clear();
        p1.clear();
        p2.clear();

    }
    string nomFichier = "approximation.txt";

    // Ouverture du fichier en mode écriture
    ofstream fichierSortie(nomFichier);
    if (!fichierSortie.is_open()) {
        cerr << "Erreur : impossible d'ouvrir le fichier " << nomFichier << endl;
        return 1;
    }
    cout << "la" << endl;


    // Écriture des éléments du vecteur dans le fichier
    fichierSortie << "OPT ";
    for (double element : list_opt) {
        fichierSortie << element << " ";
    }
    fichierSortie << "\n";

    fichierSortie << "CR ";
    for (double element : list_cr) {
        fichierSortie << element << " ";
    }
    fichierSortie << "\n";

    
    fichierSortie << "CNN ";
    for (double element : list_cnn) {
        fichierSortie << element << " ";
    }
    fichierSortie << "\n";

    // Fermeture du fichier
    fichierSortie.close();
    cout << "ici" << endl;

    return 0;
}
