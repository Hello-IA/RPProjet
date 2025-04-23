#include "Noeud.hpp"

Noeud::Noeud(int name){
    this->name = name;
}

int Noeud::getName(){
    return name;
}


vector<Noeud*> Noeud::getNeighbors(){
    vector<Noeud*> n;
    for(Edge* e : neighboringEdges){
        if(e->getLinks()[0] != this){
            n.push_back(e->getLinks()[0]);
        }else{
            n.push_back(e->getLinks()[1]);
        }
    }
    return n;
}

vector<int> Noeud::getNeighborsNames(){
    vector<Noeud*> nextNode = getNeighbors();
    vector<int> name;
    for(Noeud* n : nextNode){
        name.push_back(n->getName());
    }
    return name;
}