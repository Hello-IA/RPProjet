#include <graphviz/gvc.h>
#include <iostream>
#include "MapShape.hpp"

Graphe::Graphe(){

}

void Graphe::addNoeud(Noeud* newN){
    bool exite = false;
    for(Noeud* n : noeuds){
        if(n->getName() == newN->getName()){
            exite = true;
        }
    }
    if(!exite){
        noeuds.push_back(newN);
    }else{
        cout << "il exite deja" << endl;
    }
}

void Graphe::addEdge(Noeud* n1, Noeud* n2, int value){
    Edge* e = new Edge(n1, n2, value);
    edges.push_back(e);
}

void Graphe::display(string path){
     GVC_t* gvc = gvContext();
    Agraph_t* g = agopen((char*)"g", Agundirected, 0);
    
    // Création d'un tableau pour stocker les noeuds Graphviz
    vector<Agnode_t*> graphviz_nodes;
    
    // Ajouter les noeuds
    for(Noeud* n : noeuds) {
        string str = "Node" + to_string(n->getName());
        Agnode_t* node = agnode(g, (char*)str.c_str(), 1);
        graphviz_nodes.push_back(node);
    }
    
    // Ajouter les arêtes
    for(Edge* e : edges) {
        vector<Noeud*> links = e->getLinks();
        
        // Trouver les indices des noeuds
        int idx1 = -1, idx2 = -1;
        for(size_t i = 0; i < noeuds.size(); i++) {
            if(noeuds[i] == links[0]) idx1 = i;
            if(noeuds[i] == links[1]) idx2 = i;
        }
        
        if(idx1 >= 0 && idx2 >= 0) {
            string value_str = to_string(e->getValue());
            agedge(g, graphviz_nodes[idx1], graphviz_nodes[idx2], (char*)value_str.c_str(), 1);
        }
    }
    
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", path.c_str());
    
    gvFreeLayout(gvc, g);
    agclose(g);
}

Noeud::Noeud(int name){
    this->name = name;
}

int Noeud::getName(){
    return name;
}



Edge::Edge(Noeud* n1, Noeud* n2, int value){
    this->value = value;
    links.push_back(n1);
    links.push_back(n2);
}

int Edge::getValue(){
    return value;
}

vector<Noeud*> Edge::getLinks(){
    return links;
}

int main() {
    cout << "oui"  << endl;
    Noeud* n1 = new Noeud(1);
    Noeud* n2 = new Noeud(2);
    Graphe g;
    g.addNoeud(n1);
    g.addNoeud(n2);
    g.addEdge(n1, n2, 15);
    g.display("output.png");
    delete n1;
    delete n2;
    return 0;
}
