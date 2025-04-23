#include "Graphe.hpp"
#include "Noeud.hpp"
#include "Edge.hpp"
#include "RandomkCCTP.hpp"
#include "Prim.hpp"
#include "DegreeGrapheImpair.hpp"
#include "Coupling.hpp"
#include "CyclicRouting.hpp"
#include "CNN.hpp"
#include "Utile.hpp"
#include <iostream>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
//#include <windows.h>

int main() {
    //SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    agseterr(AGMAX);
    //Graphe g = initKCCTP(16, 14, 3, 50, 50);
    Graphe g = initKCCTP(10, 8, 3, 50, 1);
    g.display("output.png");
    
    vector<int> Christofides = christofides(g);
    cout << "Christofides : ";

    for(int n: Christofides){
        cout << n << " ";   
    } 

    cout << endl;
    vector<int> p1 = cyclicRouting(g, Christofides);
    cout << "CR " << ((is_valid_cycle(g, p1)) ? "" : "pas ") << "ok" << endl;
    vector<int> p2 = cnn(g, Christofides);
    cout << "CNN " << ((is_valid_cycle(g, p2)) ? "" : "pas ") << "ok" << endl;
    cout << "oui"  << endl;

    return 0;
}
