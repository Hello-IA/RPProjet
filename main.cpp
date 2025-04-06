#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"
#include "RandomkCCTP.hpp"
#include "Prim.hpp"

int main() {
    cout << "oui"  << endl;
    Graphe g = initKCCTP(15, 5, 0, 50, 50);
    Graphe t = prim(g, 0);
    g.display("output.png");
    t.display("tree.png");
    return 0;
}
