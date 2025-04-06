#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"
#include "RandomkCCTP.hpp"
#include "Prim.hpp"
#include "DegreeGrapheImpair.hpp"
#include <iostream>
int main() {
    Graphe g = initKCCTP(15, 5, 0, 50, 50);
    Graphe t = prim(g, 0);
    Graphe i = DGI(g, t);
    g.display("output.png");
    t.display("tree.png");
    i.display("grapheImpair.png");
    cout << "oui"  << endl;

    return 0;
}
