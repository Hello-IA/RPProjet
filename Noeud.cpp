#include "Noeud.hpp"

Noeud::Noeud(int name){
    this->name = name;
}

int Noeud::getName(){
    return name;
}


vector<Noeud*> Noeud::getNeighbor(){
    vector<Noeud*> n;
    for(Edge* e : neighbor){
        if(e->getLinks()[0] != this){
            n.push_back(e->getLinks()[0]);
        }else{
            n.push_back(e->getLinks()[1]);
        }
    }
    return n;
}

vector<int> Noeud::getNeigborName(){
    vector<Noeud*> nextNode = getNeighbor();
    vector<int> name;
    for(Noeud* n : nextNode){
        name.push_back(n->getName());
    }
    return name;
}