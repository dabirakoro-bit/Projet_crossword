# Crossword Generator in C

## Description

Ce projet consiste à générer automatiquement une grille de mots croisés en langage C.
Le programme place un premier mot, puis tente de placer les autres mots du lexique
en utilisant une méthode brute force avec vérification des contraintes.

## Fonctionnalités

- Lecture d’un lexique depuis un fichier
- Trie des mots à placés
- Placement automatique du premier mot  alétoirement
- Recherche de correspondances entre les mots
- Vérification des débordements
- Vérification des cases voisines
- Placement horizontal et vertical

## Organisation du projet

.
├── crossword.c
├── lexicon.h
├── lexique
└── README.md

## Compilation

```bash
gcc main.c crossword.c -o crossword


## Execution 

```bash
 ./crossword

 utiliser cette commande de préférence pour redirigé le resultat dans un fichier text pour la lisibilité
 ./crossword > crossword.txt 

## Principe de l’algorithme ( methode brut force)

1. Placement du premier mot dans la grille
2. Recherche d’une lettre commune entre le mot à placer et la grille
3. Calcul de la position de départ
4. Test du débordement
5. Vérification des cases voisines
6. Placement du mot si les conditions sont respectées

## Améliorations possibles

- Optimisation de la recherche (réduction du brute force)
- Gestion de plusieurs croisements par mot
- Génération de grilles plus compactes
- Interface graphique

## Auteur

Dabira Koro  
Ngowet Yacynthe 

Etudiants à polytech lille 
