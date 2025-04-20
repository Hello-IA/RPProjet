#include "Utile.hpp"

Graphe unionGraphes(Graphe t, vector<Edge*> edges){
	int nb = t.noeuds.size();
	int nbe = t.edges.size();
	Graphe g;

	for(int i = 0; i < nb; i++){
		g.addNoeud(new Noeud(i));
	}

	for(int i = 0; i < nbe; i++){
		g.addEdge(g.getNoeud(t.edges[i]->getLinks()[0]->getName()), g.getNoeud(t.edges[i]->getLinks()[1]->getName()), t.edges[i]->getValue());
	}


	for(Edge* e : edges){
		int n1 = e->getLinks()[0]->getName();
		int n2 = e->getLinks()[1]->getName();
		if(!g.getEdge(n1, n2)){
			g.addEdge(g.getNoeud(n1), g.getNoeud(n2), e->getValue());
		}
	}
	return g;
}

Graphe multigraph(Graphe u){
	int nb = u.noeuds.size();
	int nbe = u.edges.size();
	Graphe h;

	for(int i = 0; i < nb; i++){
		h.addNoeud(new Noeud(i));
	}

	for(int i = 0; i < nbe; i++){
		int n1 = u.edges[i]->getLinks()[0]->getName();
		int n2 = u.edges[i]->getLinks()[1]->getName();
		h.addEdge(h.getNoeud(n1), h.getNoeud(n2), u.edges[i]->getValue());
		h.addEdge(h.getNoeud(n1), h.getNoeud(n2), u.edges[i]->getValue());
	}

	return h;
}

vector<int> findEulerianCycle(Graphe h){
	vector<Noeud*> tour;
	stack<Noeud*> pile;
	vector<int> path;

	Noeud* s = h.getNoeud(0);

	pile.push(s);

	while (!pile.empty()){
		Noeud* v = pile.top();
		if(v->neighboringEdges.size()!=0){
			Noeud* u = v->getNeighbors().front();
			Edge* e = h.getEdge(v->getName(), u->getName());
			auto findNeighbor_v = remove(v->neighboringEdges.begin(), v->neighboringEdges.end(), e);
			v->neighboringEdges.erase(findNeighbor_v, v->neighboringEdges.end());

			auto findNeighbor_u = remove(u->neighboringEdges.begin(), u->neighboringEdges.end(), e);
			u->neighboringEdges.erase(findNeighbor_u, u->neighboringEdges.end());

			auto findEdge_h = remove(h.edges.begin(), h.edges.end(), e);
			h.edges.erase(findEdge_h, h.edges.end());
			pile.push(u);
		}else{
			pile.pop();
			tour.push_back(v);
		}
	}
	for(Noeud* t: tour){
		path.push_back(t->getName());
	}
	return path;
}

vector<int> removeDuplicates(vector<int> path){
	set<int> ensemble;
	vector<int> hamiltonian_cycle;
	for(int p : path){
		if(ensemble.find(p) == ensemble.end()){
			hamiltonian_cycle.push_back(p);
			ensemble.insert(p);
		}
	}
	hamiltonian_cycle.push_back(path[0]);
	return hamiltonian_cycle;
}


vector<int> christofides(Graphe g){
	Graphe t = prim(g, 0);
    Graphe i = DGI(g, t);
    vector<Edge*> c_opt = coupling_perfect(&i);

    Graphe u = unionGraphes(t, c_opt);

    Graphe h = multigraph(u);

    vector<int> e = findEulerianCycle(h);

    vector<int> c = removeDuplicates(e);
    return c;
}

vector<int> nearestNeighbor(Graphe* g) {
	vector<int> path;
	unordered_map<int, bool> visited;

	//cout << "nb de noeuds (nearestNeighbor) : " << g->noeuds.size() << endl;
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

double sumPath(Graphe* g, vector<int> path) {
	cout << "enter sumPath" << endl;
	double sum = 0;
	for (size_t i = 0; i < path.size() - 1; i++) {
		cout << i << endl;
		Edge* e = g->getEdge(path[i], path[i+1]);
		cout << "getEdge ok " << e->getValue() << endl;
		sum += e->getValue();
	}
	cout << "sumPath ok" << endl;
	return sum;
}

struct CompareDist {
    bool operator()(const std::pair<int, double>& a, const std::pair<int, double>& b) const {
        if (a.second != b.second)
            return a.second < b.second;
        return a.first < b.first;
    }
};

vector<int> shortestPathDijkstra(Graphe* g, Noeud* s, Noeud* t, double* sum) {
	//cout << "enter dijkstra" << endl;
	unordered_map<int, pair<double, int> > distances_pred;
	for (Noeud* n : g->noeuds) {
		pair<double, int> paire (HUGE_VALF, -1);
		distances_pred.insert({n->getName(), paire});
	}
	distances_pred[s->getName()].first = 0;

	set<pair<int, double>, CompareDist> Q;

	for (const auto &n : distances_pred) {
		Q.insert({n.first, n.second.first});
	}

	while (!Q.empty()) {
		auto it = Q.begin();
		Q.erase(it);
		Noeud *u = g->getNoeud(it->first);

		unordered_set<Edge*> edgeSet(g->edges.begin(), g->edges.end());

		vector<Edge*> filtered_neighboringEdges;
		for (Edge* e : u->neighboringEdges) {
			if (edgeSet.count(e)) {
				filtered_neighboringEdges.push_back(e);
			}
		}

		for (Edge* e : filtered_neighboringEdges) {
			vector<Noeud*> links = e->getLinks();
			Noeud* v = (u->getName() == links[0]->getName()) ? links[1] : links[0];

			double d_v = distances_pred[v->getName()].first;
			double d_u = distances_pred[u->getName()].first;
			if (d_v > d_u + e->getValue()) {
				double new_d_v = d_u + e->getValue();
				distances_pred[v->getName()].first = new_d_v;
				distances_pred[v->getName()].second = u->getName();

				Q.erase({v->getName(), d_v});
				Q.insert({v->getName(), new_d_v});
			}
		}
	}

	int temp = t->getName();
	vector<int> path;

	if (distances_pred[temp].first == HUGE_VALF) {
		return {}; // Pas de chemin trouvé
	}

	//cout << "dijkstra ok jusqu'à la recup du path" << endl;
	while(temp != s->getName()) {
		path.insert(path.begin(), temp);
		int next = distances_pred[temp].second;
		Edge* e = g->getEdge(temp, next);
		if (e == nullptr) {
			cerr << "Erreur : arête manquante entre " << temp << " et " << next << endl;
			exit(1);  // ou throw, ou continue, selon ton besoin
		}
		*sum += e->getValue();

		//cout << "getEdge ok" << endl;
		temp = next;
	}
	path.insert(path.begin(), temp);

	//cout << "dijkstra ok, taille du path : " << path.size() << ", poids du path : " << *sum << endl;
	return path;
}
