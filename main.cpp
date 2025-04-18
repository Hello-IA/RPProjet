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
    Graphe g;
    g.addNoeud(new Noeud(0));
    g.addNoeud(new Noeud(1));
    g.addNoeud(new Noeud(2));
    g.addNoeud(new Noeud(3));
    g.addNoeud(new Noeud(4));
    g.addNoeud(new Noeud(5));
    g.addNoeud(new Noeud(6));
    g.addNoeud(new Noeud(7));
    g.addNoeud(new Noeud(8));
    g.addNoeud(new Noeud(9));
    g.addNoeud(new Noeud(10));
    g.addNoeud(new Noeud(11));
    g.addNoeud(new Noeud(12));
    g.addNoeud(new Noeud(13));
    g.addNoeud(new Noeud(14));
    g.addNoeud(new Noeud(15));
    g.addEdge(g.getNoeud(0), g.getNoeud(1), 22.7325);
    g.addEdge(g.getNoeud(0), g.getNoeud(2), 41.3896);
    g.addEdge(g.getNoeud(0), g.getNoeud(3), 32.7372);
    g.addEdge(g.getNoeud(0), g.getNoeud(4), 19.4694);
    g.addEdge(g.getNoeud(0), g.getNoeud(5), 24.4577);
    g.addEdge(g.getNoeud(0), g.getNoeud(6), 39.9106);
    g.addEdge(g.getNoeud(0), g.getNoeud(7), 10.9484);
    g.addEdge(g.getNoeud(0), g.getNoeud(8), 18.2584);
    g.addEdge(g.getNoeud(0), g.getNoeud(9), 28.772);
    g.addEdge(g.getNoeud(0), g.getNoeud(10), 33.8873);
    g.addEdge(g.getNoeud(0), g.getNoeud(11), 4.4061);
    g.addEdge(g.getNoeud(0), g.getNoeud(12), 33.9695);
    g.addEdge(g.getNoeud(0), g.getNoeud(13), 37.8079);
    g.addEdge(g.getNoeud(0), g.getNoeud(14), 34.6092);
    g.addEdge(g.getNoeud(0), g.getNoeud(15), 33.0393);
    g.addEdge(g.getNoeud(1), g.getNoeud(2), 48.8555);
    g.addEdge(g.getNoeud(1), g.getNoeud(3), 13.9078);
    g.addEdge(g.getNoeud(1), g.getNoeud(4), 23.766);
    g.addEdge(g.getNoeud(1), g.getNoeud(5), 34.9961);
    g.addEdge(g.getNoeud(1), g.getNoeud(6), 17.5947);
    g.addEdge(g.getNoeud(1), g.getNoeud(7), 18.0328);
    g.addEdge(g.getNoeud(1), g.getNoeud(8), 4.53539);
    g.addEdge(g.getNoeud(1), g.getNoeud(9), 34.5898);
    g.addEdge(g.getNoeud(1), g.getNoeud(10), 18.1907);
    g.addEdge(g.getNoeud(1), g.getNoeud(11), 24.3154);
    g.addEdge(g.getNoeud(1), g.getNoeud(12), 45.534);
    g.addEdge(g.getNoeud(1), g.getNoeud(13), 42.6245);
    g.addEdge(g.getNoeud(1), g.getNoeud(14), 31.7898);
    g.addEdge(g.getNoeud(1), g.getNoeud(15), 43.9789);
    g.addEdge(g.getNoeud(2), g.getNoeud(3), 45.3376);
    g.addEdge(g.getNoeud(2), g.getNoeud(4), 25.552);
    g.addEdge(g.getNoeud(2), g.getNoeud(5), 16.939);
    g.addEdge(g.getNoeud(2), g.getNoeud(6), 56.2513);
    g.addEdge(g.getNoeud(2), g.getNoeud(7), 34.2895);
    g.addEdge(g.getNoeud(2), g.getNoeud(8), 46.0167);
    g.addEdge(g.getNoeud(2), g.getNoeud(9), 14.3275);
    g.addEdge(g.getNoeud(2), g.getNoeud(10), 40.8196);
    g.addEdge(g.getNoeud(2), g.getNoeud(11), 37.0768);
    g.addEdge(g.getNoeud(2), g.getNoeud(12), 10.0303);
    g.addEdge(g.getNoeud(2), g.getNoeud(13), 7.22709);
    g.addEdge(g.getNoeud(2), g.getNoeud(14), 21.5534);
    g.addEdge(g.getNoeud(2), g.getNoeud(15), 9.72872);
    g.addEdge(g.getNoeud(3), g.getNoeud(4), 24.3295);
    g.addEdge(g.getNoeud(3), g.getNoeud(5), 35.5341);
    g.addEdge(g.getNoeud(3), g.getNoeud(6), 10.9552);
    g.addEdge(g.getNoeud(3), g.getNoeud(7), 24.0061);
    g.addEdge(g.getNoeud(3), g.getNoeud(8), 16.436);
    g.addEdge(g.getNoeud(3), g.getNoeud(9), 32.3608);
    g.addEdge(g.getNoeud(3), g.getNoeud(10), 5.69515);
    g.addEdge(g.getNoeud(3), g.getNoeud(11), 32.6921);
    g.addEdge(g.getNoeud(3), g.getNoeud(12), 44.9528);
    g.addEdge(g.getNoeud(3), g.getNoeud(13), 38.2911);
    g.addEdge(g.getNoeud(3), g.getNoeud(14), 24.7579);
    g.addEdge(g.getNoeud(3), g.getNoeud(15), 43.2092);
    g.addEdge(g.getNoeud(4), g.getNoeud(5), 11.5519);
    g.addEdge(g.getNoeud(4), g.getNoeud(6), 34.8377);
    g.addEdge(g.getNoeud(4), g.getNoeud(7), 9.52622);
    g.addEdge(g.getNoeud(4), g.getNoeud(8), 20.5595);
    g.addEdge(g.getNoeud(4), g.getNoeud(9), 11.2719);
    g.addEdge(g.getNoeud(4), g.getNoeud(10), 22.0936);
    g.addEdge(g.getNoeud(4), g.getNoeud(11), 16.2207);
    g.addEdge(g.getNoeud(4), g.getNoeud(12), 21.8539);
    g.addEdge(g.getNoeud(4), g.getNoeud(13), 20.1382);
    g.addEdge(g.getNoeud(4), g.getNoeud(14), 15.2836);
    g.addEdge(g.getNoeud(4), g.getNoeud(15), 20.2537);
    g.addEdge(g.getNoeud(5), g.getNoeud(6), 46.2304);
    g.addEdge(g.getNoeud(5), g.getNoeud(7), 18.3142);
    g.addEdge(g.getNoeud(5), g.getNoeud(8), 31.4204);
    g.addEdge(g.getNoeud(5), g.getNoeud(9), 7.08758);
    g.addEdge(g.getNoeud(5), g.getNoeud(10), 32.6095);
    g.addEdge(g.getNoeud(5), g.getNoeud(11), 20.1378);
    g.addEdge(g.getNoeud(5), g.getNoeud(12), 10.6305);
    g.addEdge(g.getNoeud(5), g.getNoeud(13), 14.3603);
    g.addEdge(g.getNoeud(5), g.getNoeud(14), 19.0814);
    g.addEdge(g.getNoeud(5), g.getNoeud(15), 9.23116);
    g.addEdge(g.getNoeud(6), g.getNoeud(7), 32.9289);
    g.addEdge(g.getNoeud(6), g.getNoeud(8), 21.7878);
    g.addEdge(g.getNoeud(6), g.getNoeud(9), 43.3004);
    g.addEdge(g.getNoeud(6), g.getNoeud(10), 15.713);
    g.addEdge(g.getNoeud(6), g.getNoeud(11), 40.8107);
    g.addEdge(g.getNoeud(6), g.getNoeud(12), 55.8386);
    g.addEdge(g.getNoeud(6), g.getNoeud(13), 49.1683);
    g.addEdge(g.getNoeud(6), g.getNoeud(14), 35.3981);
    g.addEdge(g.getNoeud(6), g.getNoeud(15), 54.1036);
    g.addEdge(g.getNoeud(7), g.getNoeud(8), 13.8518);
    g.addEdge(g.getNoeud(7), g.getNoeud(9), 20.3657);
    g.addEdge(g.getNoeud(7), g.getNoeud(10), 24.0147);
    g.addEdge(g.getNoeud(7), g.getNoeud(11), 9.13841);
    g.addEdge(g.getNoeud(7), g.getNoeud(12), 28.8958);
    g.addEdge(g.getNoeud(7), g.getNoeud(13), 29.434);
    g.addEdge(g.getNoeud(7), g.getNoeud(14), 24.0173);
    g.addEdge(g.getNoeud(7), g.getNoeud(15), 27.545);
    g.addEdge(g.getNoeud(8), g.getNoeud(9), 31.6893);
    g.addEdge(g.getNoeud(8), g.getNoeud(10), 19.7037);
    g.addEdge(g.getNoeud(8), g.getNoeud(11), 19.7821);
    g.addEdge(g.getNoeud(8), g.getNoeud(12), 42.0333);
    g.addEdge(g.getNoeud(8), g.getNoeud(13), 40.1093);
    g.addEdge(g.getNoeud(8), g.getNoeud(14), 30.412);
    g.addEdge(g.getNoeud(8), g.getNoeud(15), 40.54);
    g.addEdge(g.getNoeud(9), g.getNoeud(10), 28.5671);
    g.addEdge(g.getNoeud(9), g.getNoeud(11), 24.7573);
    g.addEdge(g.getNoeud(9), g.getNoeud(12), 12.8245);
    g.addEdge(g.getNoeud(9), g.getNoeud(13), 9.12312);
    g.addEdge(g.getNoeud(9), g.getNoeud(14), 12.4522);
    g.addEdge(g.getNoeud(9), g.getNoeud(15), 11.0498);
    g.addEdge(g.getNoeud(10), g.getNoeud(11), 33.0947);
    g.addEdge(g.getNoeud(10), g.getNoeud(12), 41.3594);
    g.addEdge(g.getNoeud(10), g.getNoeud(13), 33.661);
    g.addEdge(g.getNoeud(10), g.getNoeud(14), 19.7064);
    g.addEdge(g.getNoeud(10), g.getNoeud(15), 39.5905);
    g.addEdge(g.getNoeud(11), g.getNoeud(12), 29.564);
    g.addEdge(g.getNoeud(11), g.getNoeud(13), 33.7092);
    g.addEdge(g.getNoeud(11), g.getNoeud(14), 31.5033);
    g.addEdge(g.getNoeud(11), g.getNoeud(15), 28.6423);
    g.addEdge(g.getNoeud(12), g.getNoeud(13), 12.6881);
    g.addEdge(g.getNoeud(12), g.getNoeud(14), 24.4097);
    g.addEdge(g.getNoeud(12), g.getNoeud(15), 1.77529);
    g.addEdge(g.getNoeud(13), g.getNoeud(14), 14.3275);
    g.addEdge(g.getNoeud(13), g.getNoeud(15), 11.3971);
    g.addEdge(g.getNoeud(14), g.getNoeud(15), 22.6868);
    g.closeParticularRoad(48);
    g.closeParticularRoad(97);
    g.closeParticularRoad(13);
    g.closeParticularRoad(112);
    g.closeParticularRoad(37);
    g.closeParticularRoad(99);
    g.closeParticularRoad(10);
    g.closeParticularRoad(77);
    g.closeParticularRoad(109);
    g.closeParticularRoad(79);
    g.closeParticularRoad(32);
    g.closeParticularRoad(16);
    g.closeParticularRoad(14);
    g.closeParticularRoad(108);

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
