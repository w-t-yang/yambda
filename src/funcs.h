#ifndef YAMBDA_FUNCS
#define YAMBDA_FUNCS

#include "types.h"

void init_funcs(Env *env);
Element *apply(Env *env, Element *lambda, Element *args);
Element *eval(Env *env, Element *head);

#endif
