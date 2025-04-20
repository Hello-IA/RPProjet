#include "CyclicRouting.hpp"


vector<int> cyclicRouting(Graphe g, vector<int> Cyclic){
    bool current_sens = true;
    vector<int> P1 = filtrerParcours(g, Cyclic);

    vector<vector<int>> P = cyclic(g, Cyclic, P1, current_sens);
    cout << "P : ";
    for(vector<int> p : P){
        cout << "p :";
        for(int i : p){
            cout << i << " ";
        }
        cout << endl;
    }
    cout << endl;

    vector<int> P_end = lastCyclic(P,g, Cyclic, current_sens);
    cout << "P_end : ";
    for(int p : P_end){
        cout << p << " ";

    }
    cout << endl;

    vector<int> c = contracte(P, P_end);

    return c;
}

vector<int> contracte(const vector<vector<int> >& P, const vector<int>& P_end) {
    vector<int> cycle;
    
    // Ajouter tous les chemins intermédiaires
    for (const auto& path : P) {
        // Pour chaque chemin, ajouter tous les nœuds sauf le dernier
        // (pour éviter les doublons avec le début du chemin suivant)
        if (!path.empty()) {
            if (cycle.empty() || cycle.back() != path.front()) {
                // Ajouter le premier nœud seulement s'il n'est pas déjà à la fin de 'cycle'
                cycle.push_back(path.front());
            }
            
            // Ajouter les nœuds suivants (sauf le dernier)
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
    
    // Vérification: si le dernier nœud est égal au premier, le supprimer
    // pour éviter les doublons dans le cycle
    if (cycle.size() > 1 && cycle.back() == cycle.front()) {
        cycle.pop_back();
    }
    
    // Affichage du cycle pour vérification
    cout << "Cycle contracté final (CR): ";
    for (int node : cycle) {
        cout << node << " ";
    }
    cout << endl;
    
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
        }else{
            cout << "imposible de se rendre de "<< noeudPrecedent << "au noeud" << noeudActuel << endl;
        }
    }
    return resultat;
}

vector<vector<int>> cyclic(Graphe g, vector<int> Cyclic, vector<int> P1, bool &current_sens) {
    vector<vector<int>> result;
    set<int> already_explor;
    Cyclic.pop_back(); // Retire le dernier élément (doublon)
    
    // Ajouter les nœuds de P1 à already_explor
    for (int node : P1) {
        already_explor.insert(node);
    }
    
    result.push_back(P1);
    
    // Détermine le sens de parcours initial (true = horaire, false = anti-horaire)
    current_sens = true;
    
    // Vérifie la condition pour le sens de parcours
    if (!result.empty() && result.back().size() > 0) {
        int vm_0 = Cyclic.back();
        cout << "vm_0 " << vm_0 << endl;
        int vm_1_last = result.back().back();
        cout << "vm_1_last " << vm_1_last << endl;
        
        if (vm_0 != vm_1_last) {
            current_sens = !current_sens;
        }
    }
    
    // Initialiser not_explore correctement
    vector<int> not_explore;
    
    // Ajouter le dernier nœud de P1 comme point de départ
    if (!P1.empty()) {
        not_explore.push_back(P1.back());
    }
    
    // Ajouter uniquement les nœuds non explorés
    cout << "Cyclic : ";
    for(int i : Cyclic) { cout << i << " "; }
    cout << endl;
    
    for (int c : Cyclic) {
        if (already_explor.find(c) == already_explor.end() && 
            find(not_explore.begin(), not_explore.end(), c) == not_explore.end()) {
            not_explore.push_back(c);
        }
    }  

    cout << "not_explore : ";  
    for(int i : not_explore) { cout << i << " "; }  
    cout << endl;
    
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
        cout << "Itération " << iteration_count << endl;
        
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
        
        cout << "current_not_explore réorganisé: ";
        for (int node : current_not_explore) { 
            cout << node << " "; 
        }
        cout << endl;
        
        // Parcours des nœuds dans l'ordre cyclique
        for (size_t i = 0; i < current_not_explore.size() - 1; i++) {
            int noeudPrecedent = current_not_explore[i];
            int noeudActuel = current_not_explore[i + 1];
            
            cout << "Traitement des noeuds " << noeudPrecedent << " -> " << noeudActuel << endl;
            
            // Vérifier si le nœud précédent est déjà dans le chemin
            if (find(p.begin(), p.end(), noeudPrecedent) == p.end()) {
                p.push_back(noeudPrecedent); 
                already_explor.insert(noeudPrecedent);
            }
            
            // Si l'arête est disponible directement
            Edge* e = g.getEdge(noeudPrecedent, noeudActuel);
            if (e && !e->close) {
                cout << "Arête directe disponible " << noeudPrecedent << " -> " << noeudActuel << endl;
                if (already_explor.find(noeudActuel) == already_explor.end()) {
                    p.push_back(noeudActuel);
                    already_explor.insert(noeudActuel);
                    cout << "Nœud " << noeudActuel << " ajouté au chemin et marqué comme exploré" << endl;
                }
            } else {
                cout << "Pas d'arête directe, recherche d'un raccourci" << endl;
                // L'arête n'est pas disponible, rechercher un raccourci
                int idx_prec = -1;
                int idx_act = -1;
                
                // Trouver les index dans Cyclic
                for (size_t j = 0; j < Cyclic.size(); j++) {
                    if (Cyclic[j] == noeudPrecedent) idx_prec = j;
                    if (Cyclic[j] == noeudActuel) idx_act = j;
                }
                
                if (idx_prec != -1 && idx_act != -1) {
                    cout << "Index trouvés: " << noeudPrecedent << " à " << idx_prec << ", " << noeudActuel << " à " << idx_act << endl;
                    
                    bool raccourci_trouve = false;
                    
                    if (current_sens) {
                        // Parcours horaire
                        int start = idx_prec;
                        int end = idx_act;
                        if (start > end) end += Cyclic.size(); // Assurer que end > start
                        
                        for (int j = start + 1; j < end && !raccourci_trouve; j++) {
                            int node_idx = j % Cyclic.size();
                            Edge* ee = g.getEdge(noeudPrecedent, Cyclic[node_idx]);
                            
                            if (ee && !ee->close && already_explor.find(Cyclic[node_idx]) == already_explor.end()) {
                                cout << "Raccourci trouvé via " << Cyclic[node_idx] << endl;
                                p.push_back(Cyclic[node_idx]);
                                already_explor.insert(Cyclic[node_idx]);
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
                            Edge* ee = g.getEdge(noeudPrecedent, Cyclic[node_idx]);
                            
                            if (ee && !ee->close && already_explor.find(Cyclic[node_idx]) == already_explor.end()) {
                                cout << "Raccourci trouvé via " << Cyclic[node_idx] << endl;
                                p.push_back(Cyclic[node_idx]);
                                already_explor.insert(Cyclic[node_idx]);
                                raccourci_trouve = true;
                            }
                        }
                    }
                    
                    // Si aucun raccourci n'est trouvé, ajouter le nœud actuel à not_explore pour traitement ultérieur
                    if (!raccourci_trouve && already_explor.find(noeudActuel) == already_explor.end()) {
                        cout << "Pas de raccourci trouvé, " << noeudActuel << " ajouté à not_explore" << endl;
                        not_explore.push_back(noeudActuel);
                    }
                }
            }
            
        }
        
        // Ajouter le dernier nœud de current_not_explore s'il n'est pas déjà exploré
        if (!current_not_explore.empty() && 
            already_explor.find(current_not_explore.back()) == already_explor.end()) {
            p.push_back(current_not_explore.back());
            already_explor.insert(current_not_explore.back());
        }
        
        cout << "Chemin actuel p: ";
        for (int node : p) { cout << node << " "; }
        cout << endl;
        
        // Si le chemin p a plus d'un élément, l'ajouter au résultat
        if (p.size() > 1) {
            result.push_back(p);
            
            // Vérifier la condition: Si Vm+1 = Vm, alors changer le sens
            if (result.size() >= 2) {
                vector<int> Vm = result[result.size() - 2];
                vector<int> Vm_plus_1 = result[result.size() - 1];
                
                if (Vm == Vm_plus_1) {
                    current_sens = !current_sens;
                    cout << "Changement de sens: " << (current_sens ? "horaire" : "anti-horaire") << endl;
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
                    cout << "Nouveau point de départ trouvé: " << c << endl;
                    break;
                }
            }
            
            // Si tous les nœuds sont explorés, sortir de la boucle
            if (all_explored) {
                cout << "Tous les nœuds sont explorés" << endl;
                break;
            }
        }
        
        cout << "not_explore à la fin d'itération: ";
        for (int node : not_explore) { cout << node << " "; }
        cout << endl;
        
        // Éviter les boucles infinies si not_explore reste identique
        if (not_explore == current_not_explore) {
            cout << "Pas de changement dans not_explore, sortie de la boucle" << endl;
            break;
        }
    }
    
    // Si p contient encore des nœuds, l'ajouter au résultat
    if (p.size() > 1) {
        result.push_back(p);
    }
    
    // Afficher les chemins résultants
    for (size_t i = 0; i < result.size(); i++) {
        cout << "Lite P" << i << " ";
        for (int node : result[i]) { cout << node << " "; }
        cout << endl;
    }
    
    return result;
}


vector<int> lastCyclic(vector<vector<int>> result, Graphe g, vector<int> Cyclic, bool previousSens) {
    // Si aucun résultat, retourner un vecteur vide
    if (result.empty()) {
        return vector<int>();
    }

    vector<int> finalPath;
    
    // Identifier le premier et le dernier nœud qui doivent être connectés
    int firstNode = result.front().front();  // Premier nœud du premier chemin
    int lastNode = result.back().back();     // Dernier nœud du dernier chemin
    
    cout << "Recherche d'un raccourci de " << lastNode << " vers " << firstNode << endl;
    
    // Vérifier s'il existe un lien direct
    Edge* direct = g.getEdge(lastNode, firstNode);
    
    if (direct && !direct->close) {
        // Si un lien direct existe, l'utiliser simplement
        cout << "Lien direct trouvé entre " << lastNode << " et " << firstNode << endl;
        finalPath.push_back(lastNode);
        finalPath.push_back(firstNode);
        return finalPath;
    }
    
    // Pas de lien direct, rechercher un raccourci à travers Cyclic
    cout << "Pas de lien direct, recherche d'un raccourci à travers le cycle" << endl;
    
    // Déterminer le sens du parcours en fonction des règles spécifiées
    bool currentSens = previousSens;
    
    // Vérifier: Si vm,0 ≠ vm−1,|Vm−1|, alors on suit l'ordre opposé
    if (result.size() >= 2) {
        int vm_0 = result.back().front();                   // Premier nœud du dernier chemin
        int vm_1_last = result[result.size()-2].back();     // Dernier nœud de l'avant-dernier chemin
        
        if (vm_0 != vm_1_last) {
            currentSens = !previousSens;
            cout << "Changement de sens car vm,0 ≠ vm−1,|Vm−1|" << endl;
        } else {
            cout << "Même sens car vm,0 = vm−1,|Vm−1|" << endl;
        }
    }
    
    // Trouver les indices du premier et dernier nœud dans Cyclic
    int lastPos = -1, firstPos = -1;
    for (size_t i = 0; i < Cyclic.size(); i++) {
        if (Cyclic[i] == lastNode) lastPos = i;
        if (Cyclic[i] == firstNode) firstPos = i;
    }
    
    if (lastPos == -1 || firstPos == -1) {
        cout << "Impossible de trouver l'un des nœuds dans Cyclic" << endl;
        return vector<int>();
    }
    
    cout << "Parcours dans le sens " << (currentSens ? "horaire" : "anti-horaire") << endl;
    
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
            cout << "la sa devrais etre 0 : " << finalPath.back() << endl;
            current = firstPos;
        }
    }

    cout << "la les noeud devris etre les même " << finalPath.back() << " et " << firstNode <<endl;
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
            cout << "Raccourci trouvé : ";
            for (int node : finalPath) {
                cout << node << " ";
            }
            cout << endl;
        } else {
            cout << "Aucun raccourci valide n'a pu être trouvé" << endl;
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