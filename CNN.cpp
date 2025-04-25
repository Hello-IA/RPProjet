#include "CNN.hpp"

// Concaténation des deux chemins obtenus, renvoie le résultat final
vector<int> concatP1P2(vector<int> P1, vector<int> P2) {
    vector<int> P_prim;

    if (!P1.empty() && P1.back() == P1.front()) {
        P1.pop_back();
    }

    P_prim.insert(P_prim.end(), P1.begin(), P1.end());
    P_prim.insert(P_prim.end(), P2.begin(), P2.end());

    return P_prim;
}

// Parcours du cycle de Christofides en sautant les noeuds non accessibles en suivant celui-ci
vector<int> shortCut(Graphe g, vector<int> cycle, vector<int>* Us) {
    int i = 0;
    int j = 1;

    vector<int> resultat;
    resultat.push_back(cycle[0]);

    // Parcours du cycle
    while (j < g.noeuds.size()) {
        Edge* e = g.getEdge(cycle[i], cycle[j]);
        if (e && !e->close) {
            resultat.push_back(cycle[j]);
            i = j;
        } else {
            Us->push_back(cycle[j]);
        }
        j++;
    }

    // Ajout du noeud de départ à la fin
    Edge* e_end = g.getEdge(cycle[i], cycle[0]);
    if (e_end && !e_end->close) {
        resultat.push_back(cycle[0]);
    } else {
        // Si l'arête entre le noeud de fin et de départ est fermé, on retourne au noeud de départ en suivant le cycle à l'envers
        vector<int> resultat_reverse(resultat.rbegin(), resultat.rend());
        resultat.insert(resultat.end(), resultat_reverse.begin() + 1, resultat_reverse.end());
    }
    return resultat;
}

// La fonction compress calcule un graphe G' contenant le sommet de départ, les sommets non visités, les arêtes inconnues 
// et des arêtes virtuelles représentant les plus courts chemins dans le graphe initial entre les sommets
Graphe compress(vector<int> Us, Graphe G, unordered_map<Edge*, vector<int> >* detours) {

    // Construction de la liste des arêtes de nature inconnue
    unordered_set<int> Us_set(Us.begin(), Us.end());
    vector<Edge*> E_prim;
    for (Edge* e : G.edges) {
        vector<Noeud*> links = e->getLinks();
        if (Us_set.count(links[0]->getName()) && Us_set.count(links[1]->getName())) {
            E_prim.push_back(e);
        }
    }

    // Construction du graphe G'
    Graphe G_prim = Graphe(G, Us, E_prim);
    unordered_map<int, Noeud*> mapping_Gprim;
    for (Noeud* n : G_prim.noeuds) {
        mapping_Gprim[n->getName()] = n;
    }

    // Construction du graphe H composé des arêtes dont on connaît la nature
    unordered_set<Edge*> E_prim_set(E_prim.begin(), E_prim.end());
    vector<Edge*> E_without_E_prim;
    copy_if(G.edges.begin(), G.edges.end(), back_inserter(E_without_E_prim), [&E_prim_set](Edge* e){
        return !E_prim_set.count(e);
    });
    Graphe H = Graphe(G.noeuds, E_without_E_prim);

    // Pour chaque paire de sommets non visité (noeuds de G'),
    // on rajoute l'arête virtuelle représentant le plus court chemin entre les deux dans H
    for (int i = 0; i < Us.size(); i++) {
        for (int j=i+1; j < Us.size(); j++) {
            Noeud* vi = H.getNoeud(Us[i]);
            Noeud* vj = H.getNoeud(Us[j]);
            double c_ij;
            vector<int> P_ij = shortestPathDijkstra(&H, vi, vj, &c_ij);
            
            // Si l'arête originale est fermée, on ajoute le plus court chemin dans la map des détours
            Edge* e = G.getEdge(vi->getName(), vj->getName());
            if (e && e->close) {
                detours->insert({e, P_ij});
            }

            G_prim.addEdge(mapping_Gprim[Us[i]], mapping_Gprim[Us[j]], c_ij);
        }
    }
    
    return G_prim;
}

// Algorithme de parcours de graphe selon le voisin le plus proche
vector<int> nearestNeighbor(Graphe* G_prim, Graphe* g, unordered_map<Edge*, vector<int> >* detours) {
	vector<int> path;
	unordered_map<int, bool> visited;

    // Sommet de départ
	Noeud* u = G_prim->noeuds[0];
	int start = u->getName();
	path.push_back(start);
	visited[start] = true;
	size_t nbvisited = 1;

    // Tant qu'il reste des noeuds à visiter
	while(nbvisited < G_prim->noeuds.size()) {

		unordered_set<Edge*> edgeSet(G_prim->edges.begin(), G_prim->edges.end());

        // Parmi les arêtes de u dans le graphe initial, on ne garde que celle qui sont dans G'
		vector<Edge*> filtered_neighboringEdges;
		for (Edge* e : u->neighboringEdges) {
			if (edgeSet.count(e)) {
				filtered_neighboringEdges.push_back(e);
			}
		}

		vector<Edge*> voisins_non_visites;

		// Ensuite on ne garde que celles menant à un sommet non visité
		copy_if(filtered_neighboringEdges.begin(), filtered_neighboringEdges.end(), back_inserter(voisins_non_visites), [&u, &visited](Edge* e) {
            vector<Noeud*> links = e->getLinks();
            int u_id = u->getName();
            int v_id = (u_id == links[0]->getName()) ? links[1]->getName() : links[0]->getName();
            return !visited[v_id];
        });

		if (voisins_non_visites.empty()) {
			cerr << "Erreur : plus de voisins non visités accessibles depuis le sommet " << u->getName() << endl;
			break;
		}

        // On sélectionne l'arête restante la plus courte
		Edge* shortest = *min_element(voisins_non_visites.begin(), voisins_non_visites.end(), [](Edge* e1, Edge* e2) {
			return e1->getValue() < e2->getValue();
		});

        // Mise à jour du noeud courant
		vector<Noeud*> links = shortest->getLinks();
		u = (u->getName() == links[0]->getName()) ? links[1] : links[0];
        
        // Si l'arête sélectionnée est fermée dans le graphe initial, on la remplace dans le chemin par son détour valide
        Edge* edge_in_original = g->getEdge(links[0]->getName(), links[1]->getName());
        if (edge_in_original) {
            if (detours->count(edge_in_original) == 0) {
                path.push_back(u->getName());
            } else {
                vector<int> detour = detours->at(edge_in_original);
                if (path.back() == detour.front()) {
                    path.insert(path.end(), detour.begin()+1, detour.end());
                } else {
                    vector<int> detour_reverse(detour.rbegin(), detour.rend());
                    path.insert(path.end(), detour_reverse.begin()+1, detour_reverse.end());
                }
            }
        } else {
            cerr << "erreur: arête inexistante" << endl;
            exit(1);
        }

		visited[u->getName()] = true;
		nbvisited++;
	}

    // On clot le chemin en rajoutant le noeud de départ (ou le détour qui y mène depuis le dernier noeud du chemin)
    if (path.size() > 1) {
        Edge* edge_in_original = g->getEdge(path.back(), start);
        if (edge_in_original && edge_in_original->close) {
            vector<int> detour = detours->at(edge_in_original);
            if (path.back() == detour.front()) {
                path.insert(path.end(), detour.begin()+1, detour.end());
            } else {
                vector<int> detour_reverse(detour.rbegin(), detour.rend());
                path.insert(path.end(), detour_reverse.begin()+1, detour_reverse.end());
            }
        } else {
            path.push_back(start);
        }
    } 
	return path;
}

vector<int> compressAndExplore(Graphe G, vector<int> christo) {
    vector<int> U;
    vector<int> P1 = shortCut(G, christo, &U);

    if (U.empty()) return P1;
    U.insert(U.begin(), christo[0]);

    unordered_map<Edge*, vector<int> > detours;
    Graphe G_prim = compress(U, G, &detours);

    vector<int> P2 = nearestNeighbor(&G_prim, &G, &detours);
    vector<int> P_prim = concatP1P2(P1, P2);
    
    return P_prim;
}
