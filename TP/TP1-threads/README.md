# TP1 : Threads POSIX

## Exercice 1: Comprendre du code avec des threads

#### Question 1: Que s'est-il passé ?

Les deux threads ont obtenus le verrou l'un après l'autre permettant d'attribuer à `i` la même valeur de `count`.
Même si le compteur s'incrémente deux fois par la suite, les deux threads vont pouvoir prétendre avoir incrémenter le même multiple de 10000.

#### Question 2: Comment corriger le problème ?

Pour corriger le problème, il suffit de mettre l'obtention du compteur et son incrémentation dans la même section critique. C'est ce qui est fait dans le code en annexe dans la fonction `getCount()`.

