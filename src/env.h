#ifndef YAMBDA_ENV
#define YAMBDA_ENV

#include "types.h"

Env *env_init();

Element *env_get(Env *env, char *symbol);
Element *env_set(Env *env, char *symbol, Element *list);
void env_unset(Env *env, char *symbol);

#endif
