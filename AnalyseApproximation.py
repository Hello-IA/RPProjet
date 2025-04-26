def extract_data_to_lists(file_path):
    opt_values = []
    cr_values = []
    cnn_values = []
    
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
            
            # Extraire les valeurs de chaque ligne
            for line in lines:
                if line.startswith('OPT '):
                    # Récupérer les valeurs après "OPT "
                    values = line.split(' ')[1:]
                    opt_values = [float(val) for val in values if val.strip()]
                
                elif line.startswith('CR '):
                    # Récupérer les valeurs après "CR "
                    values = line.split(' ')[1:]
                    cr_values = [float(val) for val in values if val.strip()]
                
                elif line.startswith('CNN '):
                    # Récupérer les valeurs après "CNN "
                    values = line.split(' ')[1:]
                    cnn_values = [float(val) for val in values if val.strip()]
        
        return opt_values, cr_values, cnn_values
    
    except FileNotFoundError:
        print(f"Erreur : Le fichier '{file_path}' n'a pas été trouvé.")
        return [], [], []
    except Exception as e:
        print(f"Erreur lors de la lecture du fichier : {e}")
        return [], [], []

# Chemin vers le fichier texte
file_path = "approximation.txt"  # Remplacez par le chemin de votre fichier

# Extraire les données dans trois listes
opt_values, cr_values, cnn_values = extract_data_to_lists(file_path)

aprox_cr = []
aprox_cnn = []

for o, r in zip(opt_values, cr_values):
    aprox_cr.append(r/o)

for o, n in zip(opt_values, cnn_values):
    aprox_cnn.append(n/o)
    
cr = sum(aprox_cr)/len(aprox_cr)

cnn = sum(aprox_cnn)/len(aprox_cnn)

print("CR :", cr)
print("CNN :", cnn)