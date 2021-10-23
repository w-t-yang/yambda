#ifndef YAMBDA_ENV
#define YAMBDA_ENV

#include "types.h"
#include "keywords.h"

Env *env_init();

Element *env_get(Env *env, char *symbol);
Element *env_set(Env *env, char *symbol, Element *list);
Element *env_unset(Env *env, char *symbol);

Element *define(Env *env, char *symbol, Element *list);
Element *reference(Env *env, char *symbol);

#endif
