# TP1 : Threads POSIX

## Exercice 1: Comprendre du code avec des threads

#### Question 1: Que s'est-il passé ?

Les deux threads ont obtenus le verrou l'un après l'autre permettant d'attribuer à `i` la même valeur de `count`.
Même si le compteur s'incrémente deux fois par la suite, les deux threads vont pouvoir prétendre avoir incrémenter le même multiple de 10000.

#### Question 2: Comment corriger le problème ?

Pour corriger le problème, il suffit de mettre l'obtention du compteur et son incrémentation dans la même section critique. C'est ce qui est fait dans le code en annexe dans la fonction `getCount()`.

## Exercice 2: Écrire de petits programmes avec des threads

En suivant les questions du sujet jusqu'à la question optionelle 6, on obtient le code en annexe.

En évitant `pthread_exit()`, élimine le psuedo-mem leak de valgrind (bug connu).

`atoi()` est préféré à `sscanf("%d", ...)` dans la lecture des arguments pour éviter de parser la chaîne de formattage.

L'utilisation du mutex n'est nécessaire que lors de l'incrémentation de la somme partielle de chaque thread à la somme globale.

## Exercice 3: Communication inter-threads

Pour des raisons de simplification de passage d'arguments, c'est le `distributeur` qui va créer deux threads fils `chiffre` et `lettres`.

On aurai bien pu déplacer le code de `distributeur` dans `main` et éviter la création d'un thread mais cela aurai eu pour effet de changer le skelette de code donné pour l'exercice.

Pour la communication inter-threads, nous sommes dans un cas classique de producteur/consommateur dans lequel le producteur, ici `distributeur`, répartie les produits entre deux consommateurs, `chiffre` et `lettres`.

L'idée va alors d'alouer un espace mémoire afin que le producteur puisse déposer ses produits qui seront consomés par le consommateur sans être bloquer.

Si cela peut se faire avec une variable et deux mutex en attente passive, on en atteint vite les limitations. 

On va donc opter pour une solution plus compliquée mais intéressante, l'allocation d'un espace mémoire circulaire `syncbuffer`. On utilise alors des sémaphores plutôt que des mutex pour leur compteurs.

Le fait de disposer d'un seul producteur/consomateur par produit permet d'éviter de devoir poser des sections critiques sur les têtes d'écriture et de lecture du tableau circulaire.


