#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "types.h"
#include "erreur.h"
#include "environnement.h"
#include "interpreteur.h"

void test_nb_parametres(sexpr liste, char* fonction, int taille){
    int compteur=1;
    while(cdr(liste)!=NULL){
        compteur+=1;
        liste=cdr(liste);
    }
    if(compteur<taille){
        erreur(ARITE, fonction, "Il manque des arguments",liste);
    }else if(compteur>taille){
        erreur(ARITE, fonction, "Trop d'argument",liste);
    }
}

sexpr add_valisp(sexpr liste, sexpr env){
    sexpr a;
    sexpr b;
    /* On vérifie l’arité */
    test_nb_parametres(liste, "+", 2);
    /* On récupère les deux paramètres */
    a = car(liste);
    b = car(cdr(liste));
    /* On vérifie de typage */
    if (!integer_p(a)) erreur(TYPAGE, "+", "nécessite un entier", a);
    if (!integer_p(b)) erreur(TYPAGE, "+", "nécessite un entier", b);
    /* On fait le calcul */
    return new_integer(get_integer(a) + get_integer(b));
}

sexpr defvar_valisp(sexpr liste, sexpr env) {
    sexpr nom;
    sexpr exp;
    sexpr res;
    test_nb_parametres(liste,"defvar",2);
    nom = car(liste);
    exp = car(cdr(liste));
    if (!symbol_p(nom)) {
        erreur(TYPAGE,"defvar","Le 1er paramètre doit être un symbole",nom);
    }
    res = eval(exp, env); /* Il faut évaluer à la main le second paramètre */
    definir_variable_globale(nom,res); 
    return res;
}

sexpr setq_valisp(sexpr liste, sexpr env){
    sexpr nom;
    sexpr exp;
    sexpr res;
    test_nb_parametres(liste,"setq",2);
    nom = car(liste);
    exp = car(cdr(liste));
    if (!symbol_p(nom)) {
        erreur(TYPAGE,"setq","Le 1er paramètre doit être un symbole",nom);
    }
    res = eval(exp, env); /* Il faut évaluer à la main le second paramètre */
    modifier_variable(env,nom,res); 
    return res;
}