#ifndef YAMBDA_UTILS
#define YAMBDA_UTILS

#include "types.h"

Element *make_none();
Element *make_error(char *fmt, ...);
Element *make_integer(int x);
Element *make_string(char *s);
Element *make_symbol(char *s);
Element *make_list_head();
Element *make_prim(int index);
Element *make_lambda(char *s);

Element *make_copy(Element *e);
void free_list(Element *e);
Element *tail_of(Element *head);

void push(Node **stack, Element *e);
Element *pop(Node **stack);

boolean streq(char *x, char *y);
boolean lsteq(Element *x, Element *y);

#define none make_none()

#endif
