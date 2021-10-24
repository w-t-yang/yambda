#include "prim.h"

#include "../env.h"
#include "../output.h"
#include "../utils.h"

Element *prim_quote(Element *x) {
  Element *h = make_list_head();
  h->sub = x;
  return h;
}

Element *prim_atom(Element *x) {
  if (!x) { return make_error("Cannot call ATOM on NULL list."); }
  if (x->type == T_ERROR
      || x->type == T_PRIM
      || x->type == T_LAMBDA
      || x->type == T_SYMBOL) {
    return make_error("Unsupported type %d for prim function ATOM.", x->type);
  }

  if (x->next) {
    return make_error("Expect 1 element for operation ATOM.");
  } else if (x->type == T_NONE
             || x->type == T_INTEGER
             || x->type == T_STRING) {
    return make_integer(1);
  } else if (x->type == T_LISTHEAD) {
    if (x->sub == NULL || (x->sub->type == T_NONE)) {
      return make_integer(1);
    } else {
      return none;
    }
  } else {
    return none;
  }
}

Element *prim_eq(Element *x) {
  if (!x) { return make_error("Cannot call EQ on NULL list."); }
  if (x->type == T_ERROR
      || x->type == T_PRIM
      || x->type == T_LAMBDA
      || x->type == T_SYMBOL) {
    return make_error("Unsupported type %d for prim function EQ.", x->type);
  }

  if (x && x->next && x->next->next) {
    return make_error("Expect 2 elements for operation EQ.");
  }

  if (x && x->next) {
    Element *a = x;
    Element *b = x->next;
    if (a->type != b->type) {
      return none;
    } else if (a->type == T_NONE) {
      return make_integer(1);
    } else if (a->type == T_INTEGER && a->int_v == b->int_v) {
      return make_integer(1);
    } else if (a->type == T_STRING && streq(a->str_v, b->str_v)) {
      return make_integer(1);
    } else if (x->type == T_LISTHEAD){
      return make_error("Comparison of lists is not implemented.");
    } else {
      return none;
    }
  }

  return make_error("Expect two elements for operation EQ.");
}

Element *prim_car(Element *x) {
  if (!x) { return make_error("Cannot call CAR on NULL list."); }
  if (x->type == T_NONE
      || x->type == T_ERROR
      || x->type == T_PRIM
      || x->type == T_LAMBDA
      || x->type == T_SYMBOL) {
    return make_error("Unsupported type %d for prim function CAR.", x->type);
  } else if (x->type == T_LISTHEAD && x->next == NULL) {
    return prim_car(x->sub);
  }

  Element *e = make_copy(x);
  e->next = NULL;
  return e;
}

Element *prim_cdr(Element *x) {
  if (!x) { return make_error("Cannot call CDR on NULL list."); }
  if (x->type == T_NONE
      || x->type == T_ERROR
      || x->type == T_PRIM
      || x->type == T_LAMBDA
      || x->type == T_SYMBOL) {
    return make_error("Unsupported type %d for prim function CDR.", x->type);
  } else if (x->type == T_LISTHEAD && x->next == NULL) {
    return prim_cdr(x->sub);
  }
  return x->next ? x->next : none;
}

Element *prim_cons(Element *x) {
  if (!x) { return make_error("Cannot call CONS on NULL list."); }
  if (x->type == T_NONE
      || x->type == T_ERROR
      || x->type == T_PRIM
      || x->type == T_LAMBDA
      || x->type == T_SYMBOL) {
    return make_error("Unsupported type %d for prim function CONS.", x->type);
  }

  Element *y = x->next;
  if (!y || y->type != T_LISTHEAD){
    return make_error("Expect 2nd element to be a list for function CONS.");
  }
  if (y->next) {
    return make_error("Expect 2 elements for function CONS.");
  }

  x->next = y->sub;
  y->sub = NULL;
  free_list(y);

  return x;
}

Element *prim_cond(Element *x) {
  if (!x) { return make_error("Cannot call CONS on NULL list."); }
  int i = 0;
  while (x) {
    if (x->type == T_ERROR
        || x->type == T_PRIM
        || x->type == T_LAMBDA
        || x->type == T_SYMBOL) {
      return make_error("Expect the %dth element to be an atom for COND.", i);
    }

    if (!x->next) { return none; }

    boolean cond = false;
    if (x->type == T_INTEGER && x->int_v != 0) { cond = true; }
    else if (x->type == T_STRING && !streq(x->str_v, "")) { cond = true; }
    else if (x->type == T_LISTHEAD && x->sub && x->sub->type != T_NONE) { cond = true;}
    if (cond) {
      Element *r = make_copy(x->next);
      r->next = NULL;
      return r;
    }

    if (!x->next->next) { return none; }
    x = x->next->next;
    i += 2;
  }
  return make_error("Unexpected error for operation COND.");
}
