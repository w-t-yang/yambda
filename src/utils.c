#include "utils.h"
#include "output.h"

Element *alloc(int type, size_t size) {
  size += ELEMENT_MIN_SIZE;
  Element *ele = malloc(size);
  ele->type = type;
  ele->int_v = 0;
  ele->str_v[0] = '\0';
  ele->next = NULL;
  ele->sub = NULL;
  return ele;
}

static Element *_none_ = NULL;
Element *make_none() {
  if (!_none_) {
    Element *ele = alloc(T_NONE, 0);
    _none_ = ele;
  }
  _none_->next = NULL;
  _none_->sub = NULL;
  return _none_;
}

Element *_make_error(char *buffer) {
  Element *ele = alloc(T_ERROR, EXTRA_SPACE);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_error(char *fmt, ...) {
  int len;
  char *msg;
  va_list args;

  va_start(args, fmt);
  len = vsnprintf(0, 0, fmt, args);
  va_end(args);

  msg = malloc(len + 1);
  va_start(args, fmt);
  vsnprintf(msg, len + 1, fmt, args);
  va_end(args);

  return _make_error(msg);
}

Element *make_integer(int x) {
  Element *ele = alloc(T_INTEGER, 0);
  ele->int_v = x;
  return ele;
}

Element *make_string(char *buffer) {
  Element *ele = alloc(T_STRING, EXTRA_SPACE);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_symbol(char *buffer) {
  // TODO: validate symbol
  Element *ele = alloc(T_SYMBOL, EXTRA_SPACE);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_list_head() {
  Element *ele = alloc(T_LISTHEAD, 0);
  ele->int_v = V_LIST_UNEVALED;
  return ele;
}

Element *make_prim(int index) {
  char *buffer = KEYWORDS[index];
  Element *ele = alloc(T_PRIM, EXTRA_SPACE);
  ele->int_v = index;
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_lambda(char *buffer) {
  Element *ele = alloc(T_LAMBDA, EXTRA_SPACE);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_copy(Element *e) {
  // TODO: consider deep copy
  int has_str = (strlen(e->str_v) > 0);
  int bytes_for_str = has_str ? strlen(e->str_v) + 1 - 8 : 0;

  Element *ele = alloc(e->type, bytes_for_str);
  ele->int_v = e->int_v;
  if (has_str) { strcpy(ele->str_v, e->str_v); }
  ele->next = e->next;
  ele->sub = e->sub;
  return ele;
}

void free_list(Element *e) {
  // TODO: properly free a list
  free(e);
}

Element *tail_of(Element *head) {
  if (!head) { return make_error("Cannot get the last element of NULL list."); }
  Element *curr = head;
  while (curr->next) {
    curr = curr->next;
  }
  return curr;
}

void push(Node **stack, Element *e) {
  Node *n = malloc(NODE_SIZE);
  n->list = e;
  if (*stack) { n->next = *stack; }
  *stack = n;
}

Element *pop(Node **stack){
  if (!*stack) { return NULL; }
  Node *n = *stack;
  *stack = (*stack)->next;
  return n->list;
}
