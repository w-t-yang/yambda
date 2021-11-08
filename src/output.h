#ifndef YAMBDA_OUTPUT
#define YAMBDA_OUTPUT

#include "types.h"

void throw(char *fmt, ...);
void debug(char *fmt, ...);
void warn(char *fmt, ...);

void print_ele(Element *ele, boolean print_type, boolean without_quotes);
void print_env(Env *env);
void print_env_node(Env *env);
void print_list(Element *curr, boolean without_quotes);

void p(char *fmt, ...);
#define pele(e) print_ele(e, true, false)
void plst(Element *curr);
void plst_for_script(Element *curr);
#define penv print_env

#endif
