#include "allocateur.h"
#include "erreur.h"
#include <stdlib.h>
#include "environnement.h"
#include "types.h"

void* valisp_malloc(size_t size){
    void* ptr;
    ptr = allocateur_malloc(size);
    if(ptr==NULL){
        ERREUR_FATALE("Pointeur Null renvoyé");
        exit(1);
    }else{
        return ptr;
    }
}

void ramasse_miette_parcourir_et_marquer(sexpr s){
    sexpr liste,a,nom,val;
    for (liste=s; liste!=NULL ; liste=cdr(liste)){
        a=car(liste);
        nom = car(a);
        val=cdr(a);
        if(ramasse_miettes_lire_marque(a)==0){
            ramasse_miettes_poser_marque(a);
        }
        if(ramasse_miettes_lire_marque(nom)==0){
            ramasse_miettes_poser_marque(nom);
        }
        if(ramasse_miettes_lire_marque(val)==0){
            ramasse_miettes_poser_marque(val);
        }
    }
}

void valisp_ramasse_miettes(sexpr env){
    ramasse_miette_parcourir_et_marquer(env);
    ramasse_miettes_liberer();
}