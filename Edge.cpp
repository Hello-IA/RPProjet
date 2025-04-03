#include "Edge.hpp"

Edge::Edge(Noeud* n1, Noeud* n2, double value){
    this->value = value;
    links.push_back(n1);
    links.push_back(n2);
}

double Edge::getValue(){
    return value;
}

vector<Noeud*> Edge::getLinks(){
    return links;
}