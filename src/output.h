#ifndef YAMBDA_OUTPUT
#define YAMBDA_OUTPUT

#include "types.h"

void throw(char *fmt, ...);
void debug(char *fmt, ...);
void warn(char *fmt, ...);

void print_ele(Element *ele);
void print_list(Element *curr);
void print_env(Env *env);
void print_env_node(Env *env);

void p(char *fmt, ...);
#define pele print_ele
#define plst print_list
#define penv print_env

#endif
