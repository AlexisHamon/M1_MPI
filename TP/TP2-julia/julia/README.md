# TP2 : Julia, threads, et répartition de charge
### Programmation multi-threadée

## Exercice 1: La fractale de Julia

Nous nous retrouvons dans le cas classique du pavage d'un domaine sans vecteur de dépendance.
On peut donc parraléliser à la volée comme bon nous le semble.

On définit une structure d'itérateur, `julia_it`, composée d'un mutex pour l'attribution des pavés.

Tant que le travail n'est pas fini `julia_it->i = 0`, chaque thread `threadCalc` va obtenir un numéro de pavé,
récupérer les bornes de ce dernier grâce aux fonctions `juliaGetXmin`, `juliaGetXmax`, `juliaGetYmin` et `juliaGetYmax`.

Toute l'image est alors parcourue via une attribution dynamique des pavés aux différents threads.

Bien que l'affirmation de la question 5 est vraie, un autre facteur important est l'optimisation du cache propre à chaque machine.
Le compromis souhaité se trouve valoir sur ma machine des pavés de taille 32. 