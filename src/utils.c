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
  Element *ele = alloc(T_INTEGER, 0);
  ele->int_v = x;
  return ele;
}

Element *make_string(char *buffer) {
  // When allocating space for a string,
  // We need strlen(buffer) bytes, and 1 byte for '\0'
  // We have already allocated 8 bytes when defining `char str_v[1];`
  int needed = strlen(buffer) + 1;
  Element *ele = alloc(T_STRING, needed > 8 ? needed - 8: 0);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_symbol(char *buffer) {
  // TODO: validate symbol
  int needed = strlen(buffer) + 1;
  Element *ele = alloc(T_SYMBOL, needed > 8 ? needed - 8: 0);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_list_head() {
  Element *ele = alloc(T_LISTHEAD, 0);
  return ele;
}

Element *make_copy(Element *e) {
  int has_str = (strlen(e->str_v) > 0);
  int bytes_for_str = has_str ? strlen(e->str_v) + 1 - 8 : 0;

  Element *ele = alloc(e->type, bytes_for_str);
  ele->int_v = e->int_v;
  if (has_str) { strcpy(ele->str_v, e->str_v); }
  ele->next = e->next;
  return ele;
}
