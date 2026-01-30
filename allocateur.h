#include <stdlib.h>
#include <setjmp.h>
#ifndef ALLOCATEUR_H
#define ALLOCATEUR_H

void initialiser_memoire_dynamique(void);
void *allocateur_malloc(size_t size);
int pointeur_vers_indice(void *ptr);
int ramasse_miettes_lire_marque(void *ptr);
int ramasse_miettes_poser_marque(void *ptr);
void ramasse_miettes_liberer(void);
void allocateur_free(void *ptr);

int taille_bloc(int i);
int bloc_precedant(int i);
int bloc_suivant(int i);
int usage_bloc(int i);

void info_bloc(size_t i) ;
void afficher_memoire(void);
void afficher_stat_memoire(void);
void afficher_stat_memoire_bref(void);

#endif