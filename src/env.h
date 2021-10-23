#ifndef YAMBDA_ENV
#define YAMBDA_ENV

#include "types.h"

Env *env_init();

Element *let(Env *env, Element *list);
Element *define(Env *env, Element *list);
Element *reference(Env *env, Element *list);

#endif
