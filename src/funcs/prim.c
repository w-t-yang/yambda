#include "prim.h"

#include "../env.h"
#include "../funcs.h"
#include "../output.h"
#include "../utils.h"

Element *prim_quote(Element *x) {
  Element *h = make_list_head();
  h->sub = x;
  return h;
}

Element *prim_atom(Element *x) {
  if (!x) { return make_error(ERR_FUNCTION_X_WITH_NULL_LIST, "atom"); }
  /* if (x->type == T_ERROR */
  /*     || x->type == T_FUNC */
  /*     || x->type == T_LAMBDA */
  /*     || x->type == T_SYMBOL) { */
  /*   return make_error(ERR_FUNCTION_X_WITH_INVALID_TYPE_Y, "atom", TYPES[x->type]); */
  /* } */

  if (x->next) {
    return make_error(ERR_FUNCTION_X_EXPECTS_Y, "atom", "1 element");
  } else if (x->type == T_NONE
             || x->type == T_INTEGER
             || x->type == T_STRING) {
    return make_integer(1);
  } else if (x->type == T_LISTHEAD) {
    if (x->sub == NULL || (x->sub->type == T_NONE)) {
      return make_integer(1);
    } else {
      return make_integer(0);
    }
  } else {
    return make_integer(0);
  }
}

Element *prim_eq(Element *x) {
  if (!x) { return make_error(ERR_FUNCTION_X_WITH_NULL_LIST, "eq"); }
  /* if (x->type == T_ERROR */
  /*     || x->type == T_FUNC */
  /*     || x->type == T_LAMBDA */
  /*     || x->type == T_SYMBOL) { */
  /*   return make_error(ERR_FUNCTION_X_WITH_INVALID_TYPE_Y, "eq", TYPES[x->type]); */
  /* } */

  if (x && x->next && x->next->next) {
    return make_error(ERR_FUNCTION_X_EXPECTS_Y, "eq", "2 elements");
  }

  if (x && x->next) {
    boolean res = false;
    Element *a = x;
    Element *b = x->next;
    x->next = NULL;

    if (a->type != b->type) {
      res = false;
    } else if (a->type == T_NONE) {
      res = true;
    } else if (a->type == T_INTEGER && a->int_v == b->int_v) {
      res = 1;
    } else if (a->type == T_STRING && streq(a->str_v, b->str_v)) {
      res = 1;
    } else if (x->type == T_LISTHEAD && lsteq(a, b)){
      res = 1;
    } else {
      res = 0;
    }
    x->next = b;
    if (res) {return make_integer(1);}
    else {return make_integer(0);}
  }

  return make_error(ERR_FUNCTION_X_EXPECTS_Y, "eq", "2 elements");
}

Element *prim_car(Element *x) {
  if (!x) { return make_error(ERR_FUNCTION_X_WITH_NULL_LIST, "car"); }
  /* if (x->type == T_NONE */
  /*     || x->type == T_ERROR */
  /*     || x->type == T_FUNC */
  /*     || x->type == T_LAMBDA */
  /*     || x->type == T_SYMBOL) { */
  /*   return make_error(ERR_FUNCTION_X_WITH_INVALID_TYPE_Y, "car", TYPES[x->type]); */
  /* } else */ if (x->type == T_LISTHEAD && x->next == NULL) {
    return prim_car(x->sub);
  }
  Element *e = make_copy(x);
  e->next = NULL;
  return e;
}

Element *prim_cdr(Element *x) {
  if (!x) { return make_error(ERR_FUNCTION_X_WITH_NULL_LIST, "cdr"); }
  /* if (x->type == T_NONE */
  /*     || x->type == T_ERROR */
  /*     || x->type == T_FUNC */
  /*     || x->type == T_LAMBDA */
  /*     || x->type == T_SYMBOL) { */
  /*   return make_error(ERR_FUNCTION_X_WITH_INVALID_TYPE_Y, "cdr", TYPES[x->type]); */
  /* } else */ if (x->type == T_LISTHEAD && x->next == NULL) {
    return prim_cdr(x->sub);
  }
  return x->next ? x->next : none;
}

Element *prim_cons(Element *x) {
  if (!x) { return make_error(ERR_FUNCTION_X_WITH_NULL_LIST, "cons"); }
  /* if (x->type == T_NONE */
  /*     || x->type == T_ERROR */
  /*     || x->type == T_FUNC */
  /*     || x->type == T_LAMBDA */
  /*     || x->type == T_SYMBOL) { */
  /*   return make_error(ERR_FUNCTION_X_WITH_INVALID_TYPE_Y, "cons", TYPES[x->type]); */
  /* } */

  Element *y = x->next;
  if (!y || y->type != T_LISTHEAD){
    return make_error(ERR_FUNCTION_X_EXPECTS_Y, "cons", "the 2nd elements to be a list");
  }
  if (y->next) {
    return make_error(ERR_FUNCTION_X_EXPECTS_Y, "cons", "2 elements");
  }

  x->next = y->sub;
  y->sub = NULL;
  free_list(y);

  Element *h = make_list_head();
  h->sub = x;

  return h;
}

Element *prim_cond(Env *env, Element *x) {
  if (!x) { return make_error(ERR_FUNCTION_X_WITH_NULL_LIST, "cond"); }
  int i = 0;
  while (x) {
    if (!x->next) { return eval(env, x); }

    /* if (x->type == T_ERROR */
    /*     || x->type == T_FUNC */
    /*     || x->type == T_LAMBDA */
    /*     || x->type == T_SYMBOL) { */

    /*   return make_error(ERR_FUNCTION_COND_EXPECTS_ATOM_AT_X, i); */
    /* } */

    Element *next = x->next->next;
    Element *y = x->next;
    x->next = NULL;
    y->next = NULL;
    x = eval(env, x);

    boolean cond = false;
    if (x->type == T_INTEGER && x->int_v != 0) { cond = true; }
    else if (x->type == T_STRING && !streq(x->str_v, "")) { cond = true; }
    else if (x->type == T_LISTHEAD && x->sub && x->sub->type != T_NONE) { cond = true;}

    if (cond) { return eval(env, y); }

    if (!next) { return none; }

    x = next;
    i += 2;
  }
  return make_error(ERR_UNEXPECTED_FOR_X, "cond");
}
