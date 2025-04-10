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
		if(v->neighbor.size()!=0){
			Noeud* u = v->getNeighbor().front();
			Edge* e = h.getEdge(v->getName(), u->getName());
			auto findNeighbor_v = remove(v->neighbor.begin(), v->neighbor.end(), e);
			v->neighbor.erase(findNeighbor_v, v->neighbor.end());

			auto findNeighbor_u = remove(u->neighbor.begin(), u->neighbor.end(), e);
			u->neighbor.erase(findNeighbor_u, u->neighbor.end());

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
