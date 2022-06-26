# TP Architecture parrallèle
Candice Astier
22 janvier 2022

## Introduction

Le but de ce TP est d'optimiser le programme `nbody0.c`. Pour cela, il a été créé plusieurs versions de plus en plus performantes.


## Organisation du dossier

Ce dossier comprends:
    - un fichier `Rapport_ASTIER_CANDICE.pdf`: qui présente le TP ainsi que les résultats calculés,
    - un script `run.sh`: qui permet de générer le taux de performances (Gflops/s) de chacune des différentes versions de `nbody0.c`;
    - un sous-dossier `src`: qui comprends toutes les versions du programme (`nbody0.c`, `nbody1.c`, `nbody2.c`, `nbody3.c`, `nbody4.c`, `nbody5.c`, `nbody6.c`, `nbody7.c`, `nbody8.c`) + un `Makefile` qui permet de compiler et executer les codes;
    - un fichier `perf_plot.gp`: qui permet de générer un histogramme des performances (cf. partie : Création des figures);
    - un script `assembly_stats.sh`: qui permet de compter le nombre d'occurrences de certaines instructions dans le code assembleur des différentes versions de `nbody0.c`;
    - un scipt `extract.py`: qui permet d'extraire du code assembleur (fichiers .s générés) seulement la fonction move_particle pour des tests plus performants,
    - un sous-dossier `assembly_data`: qui permet de stocker l'ensemble des données générés par `assembly_stats.sh`;
    - un fichier `assembly_plot.gp`: qui permet de générer une figure des taux d'occurrence (cf. partie : Création des figures);
    - un sous-dossier `annexes`: qui contient 
    	* des données de comparaisons entre les deux compilateurs (gcc et icc), 
	* un rapport de l'outil MAQAO sur la version la plus optimisée du programme (nbody8),
	* un rapport de l'outil Perf sur la version la plus optimisée du programme (nbody8) et sur la version originale non optimisée (nbody0),
	* des informations sur le CPU utilisé.


## Création des figures
Pour pouvoir créer les figures, il faut dans un premier temps générer les données grâce à la commande:
```sh
./<nom_du_script.sh>
```
Avec: "run.sh" pour l'étude des performances, et "assembly_stats.sh" pour l'étude du code assembleur.

Puis, la commande pour générer les figures (au format .png) est:
```sh
gnuplot <nom_du_fichier.gp>
```
Avec: "perf_plot.gp" pour l'étude des performances, et "assembly_plot.gp" pour l'étude du code assembleur.



