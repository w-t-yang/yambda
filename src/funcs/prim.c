#include "prim.h"

#include "../env.h"
#include "../output.h"
#include "../utils.h"

Element *prim_quote(Element *x) {
  Element *h = make_list_head();
  h->args = x;
  return h;
}

Element *prim_atom(Element *x) {
  // Is single symbol considered as atom?
  if (!x) {
    // NULL is atom
    return make_integer(1);
  } else if (x->next) {
    error("More than 1 element provided for operation ATOM?");
    return NULL;
  } else if (x->type == T_INTEGER) {
    return make_integer(1);
  } else if (x->type == T_STRING) {
    return make_integer(1);
  } else if (x->type == T_LISTHEAD) {
    if (x->args == NULL
        || (x->args->type == V_SYMBOL_NULL && x->args->next == NULL)) {
      return make_integer(1);
    } else {
      return make_integer(0);
    }
  } else if (x->type == T_SYMBOL) {
    if (x->int_v == V_SYMBOL_NULL) {
      return make_integer(1);
    }
    error("Haven't decided whether a symbol is atom or not.");
    return make_integer(0);
  } else {
    return make_integer(0);
  }
}

Element *prim_eq(Env *env, Element *x) {
  // TODO: handle more than 3 elements in list x
  if (x && x->next && x->next->next) {
    error("More than 2 elements are provided for operation EQ, extra elements are ignored.");
  }

  if (x && x->next) {
    Element *a = x->type == T_SYMBOL ? reference(env, x->str_v) : x;
    Element *b = x->next->type == T_SYMBOL ? reference(env, x->next->str_v) : x->next;
    if (a->type != b->type) {
      return make_integer(0);
    } else if (a->type == T_INTEGER && a->int_v == b->int_v) {
      return make_integer(1);
    } else if (a->type == T_STRING && streq(a->str_v, b->str_v)) {
      return make_integer(1);
    } else if (a->type == T_SYMBOL && streq(a->str_v, b->str_v)) {
      return make_integer(1);
    } else if (x->type == T_LISTHEAD){
      error("Comparison of lists is not implemented.");
      return make_integer(0);
    } else {
      return make_integer(0);
    }
  }

  error("Expect two elements for operation EQ.");
  return 0;
}

Element *prim_car(Element *x) {
  Element *e;
  if (x && x->type == T_LISTHEAD && x->next == NULL) {
    e = make_copy(x->args);
  } else  if (x) {
    e = make_copy(x);
  } else {
    error("Expecting at least 1 element for operation CAR.");
    return NULL;
  }
  e->next = NULL;
  return e;
}

Element *prim_cdr(Element *x) {
  if (x) {
    return x->next;
  }
  error("Expecting at least 1 element for operation CDR.");
  return NULL;
}

Element *prim_cons(Element *x) {
  error("Operation CONS not implemented.");
  // TODO: handle >=3 elements in list x
  if (!x) {
    return x;
  } else {
    Element *y = x->next;
    // Y has to be an expression(list)

    Element *curr = x;
    while (curr->next) {
      curr = curr->next;
    }
    curr->next = y;
    return x;
  }
}

Element *prim_cond(Element *x) {
  error("Operation COND not implemented.");
  return x;
}
