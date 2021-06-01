#include "utils.h"

#define ELEMENT_MIN_SIZE 32

Element *alloc(int type, size_t size) {
  // This might allocate a little bit more memory than needed
  // Because the last 8 bytes of Element struct were only used by str_v[1]
  // Possible to save 6-7 bytes by some optimization
  size += ELEMENT_MIN_SIZE;
  Element *ele = malloc(size);
  ele->type = type;
  return ele;
}

Element *make_integer(int x) {
  Element *ele = alloc(TV_INT, 0);
  ele->int_v = x;
  return ele;
}

Element *make_string(char *buffer) {
  Element *ele = alloc(TV_STRING, strlen(buffer) + 1);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_expression(char *buffer) {
  // TODO: make real expression
  Element *ele = alloc(T_EXPRESSION, strlen(buffer) + 1);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_symbol(char *buffer) {
  // TODO: validate symbol
  Element *ele = alloc(T_SYMBOL, strlen(buffer) + 1);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_copy(Element *e) {
  Element *ele = alloc(e->type, strlen(e->str_v) + 1);
  ele->int_v = e->int_v;
  strcpy(ele->str_v, e->str_v);
  ele->next = e->next;
  return ele;
}
