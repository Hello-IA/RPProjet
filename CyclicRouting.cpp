#include "CyclicRouting.hpp"


vector<int> cyclicRouting(Graphe g, vector<int> Cyclic){
    bool current_sens = true;
    vector<int> P1 = filtrerParcours(g, Cyclic);

    vector<vector<int>> P = cyclic(g, Cyclic, P1, current_sens);


    vector<int> P_end = lastCyclic(P,g, Cyclic, current_sens);


    vector<int> c = contracte(P, P_end);


    return c;
}

vector<int> contracte(const vector<vector<int> >& P, const vector<int>& P_end) {
    vector<int> cycle;
    
    for (const auto& path : P) {
        // pour éviter les doublons avec le début du chemin suivant
        if (!path.empty()) {
            if (cycle.empty() || cycle.back() != path.front()) {
                // Ajouter le premier nœud seulement s'il n'est pas déjà à la fin de 'cycle'
                cycle.push_back(path.front());
            }
            
            // Ajouter les noeuds suivants (sauf le dernier)
            for (size_t i = 1; i < path.size() - 1; ++i) {
                cycle.push_back(path[i]);
            }
            
            // Ajouter le dernier nœud du chemin
            if (!path.empty()) {
                cycle.push_back(path.back());
            }
        }
    }
    
    // Ajouter P_end (le raccourci final)
    if (!P_end.empty()) {
        // Vérifier si le premier nœud de P_end est déjà à la fin de cycle
        if (cycle.empty() || cycle.back() != P_end.front()) {
            // Si non, l'ajouter
            cycle.push_back(P_end.front());

        }
        
        // Ajouter les autres nœuds de P_end
        for (size_t i = 1; i < P_end.size(); ++i) {
            cycle.push_back(P_end[i]);
        }
    }
    
    return cycle;
}


vector<int> filtrerParcours(Graphe g, vector<int> Cyclic){
    Cyclic.pop_back();
    if (Cyclic.empty()) return {};
    
    vector<int> resultat;
    // On conserve toujours le premier nœud
    resultat.push_back(Cyclic[0]);
    
    for (size_t i = 1; i < Cyclic.size(); ++i) {
        int noeudPrecedent = resultat.back();
        int noeudActuel = Cyclic[i];
        
        Edge* e = g.getEdge(noeudActuel, noeudPrecedent);
        if (e && !e->close) {
            resultat.push_back(noeudActuel);
        }
    }
    return resultat;
}

vector<vector<int>> cyclic(Graphe g, vector<int> Cyclic, vector<int> P1, bool &current_sens) {
    vector<vector<int>> result;
    set<int> already_explor;
    Cyclic.pop_back(); // Retire le dernier élément (doublon)
    bool equal = false;
    
    // Ajouter les nœuds de P1 à already_explor
    for (int node : P1) {
        already_explor.insert(node);
    }
    
    result.push_back(P1);
    
    // Détermine le sens de parcours initial (true = horaire, false = anti-horaire)
    current_sens = true;

    if(equal){
        equal = false;
        current_sens = !current_sens;
    }
    
    // Vérifie la condition pour le sens de parcours
    if (!equal && !result.empty() && result.back().size() > 0) {
        int vm_0 = Cyclic.back();
        int vm_1_last = result.back().back();        
        if (vm_0 != vm_1_last) {
            current_sens = !current_sens;
        }
    }
    
    // Initialiser not_explore correctement
    vector<int> not_explore;
    
    // Ajouter le dernier noeud de P1 comme point de départ
    if (!P1.empty()) {
        not_explore.push_back(P1.back());
    }
    
    // Ajouter uniquement les nœuds non explorés
    
    for (int c : Cyclic) {
        if (already_explor.find(c) == already_explor.end() && 
            find(not_explore.begin(), not_explore.end(), c) == not_explore.end()) {
            not_explore.push_back(c);
        }
    }  
    
    // Débute le nouveau chemin avec le dernier nœud de P1
    vector<int> p;
    if (!P1.empty()) {
        p.push_back(P1.back());
    }
    
    // Limite maximale d'itérations pour éviter les boucles infinies
    int max_iterations = Cyclic.size() * 2;
    int iteration_count = 0;

    
    while (!not_explore.empty() && iteration_count < max_iterations) {
        iteration_count++;
        
        // Copier not_explore pour éviter les problèmes de modification pendant l'itération
        vector<int> current_not_explore = not_explore;
        not_explore.clear();
        
        // Réorganiser current_not_explore pour respecter l'ordre cyclique
        if (!current_not_explore.empty()) {
            vector<int> cyclically_ordered;
            int starting_node = current_not_explore.front();
            
            // Parcourir Cyclic pour trouver les nœuds dans l'ordre cyclique
            int start_idx = -1;
            for (size_t i = 0; i < Cyclic.size(); i++) {
                if (Cyclic[i] == starting_node) {
                    start_idx = i;
                    break;
                }
            }
            
            if (start_idx != -1) {
                // Parcourir Cyclic dans le bon ordre en fonction du sens de parcours
                for (size_t i = 0; i < Cyclic.size(); i++) {
                    int idx;
                    if (current_sens) {
                        // Sens horaire
                        idx = (start_idx + i) % Cyclic.size();
                    } else {
                        // Sens anti-horaire
                        idx = (start_idx - i + Cyclic.size()) % Cyclic.size();
                    }
                    
                    int node = Cyclic[idx];
                    // Si le nœud est dans current_not_explore, l'ajouter à cyclically_ordered
                    if (find(current_not_explore.begin(), current_not_explore.end(), node) != current_not_explore.end()) {
                        cyclically_ordered.push_back(node);
                    }
                }
                
                current_not_explore = cyclically_ordered;
            }
        }
        
        // Parcours des nœuds dans l'ordre cyclique
        int noeudPrecedent = current_not_explore[0];
        for (size_t i = 0; i < current_not_explore.size() - 1; i++) {
            int noeudActuel = current_not_explore[i + 1];
            
            // Vérifier si le nœud précédent est déjà dans le chemin
            if (find(p.begin(), p.end(), noeudPrecedent) == p.end()) {
                p.push_back(noeudPrecedent); 
                already_explor.insert(noeudPrecedent);
            }
            
            // Si l'arête est disponible directement
            Edge* e = g.getEdge(noeudPrecedent, noeudActuel);
            if (e && !e->close) {
                if (already_explor.find(noeudActuel) == already_explor.end()) {
                    p.push_back(noeudActuel);
                    already_explor.insert(noeudActuel);
                    noeudPrecedent = noeudActuel;
                }
            } else {
                // L'arête n'est pas disponible, rechercher un raccourci
                int idx_prec = -1;
                int idx_act = -1;
                
                // Trouver les index dans Cyclic
                for (size_t j = 0; j < Cyclic.size(); j++) {
                    if (Cyclic[j] == noeudPrecedent) idx_prec = j;
                    if (Cyclic[j] == noeudActuel) idx_act = j;
                }
                
                if (idx_prec != -1 && idx_act != -1) {
                    
                    bool raccourci_trouve = false;
                    
                    if (current_sens) {
                        // Parcours horaire
                        int start = idx_prec;
                        int end = idx_act;
                        if (start > end) end += Cyclic.size(); // Assurer que end > start
                        
                        for (int j = start + 1; j < end && !raccourci_trouve; j++) {
                            int node_idx = j % Cyclic.size();
                            Edge* ee1 = g.getEdge(noeudPrecedent, Cyclic[node_idx]);
                            Edge* ee2 = g.getEdge(noeudActuel, Cyclic[node_idx]);
                            if (ee1 && ee2 && !ee1->close && !ee2->close) {
                                p.push_back(Cyclic[node_idx]);
                                already_explor.insert(Cyclic[node_idx]);
                                p.push_back(noeudActuel);
                                already_explor.insert(noeudActuel);
                                noeudPrecedent = noeudActuel;
                                raccourci_trouve = true;
                            }
                        }
                    } else {
                        // Parcours anti-horaire
                        int start = idx_prec;
                        int end = idx_act;
                        if (start < end) start += Cyclic.size(); // Assurer que start > end
                        for (int j = start - 1; j > end && !raccourci_trouve; j--) {

                            int node_idx = (j + Cyclic.size()) % Cyclic.size();
                            Edge* ee1 = g.getEdge(noeudPrecedent, Cyclic[node_idx]);
                            Edge* ee2 = g.getEdge(noeudActuel, Cyclic[node_idx]);
                            if (ee1 && ee2 && !ee1->close && !ee2->close) {
                                p.push_back(Cyclic[node_idx]);
                                already_explor.insert(Cyclic[node_idx]);
                                p.push_back(noeudActuel);
                                already_explor.insert(noeudActuel);
                                noeudPrecedent = noeudActuel;
                                raccourci_trouve = true;
                            }
                        }
                    }
                    
                    // Si aucun raccourci n'est trouvé, ajouter le nœud actuel à not_explore pour traitement ultérieur
                    if (!raccourci_trouve && already_explor.find(noeudActuel) == already_explor.end()) {
                        not_explore.push_back(noeudActuel);
                        current_sens = !current_sens;
                    }
                }
            }
            
        }
        
        // Ajouter le dernier noeud de current_not_explore s'il n'est pas déjà exploré

        if(p == result.back()){
            equal = true;
        }
        
        // Si le chemin p a plus d'un élément, l'ajouter au résultat
        if (p.size() > 1) {
            result.push_back(p);
            
            // Vérifier la condition: Si Vm+1 = Vm, alors changer le sens
            if (result.size() >= 2) {
                vector<int> Vm = result[result.size() - 2];
                vector<int> Vm_plus_1 = result[result.size() - 1];
                
                if (Vm == Vm_plus_1) {
                    current_sens = !current_sens;
                }
            }
            
            // Réinitialiser p pour le prochain chemin
            p.clear();
            
            // Commencer le nouveau chemin avec le dernier nœud du chemin précédent
            if (!result.back().empty()) {
                p.push_back(result.back().back());
            }
        }
        
        // Si not_explore est vide mais qu'il reste des nœuds non explorés dans Cyclic
        if (not_explore.empty()) {
            bool all_explored = true;
            
            for (int c : Cyclic) {
                if (already_explor.find(c) == already_explor.end()) {
                    not_explore.push_back(c);
                    all_explored = false;
                    break;
                }
            }
            
        }
        
    }

    if(iteration_count == max_iterations){
        cout << "On est sorti de force" << endl;
    }
    
    return result;
}


vector<int> lastCyclic(vector<vector<int>> result, Graphe g, vector<int> Cyclic, bool previousSens) {
    // Si aucun résultat, retourner un vecteur vide
    if (result.empty()) {
        return vector<int>();
    }

    vector<int> finalPath;
    
    // Identifier le premier et le dernier noeud qui doivent être connectés
    int firstNode = result.front().front();  // Premier nœud du premier chemin
    int lastNode = result.back().back();     // Dernier nœud du dernier chemin
    
    
    // Vérifier s'il existe un lien direct
    Edge* direct = g.getEdge(lastNode, firstNode);
    
    if (direct && !direct->close) {
        // Si un lien direct existe, l'utiliser simplement
        finalPath.push_back(lastNode);
        finalPath.push_back(firstNode);
        return finalPath;
    }
    
    // Déterminer le sens du parcours en fonction des règles spécifiées
    bool currentSens = previousSens;
    
    // Vérifier: Si vm0 != vm−1 alors on suit l'ordre opposé
    if (result.size() >= 2) {
        int vm_0 = result.back().front();                   
        int vm_1_last = result[result.size()-2].back();     
        
        if (vm_0 != vm_1_last) {
            currentSens = !previousSens;
        }
    }
    
    // Trouver les indices du premier et dernier nœud dans Cyclic
    int lastPos = -1, firstPos = -1;
    for (size_t i = 0; i < Cyclic.size(); i++) {
        if (Cyclic[i] == lastNode) lastPos = i;
        if (Cyclic[i] == firstNode) firstPos = i;
    }
    
    if (lastPos == -1 || firstPos == -1) {
        return vector<int>();
    }
    
    
    finalPath.clear();
    finalPath.push_back(lastNode);
    
    int current = lastPos;
    bool pathValid = true;
    
    // Fonction pour obtenir le prochain nœud selon le sens de parcours
    auto nextPosition = [&Cyclic](int pos, bool sens) {
        if (sens) { // sens horaire
            return (pos + 1) % Cyclic.size();
        } else { // sens anti-horaire
            return (pos - 1 + Cyclic.size()) % Cyclic.size();
        }
    };
    
    int lookAhead = current;
        // Limiter la recherche à la longueur du cycle pour éviter des boucles infinies
    for (size_t i = 0; i < Cyclic.size() && current != firstPos; i++) {
        lookAhead = nextPosition(lookAhead, currentSens);
        int potentialNode = Cyclic[lookAhead];
        
        // Vérifier si on peut aller directement au nœud potentiel
        Edge* shortcut = g.getEdge(finalPath.back(), potentialNode);
        Edge* linck_end = g.getEdge(potentialNode, firstNode);
        if (shortcut && linck_end && !shortcut->close && !linck_end->close) {
            // Raccourci trouvé, ajouter et mettre à jour la position courante
            finalPath.push_back(potentialNode);
            finalPath.push_back(firstNode);
            current = firstPos;
        }
    }

    // Si le chemin ne se termine pas par le nœud de départ, ajouter une connexion directe
    if (pathValid && finalPath.back() != firstNode) {
        Edge* finalEdge = g.getEdge(finalPath.back(), firstNode);
        if (finalEdge && !finalEdge->close) {
            finalPath.push_back(firstNode);
        } else {
            // Si pas de connexion directe à la fin, le chemin n'est pas valide
            pathValid = false;
        
        }
        
        
        // Afficher le raccourci trouvé
        if (!finalPath.empty() && finalPath.back() == firstNode) {

        } else {
            finalPath.clear();
        }
    }
    
    return finalPath;
}
int trouverIndice(const vector<int>& liste, int valeur) {
    auto it = find(liste.begin(), liste.end(), valeur);
    
    if (it != liste.end()) {
        return distance(liste.begin(), it); 
    } else {
        return -1; 
    }
}