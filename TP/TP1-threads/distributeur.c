/******************************************************************************
**
** Version initiale.
** ----------------
** Ce programme se compose de trois fonctions. La fonction de distribution
** est chargée de lire les caractères en provenance de l'entrée standard,
** d'envoyer les chiffres à la fonction chiffre (par l'intermédiaire d'un
** appel de fonction classique) et les lettres à la fonction lettre (aussi par
** l'intermédiaire d'un appel de fonction).
** Une fois la fin de fichier atteinte, les fonctions renvoient leurs
** résultats à la fonction de distribution qui affiche ensuite les résultats.
**
** © 2002, Éric Renault pour l'Institut National des Télécommunications.
** © 2003, Denis Conan
** © 2005-2020, Martin Quinson
**
******************************************************************************/

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
** Somme des chiffres.
*/
int chiffre(int car) {
  static int somme = 0 ;
  
  if (car != EOF) {
    /* Somme des chiffres. */
    somme += (car - '0') ;
  } else {
    /* Renvoi du résultat. */
    return somme ;
  }
  return 0 ;
}

/*
** Fréquence des lettres.
*/
int* lettre(int car) {
  /* On suppose que le tableau est correctement initialisé. */
  static int frequence['z' - 'a' + 1] ;
  
  if (car != EOF) {
    /* Incrément des fréquences. */
    frequence [ tolower ( car ) - 'a' ] ++ ;
  } else {
    /* Renvoi du résultat. */
    return frequence ;
  }
  return NULL ;
}

/*
** Distribution des caractères et affichage des résultats.
*/
void distributeur() {
  int car, *frequence, i, somme ;
  
  /* Distribution des caractères. */
  do {
    car = getchar() ;
    if (isalpha(car) || (car == EOF)) {
      frequence = lettre ( car ) ;
    }
    if (isdigit(car) || (car == EOF)) {
      somme = chiffre(car);
    }
  } while(car != EOF);
  
  /* Lecture et affichage de la somme. */
  printf("Somme : %d", somme);
  
  /* Lecture et affichage des frequences. */
  for(i = 'a' ; i <= 'z' ; i ++) {
    printf(" ; %c : %d", i, frequence[i - 'a']) ;
  }
  printf("\n");
}

/*
** Lancement de la fonction de distribution.
*/
int main(int argc, char* argv[]) {
  /* Lancement du travail. */
  distributeur();
  
  /* Tout s'est bien terminé. */
  exit(0);
}
