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
    Graphe g = initKCCTP(30, 15, 0, 50, 50);
    //Graphe g = initKCCTP(6, 0, 0, 20, 50);
    
    vector<int> Christofides = christofides(g);
    cout << "Christofides : ";

    for(int n: Christofides){
        cout << n << " ";
    }

    cout << "oui"  << endl;

    return 0;
}
