#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"
#include "RandomkCCTP.hpp"
#include "Prim.hpp"
#include "DegreeGrapheImpair.hpp"
#include "Coupling.hpp"
#include <iostream>

int main() {
    Graphe g = initKCCTP(15, 5, 0, 50, 50);
    //Graphe g = initKCCTP(6, 0, 0, 20, 50);
    cout << "init ok" << endl;
    Graphe t = prim(g, 0);
    cout << "prim ok" << endl;
    Graphe i = DGI(g, t);
    cout << "dgi ok" << endl;
    //vector<Edge*> c = coupling_glutton(&i);
    //cout << "coupling glouton ok" << endl;
    //vector<Edge*> c_opt = coupling_edmonds(&i);
    //cout << "coupling opt ok" << endl;
    runEdmonds(&i);
    g.display("output.png");
    t.display("tree.png");
    i.display("grapheImpair.png");
    cout << "oui"  << endl;

    cout << "test edmonds" << endl;

    Graphe g2;

    // Créer les noeuds
    for (int i = 0; i < 5; ++i) {
        g2.addNoeud(new Noeud(i));
    }

    // Ajouter les arêtes (suivant la structure au-dessus)
    g2.addEdge(g2.getNoeud(0), g2.getNoeud(1), 10.0);
    g2.addEdge(g2.getNoeud(1), g2.getNoeud(2), 2.0);
    g2.addEdge(g2.getNoeud(2), g2.getNoeud(3), 6.0);
    g2.addEdge(g2.getNoeud(3), g2.getNoeud(4), 8.0);
    g2.addEdge(g2.getNoeud(4), g2.getNoeud(0), 5.0);

    // Lancer l'algo d’Edmonds
    runEdmonds(&g2);

    cout << "test glutton" << endl;
    vector<Edge*> glutton = coupling_glutton(&g2);
    cout << "Matching size: " << glutton.size() << endl;
    for (Edge* e: glutton) {
        vector<Noeud*> links = e->getLinks();
        cout << "Matched: " << links[0]->getName() << " <-> " << links[1]->getName() << endl;
    }

    return 0;
}
