#include "prim.h"

#include "../env.h"
#include "../output.h"
#include "../utils.h"

Element *prim_quote(Element *x) {
  return x;
}

Element *prim_atom(Element *x) {
  // Is single symbol considered as atom?
  if (!x) {
    // NULL is atom
    return make_integer(1);
  } else if (x->type > T_INTEGER && !(x->next)) {
    return make_integer(1);
  } else if (x->type > T_STRING && !(x->next)) {
    return make_integer(1);
  } else if (x->type == T_SYMBOL) {
    error("Haven't decided whether a symbol is atom or not.");
    return make_integer(0);
  } else {
    return make_integer(0);
  }
}

Element *prim_eq(Element *x) {
  // TODO: handle more than 3 elements in list x
  if (x && x->next && x->next->next) {
    error("More than 2 elements are provided for operation EQ, extra elements are ignored.");
  }

  if (x && x->next) {
    if (x->type != x->next->type) {
      return make_integer(0);
    } else if (x->type == T_INTEGER && x->int_v == x->next->int_v) {
      return make_integer(1);
    } else if (x->type == T_STRING && streq(x->str_v, x->next->str_v)) {
      return make_integer(1);
    } else if (x->type == T_SYMBOL && streq(x->str_v, x->next->str_v)) {
      return make_integer(1);
    } else if (x->type == T_LISTHEAD){
      error("Comparison of lists is not implemented.");
      return make_integer(0);
    } else {
      error("Unknown element type.");
      return make_integer(0);
    }
  }

  error("Expect two elements for operation EQ.");
  return 0;
}

Element *prim_car(Element *x) {
  if (x) {
    Element *e = make_copy(x);
    e->next = NULL;
    return e;
  }
  error("Expecting at least 1 element for operation CAR.");
  return NULL;
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
