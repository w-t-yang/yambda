#include "output.h"
#include "utils.h"

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
  // TODO: Getting the context can be needed in 2 cases
  // One is at parsing time. When any vialation occurs, we can get the context
  // immediately from the file pointer.
  // Another one is at evaluation.
  // The 2nd is harder to provide the context, because the text has already
  // been parsed. Reading from input won't give and right context.
  // A potential solution is to store the context for each symbol,
  // but it will introduce extra space overhead
  /* char *ctx = get_error_context(); */
  /* char *ctx = ""; */
  /* Element *ele = alloc(T_ERROR, EXTRA_SPACE + strlen(ctx)); */
  /* sprintf(ele->str_v, "%s Error: %s", ctx, buffer); */
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
  if (ele->str_v[0] == '$') {
    ele->int_v = V_ARG_SYMBOL;
  } else {
    ele->int_v = V_NORMAL_SYMBOL;
  }
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
  return ele;
}

Element *make_func(int index) {
  char *buffer = KEYWORDS[index];
  Element *ele = alloc(T_FUNC, EXTRA_SPACE);
  ele->int_v = index;
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_lambda(char *buffer) {
  Element *ele = alloc(T_LAMBDA, EXTRA_SPACE);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_return_node() {
  Element *ele = alloc(T_RETURN, 0);
  return ele;
}

Element *_copy_node(Element *e) {
  int has_str = (strlen(e->str_v) > 0);
  int bytes_for_str = has_str ? strlen(e->str_v) + 1 - 8 : 0;

  Element *ele = alloc(e->type, bytes_for_str);
  ele->int_v = e->int_v;
  if (has_str) { strcpy(ele->str_v, e->str_v); }
  ele->next = e->next;
  ele->sub = e->sub;
  return ele;
}

// TODO: all usage of make_copy needs to be examined and tested
Element *make_copy(Element *e) {
  return _copy_node(e);
}

Element *make_deep_copy(Element *e) {
  Element *head = NULL;
  Element *prev = NULL;
  while (e) {
    Element *c = make_copy(e);
    if (!head) {
      head = c;
      prev = c;
    } else {
      prev->next = c;
      prev = c;
    }

    if (e->sub) {
      c->sub = make_deep_copy(e->sub);
    }

    e = e->next;
  }
  return head;
}

void free_list(Element *e) {
  // TODO: properly free a list
  free(e);
}

Element *tail_of(Element *head) {
  if (!head) { return NULL; }
  Element *curr = head;
  while (curr->next) {
    if (curr->type == T_RETURN) {
      break;
    }
    curr = curr->next;
  }

  if (curr->type == T_RETURN) {
    curr->next = NULL;
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

boolean streq(char *x, char *y) {
  return !strcmp(x, y);
}

Element *_extract_list(Element *x) {
  while (x && x->next == NULL && x->type == T_LISTHEAD) {
    x = x->sub;
  }
  if (x && x->type == T_RETURN) {
    x = x->sub;
  }
  return x;
}

boolean lsteq(Element *x, Element *y) {
  x = _extract_list(x);
  y = _extract_list(y);
  while (x && y) {
    if (x->type != y->type) {
      return false;
    } else {
      if (x->type == T_NONE || x->type == T_ERROR) {
        return true;
      } else if (x->type == T_INTEGER || x->type == T_FUNC) {
        if (x->int_v != y->int_v) { return false; }
      } else if (x->type == T_SYMBOL || x->type == T_STRING || x->type == T_LAMBDA) {
        if (!streq(x->str_v, y->str_v)) { return false; }
      } else if (x->type == T_LISTHEAD) {
        if (!lsteq(x->sub, y->sub)) { return false; }
      } else {
        return false;
      }
    }
    x = x->next;
    y = y->next;
  }

  if (!x && !y) {
    return true;
  } else {
    return false;
  }
}
