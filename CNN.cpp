#include "CNN.hpp"

vector<int> filtrerParcours(Graphe* g, vector<int> cycle, vector<Edge*> closed_roads, vector<int> not_visited) {
    int i = 0;
    int j = 1;

    vector<int> resultat;

    while (j <= g->noeuds.size()) {
        Edge* e = g->getEdge(i, j);

        if (e && !e->close) {
            resultat.push_back(j);
            i = j;
        } else {
            not_visited.push_back(j);
            closed_roads.push_back(e);
            cout << "imposible de se rendre de "<< i << "au noeud" << j << endl;
        }
        j++;
    }

    Edge* e_end = g->getEdge(i, 0);
    if (e_end && !e_end->close) {
        resultat.push_back(0);
    } else {
        vector<int> resultat_reverse(resultat.rbegin(), resultat.rend());
        resultat.insert(resultat.end(), resultat_reverse.begin() + 1, resultat_reverse.end());
    }
    return resultat;
}

Graphe compress(Graphe* G_star, vector<int> Us, Graphe *G) {
    vector<Edge*> E_prim;
    for (Edge* e : G->edges) {
        vector<Noeud*> links = e->getLinks();
        int cnt_0 = count(Us.begin(), Us.end(), links[0]->getName());
        int cnt_1 = count(Us.begin(), Us.end(), links[1]->getName());
        if (cnt_0 > 0 && cnt_1 > 0) {
            E_prim.push_back(e);
        }
    }
    Graphe G_prim = Graphe(G, Us, E_prim);

    unordered_set<Edge*> E_prim_set(E_prim.begin(), E_prim.end());
    vector<Edge*> E_without_E_prim;
    copy_if(G->edges.begin(), G->edges.end(), back_inserter(E_without_E_prim), [&E_prim_set](Edge* e){
        return !E_prim_set.count(e);
    });

    Graphe H = Graphe(G->noeuds, E_without_E_prim);

    // TODO : vérifier les histoires d'indices/noeuds
    for (int i = 0; i < Us.size(); i++) {
        for (int j=i+1; j < Us.size(); j++) {
            // TODO : dijkstra
            Noeud* vi = H.getNoeud(Us[i]);
            Noeud* vj = H.getNoeud(Us[j]);
            vector<int> P_ij = shortestPathDijkstra(H, vi, vj);
            float c_ij = sumPath(&H, P_ij);

            G_prim.addEdge(G_prim.getNoeud(i), G_prim.getNoeud(j), c_ij);
        }
    }
    return G_prim;
}

vector<int> cnn(Graphe* g, vector<int> christo) {
    vector<Edge*> Eb;
    vector<int> Us;
    Us.push_back(christo[0]);

    vector<int> P1 = filtrerParcours(g, christo, Eb, Us);

    unordered_set<Edge*> Eb_set(Eb.begin(), Eb.end());
    vector<Edge*> E_without_Eb;
    copy_if(g->edges.begin(), g->edges.end(), back_inserter(E_without_Eb), [&Eb_set](Edge* e){
        return !Eb_set.count(e);
    });

    Graphe G_star = Graphe(g->noeuds, E_without_Eb);


    Graphe G_prim = compress(&G_star, Us, g);

    vector<int> P2 = nearestNeighbor(&G_prim);

    vector<int> P_prim = P1;
    P_prim.insert(P_prim.end(), P2.begin(), P2.end());

    return P_prim;
}
