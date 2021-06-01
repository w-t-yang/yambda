#ifndef YAMBDA_OUTPUT
#define YAMBDA_OUTPUT

#include "types.h"

void error(char *fmt, ...);
void debug(char *fmt, ...);
void print_ele(Element *ele);
void print_list(Element *curr);
void print_env(Env *env);

#endif
