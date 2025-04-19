#include "DegreeGrapheImpair.hpp"

Graphe DGI(Graphe g, Graphe t){
	Graphe I;
	for(Noeud* noeudt : t.noeuds){
		if(noeudt->neighboringEdges.size()%2 == 1){
			I.addNoeud(new Noeud(noeudt->getName()));
		}
	}

	for(int i = 0; i < (int)I.noeuds.size(); i++){
		for(int j =i+1; j < (int)I.noeuds.size(); j++){
			I.addEdge(I.getNoeud(i), I.getNoeud(j), g.getEdge(I.getNoeud(i)->getName(), I.getNoeud(j)->getName())->getValue());
		}
	}

	return I;
}
