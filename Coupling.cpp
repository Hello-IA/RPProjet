#include "Coupling.hpp"


void graph_to_file(Graphe* g, string filename, unordered_map<int, int>* map_ids, unordered_map<int, int>* reverse_map_ids) {
    int nb_nodes = g->noeuds.size();
    int nb_edges = g->edges.size();
    ofstream f;
    f.open(filename, ios::trunc);

    if (!f.is_open()) {
        cerr << "Erreur lors de l'ouverture du fichier " << filename << endl;
        return;
    }

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
    f.close();
}

vector<Edge*> file_to_coupling(Graphe* g, string filename, unordered_map<int, int>* reverse_map_ids) {
    vector<Edge*> result;
    ifstream f;
    f.open(filename);
    if (!f.is_open()) {
        cerr << "Erreur lors de la lecture du fichier " << filename << endl;
        exit(1);
    }

    string line;
    bool first_line = true;
    while(getline(f, line)) {
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

    f.close();
    return result;
}

vector<Edge*> coupling_perfect(Graphe* g) {
    vector<Edge*> result = vector<Edge*>();
    string inputfile = "graph.txt";
    string outputfile = "coupling.txt";

    unordered_map<int, int> map_ids;
    unordered_map<int, int> reverse_map_ids;

    graph_to_file(g, inputfile, &map_ids, &reverse_map_ids);
    
    string cmd = "./blossom5/blossom5 -V -e " + inputfile + " -w " + outputfile;
    int status = system(cmd.c_str());
    if (status != 0) {
        cerr << "Erreur lors de l'execution de blossom5." << endl;
        exit(1);
    }

    return file_to_coupling(g, outputfile, &reverse_map_ids);
}


/*
#define M 500

struct StructEdge {
    int vertex;
    StructEdge* next;
};
typedef StructEdge* EdgeNext;

class Blossom {
    StructEdge pool[M * M * 2];
    EdgeNext top = pool, adj[M];
    int V, E, q_front, q_rear;
    int match[M], bfs_queue[M], father[M], base[M];
    bool inq[M], inb[M], ed[M][M];

public:
    Blossom(int V) : V(V), E(0) {
        memset(adj, 0, sizeof(adj));
        memset(ed, 0, sizeof(ed));
    }

    void addEdge(int u, int v) {
        if (!ed[u][v] && u != v) {
            top->vertex = v; top->next = adj[u]; adj[u] = top++;
            top->vertex = u; top->next = adj[v]; adj[v] = top++;
            ed[u][v] = ed[v][u] = true;
            E++;
        }
    }

    int LCA(int root, int u, int v) {
        static bool inp[M];
        memset(inp, 0, sizeof(inp));
        while (1) {
            inp[u = base[u]] = true;
            if (u == root) break;
            u = father[match[u]];
        }
        while (1) {
            if (inp[v = base[v]]) return v;
            else v = father[match[v]];
        }
    }

    void mark_blossom(int lca, int u) {
        while (base[u] != lca) {
            int v = match[u];
            inb[base[u]] = inb[base[v]] = true;
            u = father[v];
            if (base[u] != lca) father[u] = v;
        }
    }

    void blossom_contraction(int s, int u, int v) {
        int lca = LCA(s, u, v);
        memset(inb, 0, sizeof(inb));
        mark_blossom(lca, u);
        mark_blossom(lca, v);
        if (base[u] != lca) father[u] = v;
        if (base[v] != lca) father[v] = u;
        for (int u = 0; u < V; u++) {
            if (inb[base[u]]) {
                base[u] = lca;
                if (!inq[u]) inq[bfs_queue[++q_rear] = u] = true;
            }
        }
    }

    int find_augmenting_path(int s) {
        memset(inq, 0, sizeof(inq));
        memset(father, -1, sizeof(father));
        for (int i = 0; i < V; i++) base[i] = i;
        inq[bfs_queue[q_front = q_rear = 0] = s] = true;
        while (q_front <= q_rear) {
            int u = bfs_queue[q_front++];
            for (EdgeNext e = adj[u]; e; e = e->next) {
                int v = e->vertex;
                if (base[u] != base[v] && match[u] != v) {
                    if ((v == s) || (match[v] != -1 && father[match[v]] != -1)) {
                        blossom_contraction(s, u, v);
                    } else if (father[v] == -1) {
                        father[v] = u;
                        if (match[v] == -1) return v;
                        else if (!inq[match[v]]) inq[bfs_queue[++q_rear] = match[v]] = true;
                    }
                }
            }
        }
        return -1;
    }

    int augment_path(int s, int t) {
        int u = t, v, w;
        while (u != -1) {
            v = father[u];
            w = match[v];
            match[v] = u;
            match[u] = v;
            u = w;
        }
        return t != -1;
    }

    int edmondsBlossomAlgorithm() {
        int match_counts = 0;
        memset(match, -1, sizeof(match));
        for (int u = 0; u < V; u++) {
            if (match[u] == -1)
                match_counts += augment_path(u, find_augmenting_path(u));
        }
        return match_counts;
    }

    vector<pair<int, int> > getMatching() {
        vector<pair<int, int> > res;
        for (int i = 0; i < V; ++i) {
            if (match[i] != -1 && i < match[i])
                res.emplace_back(i, match[i]);
        }
        return res;
    }
};

// Fonction d'appel à partir d'un Graphe*
void runEdmonds(Graphe* g) {
    int V = g->noeuds.size();
    Blossom blossom(V);
    
    for (Edge* e : g->edges) {
        vector<Noeud*> ends = e->getLinks();
        if (ends.size() != 2) continue;
        int u = ends[0]->getName();
        int v = ends[1]->getName();
        blossom.addEdge(u, v);
    }

    int res = blossom.edmondsBlossomAlgorithm();
    auto matching = blossom.getMatching();

    cout << "Matching size: " << res << endl;
    for (auto& [u, v] : matching) {
        cout << "Matched: " << u << " <-> " << v << endl;
    }
}
*/
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
/*
vector<Edge*> augment_matching(vector<Edge*> m, vector<Edge*> path) {
    vector<Edge*> new_m = m;

    for (Edge* e: path) {
        auto it = find(new_m.begin(), new_m.end(), e);
        if (it != new_m.end()) {
            new_m.erase(it);
        } else {
            new_m.push_back(e);
        }
    }

    return new_m;

}

Edge* get_matching_edge(vector<Edge*> m, Noeud* n) {
    cout << "enter get_matching_edge" << endl;
    for (Edge* e: m) {
        vector<Noeud*> noeuds = e->getLinks();
        if (n == noeuds[0] || n == noeuds[1]) {
            return e;
        }
    }
    return nullptr;
}

Edge* get_edge_between(Noeud* a, Noeud* b, Graphe* G) {
    for (Edge* e : G->edges) {
        auto links = e->getLinks();
        if ((links[0] == a && links[1] == b) || (links[0] == b && links[1] == a))
            return e;
    }
    return nullptr;
}


Noeud* other(Noeud* v, Edge* e) {
    vector<Noeud*> noeuds = e->getLinks();
    return (noeuds[0] == v) ? noeuds[1] : noeuds[0];
}



vector<Edge*> find_augmenting_path(Graphe* g, vector<Edge*> m) {
    cout << "entering find_augmenting_path..." << endl;
    unordered_map<Noeud*, Noeud*> parent;
    unordered_map<Noeud*, Noeud*> base;
    unordered_map<Noeud*, Edge*> match;
    unordered_map<Noeud*, int> level;  // 0 = pair, 1 = impair
    unordered_map<Noeud*, bool> visited;
    queue<Noeud*> q;
    
    cout << "right before initialisation" << endl;
    for (Noeud* n:g->noeuds) {
        if (!n) {
            cerr << "Erreur : un Noeud* nul dans G->noeuds" << endl;
            exit(1);
        }
        cout << "Initialisation sommet " << n->getName() << endl;
        cout << "init match" << endl;
        match[n] = get_matching_edge(m, n);
        cout << "init parent" << endl;
        parent[n] = nullptr;
        cout << "init base" << endl;
        base[n] = n;
        cout << "init visited" << endl;
        visited[n] = false;
        if(!match[n]) {
            cout << "init q" << endl;
            q.push(n);
            cout << "init level" << endl;
            level[n] = 0;
        }
    }
    //cout << "right before find_base..." << endl;

    auto find_base = [&](Noeud* v) {
        return base[v];
    };

    //cout << "right before lca..." << endl;
    //Lowest Common Ancestor
    auto lca = [&](Noeud* a, Noeud* b) -> Noeud* {
        unordered_set<Noeud*> path;
        while (a) {
            path.insert(find_base(a));
            a = parent[a] ? other(a, match[a]) : nullptr;
        }
        while (b) {
            if (path.count(find_base(b)))
                return find_base(b);
            b = parent[b] ? other(b, match[b]) : nullptr;
        }
        return nullptr;
    };
    //cout << "right before mark_blossom..." << endl;

    auto mark_blossom = [&](Noeud* v, Noeud* w, Noeud* lca_node, unordered_map<Noeud*, bool>& in_blossom) {
        while (find_base(v) != lca_node) {
            in_blossom[find_base(v)] = true;
            in_blossom[find_base(other(v, match[v]))] = true;
            parent[v] = w;
            w = other(v, match[v]);
            v = parent[w];
        }
    };
    //cout << "right before the while..." << endl;
    while (!q.empty()) {
        Noeud* n = q.front();
        q.pop();

        for (Edge* e : n->neighbor) {
            Noeud* w = other(n, e);
            if (base[n] == base[w] || match[n] == e) {
                continue;
            }
            if (!visited[w]) {
                parent[w] = n;
                if (!match[w]) {
                    vector<Edge*> path;
                    Noeud* x = n;

                    while (parent[x]) {
                        Edge* edge = get_edge_between(x, parent[x], g);
                        if (!edge) {
                            cerr << "get_edge_between returned nullptr" << endl;
                            exit(1);
                        }
                        path.push_back(edge);
                        x = parent[x];
                    }
                    reverse(path.begin(), path.end());
                    path.push_back(e);
                    x = w;
                    while (parent[x]) {
                        path.push_back(get_edge_between(x, parent[x], g));
                        x = parent[x];
                    }
                    return path;
                }
                visited[w] = true;
                q.push(other(w, match[w]));
                parent[other(w, match[w])] = w;
            } else if (level[w] == 0) {
                Noeud* b = lca(n, w);
                unordered_map<Noeud*, bool> in_blossom;
                mark_blossom(n, w, b, in_blossom);
                mark_blossom(w, n, b, in_blossom);
                for (Noeud* u : g->noeuds) {
                    if (in_blossom[base[u]]) {
                        base[u] = b;
                        if (!visited[u]) {
                            visited[u] = true;
                            q.push(u);
                        }
                    }
                }
            }
        }
    }
    //cout << "maybe error when i do the {}" << endl;
    return {};

}

vector<Edge*> find_maximum_matching(Graphe* g, vector<Edge*> m) {
    vector<Edge*> path = find_augmenting_path(g, m);
    if (path.empty()) {
        return m;
    } else {
        return find_maximum_matching(g, augment_matching(m, path));
    }
}

vector<Edge*> coupling_edmonds(Graphe* g) {
    cout << "entering coupling opt..." << endl;
    return find_maximum_matching(g, vector<Edge*>());
}
*/



