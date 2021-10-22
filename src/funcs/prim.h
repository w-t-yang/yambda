#ifndef YAMBDA_FUNCS_PRIM
#define YAMBDA_FUNCS_PRIM

#include "../types.h"

Element *prim_quote(Element *x);
Element *prim_atom(Element *x);
Element *prim_eq(Element *x);
Element *prim_car(Element *x);
Element *prim_cdr(Element *x);
Element *prim_cons(Element *x);
Element *prim_cond(Element *x);

#endif
