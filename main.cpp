#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"
#include "RandomkCCTP.hpp"
#include "Prim.hpp"
#include "DegreeGrapheImpair.hpp"
#include "Coupling.hpp"
#include "CyclicRouting.hpp"
#include "Utile.hpp"
#include <iostream>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include <windows.h>

int main() {
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    agseterr(AGMAX);
    Graphe g = initKCCTP(16, 14, 3, 50, 50);
    g.display("output.png");
    //Graphe g = initKCCTP(6, 0, 0, 20, 50);
    
    vector<int> Christofides = christofides(g);
    cout << "Christofides : ";

    
    for(int n: Christofides){
        cout << n << " ";
    } 
    cout << endl;
    vector<int> p1 = cyclicRouting(g, Christofides);
    cout << "oui"  << endl;

    return 0;
}
