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