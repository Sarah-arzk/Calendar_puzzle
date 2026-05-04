# Calendrier Puzzle Solver

## Description
Ce projet est un solveur du puzzle calendrier basé sur le backtracking et les bitmasks.

Le programme place 10 pièces différentes sur une grille 8x8 afin de remplir un calendrier en fonction d’une date donnée (jour, mois, weekday).

Le solveur utilise également le multithreading pour accélérer les calculs.

---

## Auteurs

- AREZKI Sarah 

---

## Structure du projet

- main.c : interface utilisateur, affichage et exécution
- solver.c : algorithme de résolution + backtracking + threads
- global.c : variables globales et données du calendrier
- calendrier_puzzle_projet.h : structures et constantes
- Makefile : compilation

---
## Résultats

Lors de l’exécution, le programme affiche :

- Le nombre total de solutions trouvées
- Le temps d’exécution du solveur

Exemple obtenu sur une machine standard :

- Nombre de solutions : 1518  
- Temps d'exécution : 7.170 secondes  

Ces valeurs peuvent varier selon :
- la machine utilisée
- le nombre de threads
- la date choisie

---

## Compilation

```bash
make
