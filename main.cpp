#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"
#include "RandomkCCTP.hpp"

int main() {
    cout << "oui"  << endl;
    Graphe g = initKCCTP(10, 5, 0, 50, 50);
    g.display("output.png");
    return 0;
}
