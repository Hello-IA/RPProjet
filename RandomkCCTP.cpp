#include "RandomkCCTP.hpp"

vector<pair<double, double> > points;

Graphe initCTP(int nbNoeud, int min, int max, int seed){
	mt19937 generator(seed); 
	Noeud* n = nullptr;
	Graphe g;
	std::uniform_real_distribution<double> distribution(min, max);
	for(int i = 0; i < nbNoeud; i++){
		n = new Noeud(i);
		g.addNoeud(n);
	}

	for(int i = 0; i < nbNoeud; i++){
		pair<double, double> p(distribution(generator), distribution(generator));
		points.push_back(p);
	}
	for(int i = 0; i < nbNoeud; i++){
		for(int j = i; j < nbNoeud; j++){
			if (i != j){
				double distance = sqrt(pow(points[i].first  - points[j].first , 2) + pow(points[i].second  - points[j].second , 2));
				g.addEdge(g.getNoeud(i), g.getNoeud(j), distance);
			}
		}
	}
	return g;
}

Graphe initKCCTP(int nbNoeud, int k, int min, int max, int seed){
	Graphe g =  initCTP(nbNoeud, min, max, seed);
	g.closeRoad(k, seed);
	return g;
}