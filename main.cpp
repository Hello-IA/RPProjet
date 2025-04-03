#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"
#include "RandomkCCTP.hpp"
int main() {
    cout << "oui"  << endl;
    Graphe g = initCTP(10, 0, 50, 0);
    g.display("output.png");
    return 0;
}
