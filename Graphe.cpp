#include "Graphe.hpp"

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

void Graphe::addEdge(Noeud* n1, Noeud* n2, double value){
    Edge* e = new Edge(n1, n2, value);
    edges.push_back(e);
    n1->neighbor.push_back(e);
    n2->neighbor.push_back(e);
}

void Graphe::display(string path = "output.png"){
     GVC_t* gvc = gvContext();
    Agraph_t* g = agopen((char*)"g", Agundirected, 0);

    vector<Agnode_t*> graphviz_nodes;
    

    for(Noeud* n : noeuds) {
        string str = "Node" + to_string(n->getName());
        Agnode_t* node = agnode(g, (char*)str.c_str(), 1);
        graphviz_nodes.push_back(node);
    }
    

    for(Edge* e : edges) {
        vector<Noeud*> links = e->getLinks();
        
        int idx1 = -1, idx2 = -1;
        for(size_t i = 0; i < noeuds.size(); i++) {
            if(noeuds[i] == links[0]) idx1 = i;
            if(noeuds[i] == links[1]) idx2 = i;
        }
        
        if(idx1 >= 0 && idx2 >= 0) {
            Agedge_t* edge = agedge(g, graphviz_nodes[idx1], graphviz_nodes[idx2], (char*)"", 1);

            string value_str = to_string(e->getValue());
            agsafeset(edge, (char*)"label", (char*)value_str.c_str(), (char*)"");
        }
    }
    
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", path.c_str());
    
    gvFreeLayout(gvc, g);
    agclose(g);
}

Noeud* Graphe::getNoeud(int nb){
    return noeuds[nb];
}