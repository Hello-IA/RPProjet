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
	vector<bool> visited(g->noeuds.size(), false);

	Noeud* u = g->noeuds[0];
	path.push_back(u->getName());
	visited[u->getName()] = true;
	int nbvisited = 1;

	while(nbvisited < g->noeuds.size()) {
		vector<Edge*> voisins = u->neighboringEdges;
		vector<Edge*> voisins_ouverts;

		copy_if(voisins.begin(), voisins.end(), back_inserter(voisins_ouverts), [&u, &visited](Edge* e) {
			vector<Noeud*> links = e->getLinks();
			if (u->getName() == links[0]->getName()) {
				return !visited[links[1]->getName()];
			}
			return !visited[links[0]->getName()];
		});
		Edge* shortest = *min_element(voisins_ouverts.begin(), voisins_ouverts.end(), [](Edge* e1, Edge* e2) {
			return e1->getValue() < e2->getValue();
		});

		vector<Noeud*> links = shortest->getLinks();
		if (u->getName() == links[0]->getName()) {
			u = links[1];
		} else {
			u = links[0];
		}

		path.push_back(u->getName());
		visited[u->getName()] = true;
		nbvisited++;
	}
	return path;
}

float sumPath(Graphe* g, vector<int> path) {
	float sum = 0;
	for (int i = 0; i < path.size() - 1; i++) {
		Edge* e = g->getEdge(path[i], path[i+1]);
		sum += e->getValue();
	}
	return sum;
}
