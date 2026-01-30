#include "types.h"
#include "memoire.h"
#include "erreur.h"
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

/*création des valisp_sexpr / valisp_object*/
typedef enum type {entier,chaine,symbole,Cons,prim,spec} valisp_types;

typedef struct cons{
    sexpr car;
    sexpr cdr;
} valisp_cons;

typedef union{
    int INTEGER;
    char* STRING;
    valisp_cons CONS;
    sexpr (*PRIMITIVE) (sexpr,sexpr);
}valisp_data;

struct valisp_object{
    valisp_types type;
    valisp_data data;
};

/*On code les entiers*/
sexpr new_integer(int i){
    sexpr new;
    valisp_data data;
    new =valisp_malloc(sizeof(struct valisp_object));
    new->type=entier;
    data.INTEGER = i;
    new->data=data;
    return new;
}

bool integer_p(sexpr val){
    if(val==NULL){
        return 0;
    }else{
        return val->type == 0;
    }
}

int get_integer(sexpr val){
    return val->data.INTEGER;
}

/*On code les chaînes et symboles*/
char* chaine_vers_memoire(const char*c){
    char* ptr;
    ptr=valisp_malloc(strlen(c)+1);
    strcpy(ptr,c);
    return ptr;
}

sexpr new_string(char*c){
    sexpr new;
    valisp_data data;
    new =valisp_malloc(sizeof(struct valisp_object));
    new->type=chaine;
    data.STRING = chaine_vers_memoire(c);
    new->data=data;
    return new;
}

sexpr new_symbol(char*c){
    sexpr new;
    valisp_data data;
    if(strcmp("nil",c)==0){
        return NULL;
    }else{
        new =valisp_malloc(sizeof(struct valisp_object));
        new->type=symbole;
        data.STRING = chaine_vers_memoire(c);
        new->data=data;
        return new;
    }
}

char* get_symbol(sexpr val){
    return val->data.STRING;
}
char* get_string(sexpr val){
    return val->data.STRING;
}

bool string_p(sexpr val){
    if(val==NULL){
        return 0;
    }else{
        return val->type == 1;
    }
}
bool symbol_p(sexpr val){
    if(val==NULL){
        return 0;
    }else{
        return val->type == 2;
    }
}

bool symbol_match_p(sexpr val, const char *chaine){
    char* c;
    int res;
    c=get_symbol(val);
    res=strcmp(c,chaine);
    return res==0;
}

/*On code les listes*/
sexpr cons(sexpr e1, sexpr e2){
    sexpr new;
    valisp_data data;
    valisp_cons c;
    new =valisp_malloc(sizeof(struct valisp_object));
    new->type=3;
    c.car=e1;
    c.cdr = e2;
    data.CONS = c;
    new->data=data;
    return new;
}

bool cons_p(sexpr val){
    if(val==NULL){
        return 0;
    }else{
        return val->type == 3;
    }
}

bool list_p(sexpr e){
    return ((cons_p(e)==1 && (cons_p(cdr(e))==1 || cdr(e)==NULL)) || NULL==e);
}

sexpr car(sexpr e){
    if(e==NULL){
        ERREUR_FATALE("e est NULL\n");
    }
    return e->data.CONS.car;
}

sexpr cdr(sexpr e){
    if(e==NULL){
        ERREUR_FATALE("e est NULL\n");
    }
    return e->data.CONS.cdr;
}

void set_car(sexpr e, sexpr nouvelle){
    e->data.CONS.car = nouvelle;
}

void set_cdr(sexpr e, sexpr nouvelle){
    e->data.CONS.cdr = nouvelle;
}

void afficher_liste(sexpr e){
    afficher(car(e));
    if(cons_p(cdr(e))){
        printf(" ");
        afficher_liste(cdr(e));
    }else if(cdr(e)!=NULL){
        printf(" . ");
        afficher(cdr(e));
    }
    /*Si cdr(e) est NULL on quitte la fonction*/
}

/*On code les primitives et les spéciales*/
sexpr new_primitive(sexpr (*p)(sexpr,sexpr)){
    sexpr new;
    valisp_data data;
    new =valisp_malloc(sizeof(struct valisp_object));
    new->type=4;
    data.PRIMITIVE=(*p);
    new->data=data;
    return new;
}

sexpr new_speciale(sexpr (*p)(sexpr,sexpr)){
    sexpr new;
    valisp_data data;
    new =valisp_malloc(sizeof(struct valisp_object));
    new->type=5;
    data.PRIMITIVE=p;
    new->data=data;
    return new;
}

bool prim_p(sexpr val){
    if(val==NULL){
        return 0;
    }else{
        return val->type == 4;
    }
}

bool spec_p(sexpr val){
    if(val==NULL){
        return 0;
    }else{
        return val->type == 5;
    }
}

sexpr run_prim(sexpr p, sexpr l, sexpr env){
    sexpr res;
    res = p->data.PRIMITIVE(l,env);
    return res;
}

void afficher(sexpr val){
    if(integer_p(val)){
        int i;
        i=get_integer(val);
        printf("%i",i);
    } else if(string_p(val) || symbol_p(val)){
        char* c;
        c=get_string(val);
        printf("%s",c);
    }else if(cons_p(val)){
        printf("(");
        afficher_liste(val);
        printf(")");
    }else if(prim_p(val)){
        printf("#<primitive>\n");
    }else if(spec_p(val)){
        printf("#<speciale>\n");
    }
}
