#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"
#include "RandomkCCTP.hpp"
#include "Prim.hpp"
#include "DegreeGrapheImpair.hpp"
#include "Coupling.hpp"
#include "Utile.hpp"
#include <iostream>

int main() {
    Graphe g = initKCCTP(15, 5, 0, 50, 50);
    //Graphe g = initKCCTP(6, 0, 0, 20, 50);
    Graphe t = prim(g, 0);
    Graphe i = DGI(g, t);
    //vector<Edge*> c = coupling_glutton(&i);
    //cout << "coupling glouton ok" << endl;
    vector<Edge*> c_opt = coupling_perfect(&i);

    Graphe u = unionGraphes(t, c_opt);
    //cout << "coupling opt ok" << endl;
    //runEdmonds(&i);
    g.display("output.png");
    t.display("tree.png");
    i.display("grapheImpair.png");
    u.display("union.png");
    float value = 0;
    for (Edge* e: c_opt) {
        value += e->getValue();
        vector<Noeud*> links = e->getLinks();
        cout << "Matched: " << links[0]->getName() << " <-> " << links[1]->getName() << endl;
    }
    cout << "Matching Size: " << c_opt.size() << endl;
    cout << "Matching Value: " << value << endl;
    cout << "oui"  << endl;

    return 0;
}
