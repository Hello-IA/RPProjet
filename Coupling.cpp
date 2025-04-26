#include "Coupling.hpp"


string graph_to_string(Graphe* g, unordered_map<int, int>* map_ids, unordered_map<int, int>* reverse_map_ids) {
    int nb_nodes = g->noeuds.size();
    int nb_edges = g->edges.size();
    stringstream f;

    f << nb_nodes << " " << nb_edges << endl;
    int idcount = 0;
    for (Edge* e : g->edges) {
        vector<Noeud*> links = e->getLinks();
        auto result = map_ids->insert({links[0]->getName(), idcount});
        if (result.second) {
            reverse_map_ids->insert({idcount, links[0]->getName()});
            idcount++;
        }
        result = map_ids->insert({links[1]->getName(), idcount});
        if (result.second) {
            reverse_map_ids->insert({idcount, links[1]->getName()});
            idcount++;
        }

        f << map_ids->at(links[0]->getName()) << " " << map_ids->at(links[1]->getName()) << " " << int(round(e->getValue())) /*blossom5 doesn't accept float*/ << endl;
    }
    return f.str();
}

vector<Edge*> string_to_coupling(Graphe* g, string graphStr, unordered_map<int, int>* reverse_map_ids) {
    vector<Edge*> result;
    stringstream ss(graphStr);


    string line;
    bool first_line = true;
    while(getline(ss, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (first_line) {
            first_line = false;
            continue;
        }
        istringstream iss(line);
        int u, v;
        iss >> u >> v;

        result.push_back(g->getEdge(reverse_map_ids->at(u), reverse_map_ids->at(v)));
    }

    return result;
}

vector<Edge*> coupling_perfect(Graphe* g) {
    vector<Edge*> result = vector<Edge*>();

    unordered_map<int, int> map_ids;
    unordered_map<int, int> reverse_map_ids;

    string data = graph_to_string(g, &map_ids, &reverse_map_ids);
    string output = coupling(data);

    return string_to_coupling(g, output, &reverse_map_ids);
}

vector<Edge*> coupling_glutton(Graphe* g) {
    vector<Edge*> c;
    unordered_set<Noeud*> traites;

    vector<Edge*> sorted_edges = g->edges;
    sort(sorted_edges.begin(), sorted_edges.end(), [](Edge* e1, Edge* e2){
        return e1->getValue() < e2->getValue();
    });

    for (Edge* edge: sorted_edges) {
        vector<Noeud*> noeuds = edge->getLinks();
        Noeud* u = noeuds[0];
        Noeud* v = noeuds[1];

        if (traites.count(u) == 0 && traites.count(v) == 0) {
            c.push_back(edge);
            traites.insert(u);
            traites.insert(v);
        }
    }

    return c;
}
