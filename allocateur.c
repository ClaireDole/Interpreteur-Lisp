#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "allocateur.h"
#include "erreur.h"

typedef uint32_t bloc;

#define TAILLE_MEMOIRE_DYNAMIQUE (1 << 15) /*2^15*/

bloc MEMOIRE_DYNAMIQUE[TAILLE_MEMOIRE_DYNAMIQUE];

void initialiser_memoire_dynamique(){
    MEMOIRE_DYNAMIQUE[0]=(1<<15)-1;
    MEMOIRE_DYNAMIQUE[TAILLE_MEMOIRE_DYNAMIQUE-1]=(1<<16)-1;
}

bloc cons_bloc(int rm, int precedant, int libre, int suivant){
    bloc new;
    new=(rm<<31);
    new=new | (precedant<<16);
    new = new | (libre<<15);
    new = new | suivant;
    return new;
}

/*indice du bloc precedant*/
int bloc_precedant(int i){
    bloc b;
    b=MEMOIRE_DYNAMIQUE[i];
    b=b>>16;
    b=b&((1<<15)-1);
    return b;
}

/*indice du bloc suivant*/
int bloc_suivant(int i){
    bloc b;
    b=MEMOIRE_DYNAMIQUE[i];
    b=b&((1<<15)-1);
    return b;
}

/*indice du bloc pour savoir s'il est libre*/
int usage_bloc(int i){
    bloc b;
    b=MEMOIRE_DYNAMIQUE[i];
    b=(b>>15)&1;
    return b;
}

/*indice pour le ramasse-miette*/
int rm_bloc(int i){
    bloc b;
    b=MEMOIRE_DYNAMIQUE[i];
    b=b>>31;
    return b&1;
}

/*taille en nombre de case*/
int taille_bloc(int i){
    int taille;
    if(bloc_suivant(i)==i){
        return 0;
    }
    else{
        taille = bloc_suivant(i)-i -1;
    }
    return taille;
}

int rechercher_bloc_libre (size_t size){
    int i=0;
    for(i=0;i!=bloc_suivant(i);i=bloc_suivant(i)){
        if(!usage_bloc(i) && (taille_bloc(i)>= size)){ /*+1 pour l'en-tête du bloc et conversion octets en blocs*/
            return i;
        }
    }
    return -1;
}

void *allocateur_malloc(size_t size){
    int i,n;
    bloc k,j,l;
    /*On redéfinit size pour qu'elle soit en bloc et non en octet*/
    if(size%sizeof(bloc)==0){
        size = size/sizeof(bloc);
    }else{
        size= size/sizeof(bloc)+1;
    }
    i=rechercher_bloc_libre(size);
    if(i==-1){
        return NULL; /*Pas de bloc libre disponible*/
    }
    if(taille_bloc(i)==size){
        /*k-->i-->j*/
        k=bloc_precedant(i);
        j=bloc_suivant(i);
        MEMOIRE_DYNAMIQUE[i]=cons_bloc(0,k,1,j);
    } else{
        /*avant : k-->i-->j-->l
            après : k-->i-->n-->j-->*/
        k=bloc_precedant(i);
        j=bloc_suivant(i);
        l=bloc_suivant(j);
        n=i+size+1;
        MEMOIRE_DYNAMIQUE[i]=cons_bloc(rm_bloc(i),k,1,n);
        MEMOIRE_DYNAMIQUE[n]=cons_bloc(0,i,0,j);
        MEMOIRE_DYNAMIQUE[j]=cons_bloc(rm_bloc(j),n,1,l);
    }
    return &MEMOIRE_DYNAMIQUE[i+1];
}

/*CODE ENSEIGNANT
***************************************************************/
void info_bloc(size_t i) {
    int t = taille_bloc(i);
    int p = bloc_precedant(i);
    int s = bloc_suivant(i);
    char * u = usage_bloc(i) ? "[x]" : "[ ]";

    printf("%s %p %5d → %ld → %-5d (%u)\n",
           u, (void *) &MEMOIRE_DYNAMIQUE[i], p, i, s, t);
}

void afficher_memoire() {
    int i;
    for (i=0; bloc_suivant(i) != i; i = bloc_suivant(i)) {
	printf("[%5d] ",i);
	info_bloc(i);
    }
    printf("[%5d] ",i);
    info_bloc(i);
}

void afficher_stat_memoire() {
    size_t i = 0;
    int t = TAILLE_MEMOIRE_DYNAMIQUE;
    int n = 0;
    int b = 0;
    int a = 0;
    for (i=0; bloc_suivant(i) != i; i = bloc_suivant(i)) {
        n+=1;
        if (usage_bloc(i)) {
            a += taille_bloc(i) + 1;
            b += 1;
        }
    }
    printf("%d/%d (%.2f %%) %d/%d blocs alloués", a, t, (100.*a)/t, b,n);
}

void afficher_stat_memoire_bref() {
    size_t i = 0;
    int t = TAILLE_MEMOIRE_DYNAMIQUE;
    int n = 0;
    int b = 0;
    int a = 0;
    for (i=0; bloc_suivant(i) != i; i = bloc_suivant(i)) {
        n+=1;
        if (usage_bloc(i)) {
            a += taille_bloc(i) + 1;
            b += 1;
        }
    }
    printf("%.2f%%", (100.*a)/t);
}
/*CODE ENSEIGNANT********************************************/

int pointeur_vers_indice(void *ptr){
    int i = (bloc*)ptr - MEMOIRE_DYNAMIQUE - 1;
    if (i<0 || i>=TAILLE_MEMOIRE_DYNAMIQUE){
        ERREUR_FATALE("indice invalide");
    }
    return i;
}

int ramasse_miettes_lire_marque(void *ptr){
    int i;
    i=pointeur_vers_indice(ptr);
    return rm_bloc(i);
}

int ramasse_miettes_poser_marque(void *ptr){
    int i;
    if(ramasse_miettes_lire_marque(ptr)==1){
        ERREUR_FATALE("Le bloc est déjà marqué par le ramasse-miette");
    }
    i = pointeur_vers_indice(ptr);
    MEMOIRE_DYNAMIQUE[i]=cons_bloc(1,bloc_precedant(i),usage_bloc(i),bloc_suivant(i));
    return 0;
}

int bloc_libre(i){
    if(bloc_suivant(i)==i){
        return 0;
    }else if(rm_bloc(i)==1){
        return 1;
    }
    return 0;
}

void ramasse_miettes_liberer(void){
    int i,j;
    for(i=0; bloc_suivant(i) != i; i = bloc_suivant(i)){
        if(bloc_libre(i)){
            for(j=0; bloc_suivant(j)!=j;j=bloc_suivant(j)){
                if(!bloc_libre(j)){
                    MEMOIRE_DYNAMIQUE[i]=cons_bloc(0,bloc_precedant(i),0,j);
                    MEMOIRE_DYNAMIQUE[j]= cons_bloc(1,i,usage_bloc(j),bloc_suivant(j));
                }
            }
        }else if(!bloc_libre(i)){
            MEMOIRE_DYNAMIQUE[i]=cons_bloc(0,bloc_precedant(i),usage_bloc(i), bloc_suivant(i));
        }
    }
}


