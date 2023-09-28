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
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct syncbuffer {
  unsigned length;
  sem_t s_filled;
  sem_t s_empty;
  unsigned r_head;
  unsigned w_head;
  char *buffer;
} syncbuffer;

syncbuffer *syncbufferAlloc(unsigned value) {
  syncbuffer *sbuf = malloc(sizeof(syncbuffer));
  sbuf->length = value;
  sem_init(&sbuf->s_filled, 0, 0);
  sem_init(&sbuf->s_empty, 0, value);
  sbuf->r_head = 0;
  sbuf->w_head = 0;
  sbuf->buffer = malloc(value * sizeof(char));
  return sbuf;
}

void syncbufferFree(syncbuffer *sbuf) {
  sem_destroy(&sbuf->s_filled);
  sem_destroy(&sbuf->s_empty);
  free(sbuf->buffer);
  free(sbuf);
}

void syncbufferAppend(syncbuffer *sbuf, char car) {
  sem_wait(&sbuf->s_empty);
  sbuf->buffer[sbuf->r_head] = car;
  sbuf->r_head = (sbuf->r_head + 1) % sbuf->length;
  sem_post(&sbuf->s_filled);
}

char syncbufferGet(syncbuffer *sbuf) {
  sem_wait(&sbuf->s_filled);
  char car = sbuf->buffer[sbuf->w_head];
  sbuf->w_head = (sbuf->w_head + 1) % sbuf->length;
  sem_post(&sbuf->s_empty);
  return car;
}

/*
** Somme des chiffres.
*/
void *chiffre(void *ptr) {
  syncbuffer *chiffre_buffer = (syncbuffer *)ptr;

  static int somme = 0;
  char car;

  while ((car = syncbufferGet(chiffre_buffer)) != EOF) {
    somme += (car - '0');
  };

  return (void *)&somme;
}

/*
** Fréquence des lettres.
*/
void *lettre(void *ptr) {
  syncbuffer *lettre_buffer = (syncbuffer *)ptr;

  static int frequence['z' - 'a' + 1];
  char car;

  while ((car = syncbufferGet(lettre_buffer)) != EOF) {
    frequence[tolower(car) - 'a']++;
  };

  return (void *)frequence;
}

/*
** Distribution des caractères et affichage des résultats.
*/
void *distributeur(void *ignored) {
  /* Création des Threads Concurrents */

  syncbuffer *chiffre_buffer = syncbufferAlloc(512);
  pthread_t pth_chiffre;
  pthread_create(&pth_chiffre, NULL, chiffre, (void *)chiffre_buffer);

  syncbuffer *lettre_buffer = syncbufferAlloc(512);
  pthread_t pth_lettre;
  pthread_create(&pth_lettre, NULL, lettre, (void *)lettre_buffer);

  /* Distribution des caractères. */
  int car;
  do {
    car = getchar();
    if (isalpha(car) || (car == EOF)) {
      syncbufferAppend(lettre_buffer, car);
    }
    if (isdigit(car) || (car == EOF)) {
      syncbufferAppend(chiffre_buffer, car);
    }
  } while (car != EOF);

  /* Attente des Threads Concurrents */
  int *somme_ptr;
  pthread_join(pth_chiffre, (void *)&somme_ptr);
  syncbufferFree(chiffre_buffer);
  int *frequence_ptr;
  pthread_join(pth_lettre, (void *)&frequence_ptr);
  syncbufferFree(lettre_buffer);

  /* Lecture et affichage de la somme. */
  printf("Somme : %d\n", *somme_ptr);

  /* Lecture et affichage des frequences. */
  for (int i = 'a'; i <= 'z'; i++) {
    printf(" ; %c : %d", i, frequence_ptr[i - 'a']);
  }
  printf("\n");

  return NULL;
}

/*
** Lancement de la fonction de distribution.
*/
int main(int argc, char *argv[]) {
  /* Lancement du travail. */
  pthread_t pth_distrib;
  pthread_create(&pth_distrib, NULL, distributeur, NULL);

  /* Attente du travail. */
  pthread_join(pth_distrib, NULL);

  /* Tout s'est bien terminé. */
  exit(0);
}
