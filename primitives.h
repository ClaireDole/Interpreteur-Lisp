#include "types.h"
#include "interpreteur.h"
#include <setjmp.h>

#ifndef VALISP_PRIMITIVES_H
#define VALISP_PRIMITIVES_H

sexpr add_valisp(sexpr liste, sexpr env);
sexpr test_nb_parametes(sexpr liste, char* fonction, int taille);
sexpr defvar_valisp(sexpr liste, sexpr env);
sexpr setq_valisp(sexpr liste, sexpr env);
#endif