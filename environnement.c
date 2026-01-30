#include "types.h"
#include "allocateur.h"
#include "couleurs.h"
#include "primitives.h"
#include <stdio.h>

sexpr ENV;

sexpr environnement_global(void){
    return ENV;
}

void initialiser_memoire(void){
    sexpr t;
    sexpr liste;
    initialiser_memoire_dynamique();
    t=new_symbol("t");
    liste=cons(t,t);
    /*Il faut affecter à ENV une liste chaînée...*/
    ENV=cons(liste,NULL);
}

int trouver_variable(sexpr env, sexpr variable, sexpr *resultat){
    sexpr a, liste, nom, val;
    for (liste=env; liste!=NULL ; liste=cdr(liste)) {
        a = car(liste); /* a est de la forme (nom . valeur) */
        nom = car(a);
        val = cdr(a);
        if(symbol_match_p(variable,get_symbol(nom))){
            *resultat=val;
            return 0;
        }
    }
    return -1;
}

int modifier_variable(sexpr env, sexpr variable, sexpr valeur){
    sexpr a, liste, nom;
    for (liste=env; liste!=NULL ; liste=cdr(liste)) {
        a = car(liste); /* a est de la forme (nom . valeur) */
        nom = car(a);
        if(symbol_match_p(nom,get_symbol(variable))){
            set_cdr(a,valeur);
            return 0;
        }
    }
    return -1;
}

void definir_variable_globale(sexpr variable, sexpr valeur){
    int valid=0;
    sexpr a, liste, nom, new,sauv;
    for (liste=environnement_global(); liste!=NULL ; liste=cdr(liste)) {
        a = car(liste); /* a est de la forme (nom . valeur) */
        nom = car(a);
        if(symbol_match_p(nom,get_symbol(variable))){
            set_cdr(a,valeur);
            valid=1;
            break;
        }
        sauv=liste;
    }
    if(valid==0){/*on a pas trouvé une variable portant ce nom*/
        new=cons(cons(variable,valeur),NULL);
        set_cdr(sauv,new);
    }
}

void charger_une_primitive(char* nom, sexpr (*prim)(sexpr, sexpr)){
    sexpr name;
    name = new_symbol(nom);
    afficher(name);
    printf("\n");
    definir_variable_globale(name,new_primitive(prim));
}
void charger_une_speciale(char* nom, sexpr (*prim)(sexpr, sexpr)){
    sexpr new = new_speciale(prim);
    sexpr name = new_symbol(nom);
    definir_variable_globale(name,new);
}

/*Affichage des informations de l'environnement*/
int longueur_env(sexpr env){
    sexpr liste;
    int compteur=0;
    for(liste=env;liste!=NULL;liste=cdr(liste)){
        compteur+=1;
    }
    return compteur;
}

void valisp_stat_memoire(void){
    afficher_stat_memoire();
    printf(" -> %i variables\n",longueur_env(environnement_global()));
}

void afficher_env(sexpr env){
    sexpr liste,a,nom,val;
    for(liste=env;liste!=NULL;liste=cdr(liste)){
        a=car(liste);
        nom=car(a);
        val=cdr(a);
        printf("%s%s%s ",couleur_bleu,get_symbol(nom),couleur_bleu);
        if(integer_p(val)){
            printf("%s%i%s\n",couleur_vert,get_integer(val),couleur_vert);
        }else if(symbol_p(val) || string_p(val)){
            printf("%s%s%s\n",couleur_vert,get_string(val),couleur_vert);
        }else if(prim_p(val)){
            printf("%s#<primitive>%s\n",couleur_vert,couleur_vert);
        }else if(spec_p(val)){
            printf("%s#<speciale>%s\n",couleur_vert,couleur_vert);
        }
    }
}
