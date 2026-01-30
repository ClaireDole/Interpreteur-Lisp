#include <stdio.h>
#include "couleurs.h"
#include "types.h"
#include "stdlib.h"
#include "couleurs.h"
#include <setjmp.h>

/**********************/
/*                    */
/*  Erreurs Fatales   */
/*                    */
/**********************/

/* Pour s’arrêter lorsque le développeur tombe sur cas qui ne devrait
   jamais arriver. Normalement, si le programme est écrit sans bugs,
   les erreurs fatales ne devrait jamais être rencontrées. */

int erreur_fatale(char  *fichier, int ligne, char* causes) {
    fprintf(stderr,"%s", couleur_rouge);
    fprintf(stderr,"\n   /!\\ Erreur fatale /!\\");
    fprintf(stderr,"%s", couleur_defaut);
    fprintf(stderr,"   %s ligne %d\n\n", fichier, ligne);
    fprintf(stderr,"%s\n", causes);
    exit(1);
}

enum erreurs {
    TYPAGE, /* Paramètre du mauvais type */
    ARITE, /* Mauvais nombres de paramètres */
    NOM, /* Variables non définie */
    MEMOIRE, /* Plus de mémoire */
    DIVISION_PAR_ZERO, /* Tentative d’invocation de l’infinie */
    SYNTAXE, /* Parseur : le code écrit n’est pas du Lisp */
    MEMOIRE_PARSEUR, /* Parseur : la sexpr est trop grosse */
    RUNTIME /* Vos propres erreurs lancée depuis valisp */
};

sexpr SEXPR_ERREUR;
char* FONCTION_ERREUR;
char* MESSAGE_ERREUR;
enum erreurs TYPE_ERREUR;

void affiche_Type_erreur(void){
    switch (TYPE_ERREUR){
        case TYPAGE :
            printf("%sTYPAGE%s",couleur_rouge,couleur_rouge);
            break;
        case ARITE :
            printf("%sARITE%s",couleur_rouge,couleur_rouge);
            break;
        case NOM :
            printf("%sNOM%s",couleur_rouge,couleur_rouge);
            break;
        case MEMOIRE :
            printf("%sMEMOIRE%s",couleur_rouge,couleur_rouge);
            break;
        case DIVISION_PAR_ZERO :
            printf("%sDIVISION_PAR_ZERO%s",couleur_rouge,couleur_rouge);
            break;
        case SYNTAXE :
            printf("%sSYNTAXE%s",couleur_rouge,couleur_rouge);
            break;
        case MEMOIRE_PARSEUR :
            printf("%sMEMOIRE_PARSEUR%s",couleur_rouge,couleur_rouge);
            break;
        case RUNTIME :
            printf("%sRUNTIME%s",couleur_rouge,couleur_rouge);
            break;
        default :
            break;
    }
}

void afficher_erreur(void){
    printf("%sErreur d'exécution [%s",couleur_rouge,couleur_rouge);
    affiche_Type_erreur();
    printf("%s] :  %s\n",couleur_rouge,couleur_rouge);
    printf("%sFonction fautive : \"%s\"%s\n",couleur_rouge,FONCTION_ERREUR,couleur_rouge);
    printf("%sValeur fautive : %s",couleur_rouge,couleur_rouge);
    afficher(SEXPR_ERREUR);
    printf("%sMessage : %s%s\n",couleur_rouge,MESSAGE_ERREUR,couleur_rouge);
}
jmp_buf buf;

jmp_buf *jump_buffer(){
    return &buf;
}
void erreur(enum erreurs type, char* fonction, char* explication, sexpr s){
    TYPE_ERREUR=type;
    FONCTION_ERREUR=fonction;
    MESSAGE_ERREUR=explication;
    SEXPR_ERREUR=s;
    afficher_erreur();
    longjmp(buf,1);
}
