#ifndef YAMBDA_UTILS
#define YAMBDA_UTILS

#include "types.h"

Element *make_integer(int x);
Element *make_string(char *s);
Element *make_symbol(char *s);
Element *make_list_head();
Element *make_copy(Element *e);

#endif