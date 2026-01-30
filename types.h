#include <setjmp.h>
#ifndef TYPES_H
#define TYPES_H

typedef int bool;
/*déclaration de la struct valisp_object*/
struct valisp_object;
typedef struct valisp_object* sexpr;

/*déclaration des fonctions entier*/
sexpr new_integer(int i);
bool integer_p(sexpr val);
int get_integer(sexpr val);
void afficher(sexpr val);

/*Déclaration des fonctions chaînes et symboles*/
sexpr new_string(char* c);
sexpr new_symbol(char* c);
char* get_symbol(sexpr val);
char* get_string(sexpr val);
bool string_p(sexpr val);
bool symbol_p(sexpr val);
bool symbol_match_p(sexpr val, const char* chaine);

/*Déclaration des fonctions liste*/
sexpr cons(sexpr e1, sexpr e2);
bool cons_p(sexpr e);
bool list_p(sexpr e);
sexpr car(sexpr e);
sexpr cdr(sexpr e);
void set_car(sexpr e, sexpr nouvelle);
void set_cdr(sexpr e, sexpr nouvelle);
void afficher_liste(sexpr e);

/*Déclaration des fonctions primitives et spéciales*/
sexpr new_primitive(sexpr (*p)(sexpr,sexpr));
sexpr new_speciale(sexpr (*p)(sexpr,sexpr));
bool prim_p(sexpr val);
bool spec_p (sexpr val);
sexpr run_prim(sexpr p, sexpr liste, sexpr env);

#endif