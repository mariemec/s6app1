import numpy as np

# Quelques exercices avec l'indexation numpy
# Les reponses doivent etre dans les variables qX, et les reponses attendues 
# sont dans les variables qX_rep.
# Les reponses doivent etre des sous-ensembles des tableaux data1D, 2D ou 3D.
# Modifiez les lignes qX = dataXD[:] pour obtenir les reponses attendues.
# La reponse par defaut retourne tous les elements.
# La fonction test prend l'ensemble des reponses et vous donnent votre resultat.
# Tout est automatique, vous n'avez rien a changer au programme a part ces lignes.

def test(qs, rs):
    pairs = zip(qs, rs)
    i = 0
    for (q, r) in pairs:
        i += 1
        if not np.array_equal(q, r):
            print("Erreur q%i: %s devrait etre %s."%(i, str(q), str(r)))

data1D = np.array([1,2,3,4,5])      # Vecteur 1x5
data2D = np.array([                 # Matrice 5x5
    [1,2,3,4,5],
    [6,7,8,9,10],
    [11,12,13,14,15],
    [16,17,18,19,20],
    [21,22,23,24,25]
])
data3D = np.array([                 # Cube 2x2x2
    [[1,2], [3,4]],                 # Premiere couche 2x2
    [[5,6], [7,8]]                  # Deuxieme couche 2x2
])

### QUESTIONS ###

# Q1 - Tous les elements du vecteur 1D (la reponse est donnee)
q1      = data1D[:] 
q1_rep  = data1D

# Q2 - Les trois premiers elements du vecteur 1D
q2      = data1D[:3]
q2_rep  = np.array([1,2,3])

# Q3 - Les deux derniers elements du vecteur 1D
q3      = data1D[-2:]
q3_rep  = np.array([4,5])

# Q4 - L'element (2,3) de la matrice 2D (attention: 1 est le premier element)
q4      = data2D[1][2]
q4_rep  = 8

# Q5 - La derniere colonne de la matrice 2D, en utilisant un indice negatif
q5      = [d[-1] for d in data2D]
q5_rep  = np.array([5, 10, 15, 20, 25])

# Q6 - La couche inferieure du cube 2x2x2
q6      = data3D[-1]
q6_rep  = np.array([[5,6], [7,8]])

# Q7 - Dans la premiere rangee de la matrice 2D, la soustraction des elements 2-5 et 1-4,
#      donc (2,3,4,5) - (1,2,3,4) = (1,1,1,1)
q7      = data2D[0][1:]-data2D[0][:-1]
q7_rep  = np.array([1,1,1,1])

### VeRIFICATION ###

qs = [q1,q2,q3,q4,q5,q6,q7]
rs = [q1_rep, q2_rep, q3_rep, q4_rep, q5_rep, q6_rep, q7_rep]
test(qs, rs)