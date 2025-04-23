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
bool isHamiltonianCycle(const std::vector<Noeud*>& path, const Graphe& graphe) {
    for (size_t i = 0; i < path.size(); ++i) {
        Noeud* current = path[i];
        Noeud* next = path[(i + 1) % path.size()];
        bool connected = false;
        for (Edge* edge : current->neighboringEdges) {
            if (getOtherNode(edge, current) == next && !edge->close) {
                connected = true;
                break;
            }
        }
        if (!connected) {
            return false;
        }
    }
    return true;
}


double findHamiltonianCycle(Graphe& graphe) {
    vector<Noeud*> noeuds = graphe.noeuds;
    vector<Noeud*> bestPath;
    double minCost = std::numeric_limits<double>::infinity();

    do {
        if (isHamiltonianCycle(noeuds, graphe)) {
            double currentCost = 0.0;
            for (size_t i = 0; i < noeuds.size(); ++i) {
                Noeud* current = noeuds[i];
                Noeud* next = noeuds[(i + 1) % noeuds.size()];
                for (Edge* edge : current->neighboringEdges) {
                    if (getOtherNode(edge, current) == next) {
                        currentCost += edge->getValue();
                        break;
                    }
                }
            }
            if (currentCost < minCost) {
                minCost = currentCost;
                bestPath = noeuds;
            }
        }
    } while (next_permutation(noeuds.begin(), noeuds.end(),
                                   [](Noeud* a, Noeud* b) { return a->getName() < b->getName(); }));
    return minCost;
    /*
    if (minCost == numeric_limits<double>::infinity()) {
        cout << "Aucun cycle hamiltonien valide trouvé." << endl;
    } else {
        cout << "Cycle hamiltonien de coût minimal : " << minCost << endl;
        cout << "Chemin : ";
        for (Noeud* noeud : bestPath) {
            cout << noeud->getName() << " ";
        }
        cout << bestPath.front()->getName() << endl; // Retour au point de départ
    }*/
}

double pathCost(Graphe g, vector<int> path){
    double cost = 0;
    for(size_t i = 1; i < path.size(); i++){
        if(g.getEdge(path[i-1], path[i]))
            cost += g.getEdge(path[i-1], path[i])->getValue();
    }
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
    for(size_t i = 1; i< 2; i++){
        cout<< "Tour : " << i << endl;
        g = initKCCTP(10, 8, 50, 50, (int)i);
        //g.display("output.png");
        //Graphe g = initKCCTP(6, 0, 0, 20, 50);
        
        Christofides = christofides(g);
        //cout << "Christofides : ";
        /*
        for(int n: Christofides){
            cout << n << " ";   
        } */

        p1 = cyclicRouting(g, Christofides);
        list_cr.push_back(pathCost(g, p1));
        cout << "hih"<< endl;
        g = initKCCTP(10, 8, 50, 50, (int)i);     
        Christofides = christofides(g);

        
        
        p2 = cnn(&g, Christofides);
        cout << "haha" <<endl;
        list_cnn.push_back(pathCost(g, p2));


        list_opt.push_back(findHamiltonianCycle(g));
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
    for (int element : list_opt) {
        fichierSortie << element << " ";
    }
    fichierSortie << "\n";

    fichierSortie << "CR ";
    for (int element : list_cr) {
        fichierSortie << element << " ";
    }
    fichierSortie << "\n";


    fichierSortie << "CNN ";
    for (int element : list_cnn) {
        fichierSortie << element << " ";
    }
    fichierSortie << "\n";

    // Fermeture du fichier
    fichierSortie.close();
    cout << "ici" << endl;

    return 0;
}
