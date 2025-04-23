#include "CNN.hpp"

vector<int> concatP1P2(vector<int> P1, vector<int> P2) {
    vector<int> P_prim;

    // Enlever le dernier 0 de P1 s’il existe (on fermera après P2)
    if (!P1.empty() && P1.back() == P1.front()) {
        P1.pop_back();
    }

    // Ajouter P1 à P'
    P_prim.insert(P_prim.end(), P1.begin(), P1.end());

    // Ajouter P2 à P'
    P_prim.insert(P_prim.end(), P2.begin(), P2.end());

    // Ajouter un retour à 0 si besoin
    if (!P_prim.empty() && P_prim.back() != 0) {
        P_prim.push_back(0);
    }

    return P_prim;
}


vector<int> shortCut(Graphe g, vector<int> cycle, /*vector<Edge*>* Eb,*/ vector<int>* Us) {
    int i = 0;
    int j = 1;

    vector<int> resultat;
    resultat.push_back(cycle[0]);

    while (j < g.noeuds.size()) {
        //cout << cycle[i] << " vers " << cycle[j] << " ? ";
        /*for (Noeud* n : g.noeuds) {
            if (n->getName() != cycle[i]) {
                Edge* e = g.getEdge(cycle[i], n->getName());
                if (e && e->close) {
                    Eb->push_back(e);
                }
            }
        }*/
        
        Edge* e = g.getEdge(cycle[i], cycle[j]);
        if (e && !e->close) {
            //cout << "oui !" << endl;
            resultat.push_back(cycle[j]);
            i = j;
        } else {
            //cout << "non..." << endl;
            Us->push_back(cycle[j]);
        }
        j++;
    }

    Edge* e_end = g.getEdge(cycle[i], cycle[0]);
    if (e_end && !e_end->close) {
        resultat.push_back(cycle[0]);
    } else {
        vector<int> resultat_reverse(resultat.rbegin(), resultat.rend());
        resultat.insert(resultat.end(), resultat_reverse.begin() + 1, resultat_reverse.end());
    }
    return resultat;
}

Graphe compress(/*Graphe* G_star,*/ vector<int> Us, Graphe G) {

    unordered_set<int> Us_set(Us.begin(), Us.end());
    vector<Edge*> E_prim;
    for (Edge* e : G.edges) {
        vector<Noeud*> links = e->getLinks();
        if (Us_set.count(links[0]->getName()) && Us_set.count(links[1]->getName())) {
            E_prim.push_back(e);
        }

    }
    Graphe G_prim = Graphe(G, Us, E_prim);
    unordered_map<int, Noeud*> mapping_Gprim;
    for (Noeud* n : G_prim.noeuds) {
        mapping_Gprim[n->getName()] = n;
    }


    unordered_set<Edge*> E_prim_set(E_prim.begin(), E_prim.end());
    vector<Edge*> E_without_E_prim;
    copy_if(G.edges.begin(), G.edges.end(), back_inserter(E_without_E_prim), [&E_prim_set](Edge* e){
        return !E_prim_set.count(e);
    });

    Graphe H = Graphe(G.noeuds, E_without_E_prim);

    for (int i = 0; i < Us.size(); i++) {
        for (int j=i+1; j < Us.size(); j++) {
            Noeud* vi = H.getNoeud(Us[i]);
            Noeud* vj = H.getNoeud(Us[j]);
            double c_ij;
            vector<int> P_ij = shortestPathDijkstra(&H, vi, vj, &c_ij);

            G_prim.addEdge(mapping_Gprim[Us[i]], mapping_Gprim[Us[j]], c_ij);
        }
    }
    
    return G_prim;
}

vector<int> nearestNeighbor(Graphe* g) {
	vector<int> path;
	unordered_map<int, bool> visited;

	cout << "nb de noeuds (nearestNeighbor) : " << g->noeuds.size() << endl;
	Noeud* u = g->noeuds[0];
	int start = u->getName();
	path.push_back(start);
	visited[start] = true;
	size_t nbvisited = 1;

	while(nbvisited < g->noeuds.size()) {

		unordered_set<Edge*> edgeSet(g->edges.begin(), g->edges.end());

		vector<Edge*> filtered_neighboringEdges;
		for (Edge* e : u->neighboringEdges) {
			if (edgeSet.count(e)) {
				filtered_neighboringEdges.push_back(e);
			}
		}

		vector<Edge*> voisins = filtered_neighboringEdges;
		vector<Edge*> voisins_ouverts;

		// Filtrer les arêtes fermées
		copy_if(voisins.begin(), voisins.end(), back_inserter(voisins_ouverts), [&u, &visited](Edge* e) {
            vector<Noeud*> links = e->getLinks();
            int u_id = u->getName();
            int v_id = (u_id == links[0]->getName()) ? links[1]->getName() : links[0]->getName();
            return !visited[v_id];
        });

		if (voisins_ouverts.empty()) {
			cerr << "Erreur : plus de voisins accessibles depuis le sommet " << u->getName() << endl;
			break; // ou return path;
		}

		Edge* shortest = *min_element(voisins_ouverts.begin(), voisins_ouverts.end(), [](Edge* e1, Edge* e2) {
			return e1->getValue() < e2->getValue();
		});

		vector<Noeud*> links = shortest->getLinks();
		u = (u->getName() == links[0]->getName()) ? links[1] : links[0];

		int u_id = u->getName();
		path.push_back(u_id);
		visited[u_id] = true;
		nbvisited++;
	}
	path.push_back(start);
	return path;
}

vector<int> cnn(Graphe G, vector<int> christo) {
    //vector<Edge*> Eb;
    vector<int> U;

    cout << "christofides (rappel) : ";
    for (int x : christo) {
        cout << x << " ";
    }
    cout << endl;

    vector<int> P1 = shortCut(G, christo, /*&Eb,*/ &U);

    cout << "P1 : ";
    for (int node : P1) {
        cout << node << " ";
    }
    cout << endl;

    if (U.empty()) return P1;

    /*
    unordered_set<Edge*> Eb_set(Eb.begin(), Eb.end());
    vector<Edge*> E_without_Eb;
    copy_if(G.edges.begin(), G.edges.end(), back_inserter(E_without_Eb), [&Eb_set](Edge* e){
        return !Eb_set.count(e);
    });
    */

    //Graphe G_star = Graphe(G.noeuds, E_without_Eb);
    /*
    cout << "G* (Noeuds de G, arêtes de G sans celles qu'on sait bloquées)" << endl;
    for (Noeud* n : G_star.noeuds) {
        cout << n->getName() << " ";
    }
    cout << endl;
    for (Edge* e : G_star.edges) {
        vector<Noeud*> links = e->getLinks();
        cout << links[0]->getName() << " <-> " << links[1]->getName() << endl;
    }*/

    
    Graphe G_prim = compress(/*&G_star,*/ U, G);
    //cout << "compress ok" << endl;
/*
    cout << "G' (Noeuds de G non visités, arêtes dont on ne connaît pas la nature)" << endl;
    for (Noeud* n : G_prim.noeuds) {
        cout << n->getName() << " ";
    }
    cout << endl;
    for (Edge* e : G_prim.edges) {
        vector<Noeud*> links = e->getLinks();
        cout << links[0]->getName() << " <-> " << links[1]->getName() << endl;
    }
*/
    vector<int> P2 = nearestNeighbor(&G_prim);

    cout << "P1 : ";
    for (int node : P1) {
        cout << node << " ";
    }
    cout << endl;

    cout << "P2 : ";
    for (int node : P2) {
        cout << node << " ";
    }
    cout << endl;

    vector<int> P_prim = concatP1P2(P1, P2);

    cout << "Cycle contracté final (CNN): ";
    for (int node : P_prim) {
        cout << node << " ";
    }
    cout << endl;

    return P_prim;
}
