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
#include "ExactTSPSolver.hpp"
#include <iostream>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>


#include <iostream>
#include <vector>
#include <limits>

#include <fstream>



     
int main() {
    //SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    //agseterr(AGMAX); 
    vector<double> list_opt;
    vector<double> list_cr;
    vector<double> list_cnn;
    vector<int> p1;
    vector<int> p2;
    vector<int> Christofides;
    Graphe g;
    double cout_opt = 0;
    double cout_cnn = 0;
    double cout_cr =0;
    for(int i = 0; i< 50; i++){
        cout<< "Tour : " << i << endl;
        g = initKCCTP(10, 8, 0, 50, i);
        //g.display("output.png");
        //Graphe g = initKCCTP(6, 0, 0, 20, 50);
        
        Christofides = christofides(g);
        //cout << "Christofides : ";
        /*
        for(int n: Christofides){
            cout << n << " ";   
        } */

        p1 = cyclicRouting(g, Christofides);
        cout_cr = pathCost(g, p1);
        cout << "CR : ";
        for(int i : p1) 
            cout << i << " ";
        cout << " cout de CR " << cout_cr << endl;

        list_cr.push_back(cout_cr);
        //cout << "hih"<< endl;

        
        p2 = cnn(g, Christofides);
        cout_cnn= pathCost(g, p2);
        cout << "CNN : ";
        for(int i : p2) 
            cout << i << " ";
        cout << " cout de CNN " << cout_cnn << endl;

        

        //cout << "haha" <<endl;
        list_cnn.push_back(cout_cnn);
        
        vector<int> cycle_opt = findExactTSPSolutionFromTour(g, Christofides);
        cout_opt = pathCost(g, cycle_opt);
        cout << "Cycle OPT : ";
        for(int c : cycle_opt) 
        cout << c << " ";
        cout << " cout de OPT " << cout_opt << endl;




        
        //list_opt.push_back(cout_opt);


        Christofides.clear();
        p1.clear();
        p2.clear();

    }
    string nomFichier = "approximation.txt";

    // Ouverture du fichier en mode écriture
    ofstream fichierSortie(nomFichier);
    if (!fichierSortie.is_open()) {
        cerr << "Erreur : impossible d'ouvrir le fichier " << nomFichier << endl;
        return 1;
    }
    cout << "la" << endl;


    // Écriture des éléments du vecteur dans le fichier
    fichierSortie << "OPT ";
    for (double element : list_opt) {
        fichierSortie << element << " ";
    }
    fichierSortie << "\n";

    fichierSortie << "CR ";
    for (double element : list_cr) {
        fichierSortie << element << " ";
    }
    fichierSortie << "\n";

    
    fichierSortie << "CNN ";
    for (double element : list_cnn) {
        fichierSortie << element << " ";
    }
    fichierSortie << "\n";

    // Fermeture du fichier
    fichierSortie.close();
    cout << "ici" << endl;

    return 0;
}
