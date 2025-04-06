#include "Prim.hpp"

Graphe prim(Graphe g, int s){
	int nb = g.noeuds.size();
	double coute[nb];
	int pred[nb];
	for(int i = 0; i < nb; i++){
		coute[i] = numeric_limits<double>::infinity();
		pred[i] = -1;
	}

	coute[s] = 0; 

	vector<int> F;
	for(int i =  0; i < nb; i++){
		F.push_back(i);
	}
	while(F.size() != 0){

		double valeMin = numeric_limits<double>::infinity();
		int u = -1;
		for(int i : F){
			if(valeMin > coute[i]){
				valeMin = coute[i];
				u = i;
			}
		}

		if(u != -1){
			
			auto it = find(F.begin(), F.end(), u);

			F.erase(it);
		}else{
			cout << "une erreur est survenus dans Prim ligne 31"<< endl;
		}
		

		vector<int> v = g.getNoeud(u)->getNeigborName();

		for(int neighbor: v){
			auto it = find(F.begin(), F.end(), neighbor);
			double w = g.getEdge(u, neighbor)->getValue();
			if(it != F.end() && w< coute[neighbor]){
				coute[neighbor] = w;
				pred[neighbor] = u;
			}
		}


	}
    Graphe t;

    Noeud* n = nullptr;
    for(int i = 0; i < nb; i++){
		n = new Noeud(i);
		t.addNoeud(n);
	}


	for (int i = 0; i < nb; i++) {
        int parentIndex = pred[i];
        if (parentIndex != -1) {
        	t.addEdge(t.getNoeud(parentIndex), t.getNoeud(i), g.getEdge(parentIndex, i)->getValue());
        }
    }
	return t;
}

