import matplotlib.pyplot as plt
import numpy as np

# Données
tailles = np.array([10, 20, 40, 60, 80, 100])

# Avec Christofides
temps_cr = np.array([0.00249084, 0.0284689, 0.399835, 2.09249, 6.444, 15.6631])
temps_cnn = np.array([0.00348648, 0.0344883, 0.452101, 2.30574, 6.93661, 16.3953])
ecarts_types_cr = np.array([0.000288738, 0.000848713, 0.00924475, 0.0623714, 0.184656, 1.27154])
ecarts_types_cnn = np.array([0.000954628, 0.00632627, 0.0598401, 0.191839, 0.570596, 1.62617])

# Sans Christofides
#temps_cr = np.array([0.00022378, 0.00136512, 0.00961938, 0.0344805, 0.0903767, 0.172662])
#temps_cnn = np.array([0.00122462, 0.00776766, 0.065531, 0.258764, 0.720863, 1.07373])
#ecarts_types_cr = np.array([0.0000262223, 0.000204138, 0.000720738, 0.00297387, 0.00802789, 0.00968883])
#ecarts_types_cnn = np.array([0.000825117, 0.00668408, 0.0633652, 0.183887, 0.586427, 0.970382])

# Création des subplots
fig, ax = plt.subplots(figsize=(10, 6))

fig.canvas.manager.set_window_title("cr_vs_cnn_avec_christofides")

# Courbe CR + zone d'incertitude
ax.plot(tailles, temps_cr, label='cr', marker='o')
ax.fill_between(tailles, temps_cr - ecarts_types_cr, temps_cr + ecarts_types_cr, alpha=0.2)

# Courbe CNN + zone d'incertitude
ax.plot(tailles, temps_cnn, label='cnn', marker='s')
ax.fill_between(tailles, temps_cnn - ecarts_types_cnn, temps_cnn + ecarts_types_cnn, alpha=0.2)

# Titre et légende
ax.set_title("Temps d'exécution moyens avec écart-type")
ax.set_xlabel("Taille du graphe")
ax.set_ylabel("Temps (secondes)")
ax.legend()
ax.grid(True, linestyle='--', alpha=0.5)

plt.tight_layout()
plt.show()
