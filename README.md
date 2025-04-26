Ce projet a été réalisé par Simon Groc et Nathan Guetteville

Vous aurez besoin de Graphviz : https://graphviz.org/download/ (obligatoire)
et Numpy et Matplotlib si vous voulez exécuter `plot_speed.py` (optionnel)
`pip install matplotlib`
`pip install numpy`

Compilation :
- sur Windows : `build.bat`
- autres : `./build.sh`

Execution :
- sur Windows : `./graphapp.exe`
- autres : `./graphapp`

La fonction main dans `main.cpp` vous permet de tester les algorithmes, vous pouvez changer les paramètres du graphe en modifiant les arguments de l'appel à `initKCCTP` en prenant soin de respecter l'inégalité `k < n - 1` (Attention, si vous instanciez un graphe de taille supérieure à 10, nous vous conseille de commenter le bloc // OPT du main, pour éviter une longue exécution). Si vous le souhaitez, vous pouvez décommenter l'appel aux fonctions `compute_approx` et `compute_speed` afin de tester plus en profondeur les algorithmes.

Le graphe généré est visualisable dans le fichier `output.png`.

Pour l'approximation, après exécution de l'application principale et visualisation globale dans le terminal :
 - `python3 AnalyseApproximation.py`

Les résultats de l'analyse s'afficheront dans le terminal.

Pour le temps d'execution, si vous voulez inclure Christofides dans les calculs, passez `true` en argument de `compute_speed`, `false` sinon. Les résultats s'afficheront dans le terminal après exécution de l'application principale. Reportez-les dans les tableaux adéquats du fichier `plot_speed.py` et :
- `python3 plot_speed.py`

pour visualiser les courbes.

Source du dossier `blossom5` implémentant l'algorithme blossom pour le couplage parfait de poids minimum : https://pub.ista.ac.at/~vnk/software.html#BLOSSOM5
