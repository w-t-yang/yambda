#include "env.h"
#include "output.h"
#include "primitives.h"
#include "utils.h"

Element *reference(Env *env, char *symbol) {
  Element *e = env_get(env, symbol);
  if (e->type > T_VALUE) {
    return e;
  } else if (e->type == T_SYMBOL) {
    return reference(env, e->str_v);
  } else if (e->type == T_EXPRESSION) {
    error("Not implement for referencing expression.");
  }
  error("Unknown reference type");
  return NULL;
}

Element *prim_quote(Element *x) {
  return x;
}

Element *prim_atom(Element *x) {
  // Is single symbol considered as atom?
  if (!x) {
    return make_integer(1);
  } else if (x->type > T_VALUE && !(x->next)) {
    return make_integer(1);
  } else if (x->type == T_SYMBOL) {
    error("Haven't decided whether a symbol is atom or not.");
    return make_integer(0);
  } else {
    return make_integer(0);
  }
}

Element *prim_eq(Element *x) {
  // TODO: handle >=3 elements in list x
  if (x && x->next) {
    if (x->type == TV_INT && x->int_v == x->next->int_v) {
      return make_integer(1);
    } else if (x->type == TV_STRING && streq(x->str_v, x->next->str_v)) {
      return make_integer(1);
    } else if (x->type == T_SYMBOL) {
      error("Not implemented yet for symbol comparison.");
      return make_integer(0);
    } else {
      return make_integer(0);
    }
  }
  error("Expecting two elements for operation EQ.");
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
  error("Not implements.");
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
  error("Not implemented.");
  return x;
}

Element *prim_plus(Env *env, Element *x) {
  int res = 0;
  while (x) {
    if (x->type == T_SYMBOL) {
      Element *v = reference(env, x->str_v);
      Element *r = prim_plus(env, v);
      res += r->int_v;
    } else if (x->type == TV_INT) {
      res += x->int_v;
    } else {
      error("Expecting numbers for operation PLUS.");
    }

    x = x->next;
  }
  return make_integer(res);
}

Element *prim_assign(Env *env, Element *x) {
  if (x && x->type == T_SYMBOL) {
    env_set(env, x->str_v, x->next);
    return x->next;
  }
  error("Invalid symbol.");
  return NULL;
}

void init_primitives(Env *env) {
  env_set(env, "`", NULL);
  env_set(env, "a?", NULL);
  env_set(env, "=?", NULL);
  env_set(env, "<|", NULL);
  env_set(env, "|>", NULL);
  env_set(env, ".", NULL);
  env_set(env, "?", NULL);

  env_set(env, "+", NULL);
  env_set(env, "=", NULL);
}



Element *apply(Env *env, Element *lambda, Element *args) {
  Element *res = NULL;

  env_set(env, ARGS_SYMBOL, args);

  //res = progn(env, lambda);
  /* while (lambda) { */
  /*   if (lambda->type == T_EXPRESSION){ */
  /*     error("Not implemented yet."); */
  /*   } else { */
  /*     eval(env, lambda); */
  /*   } */
  /*   lambda = lambda->next; */
  /* } */

  env_unset(env, ARGS_SYMBOL);

  return res;
}

Element *eval(Env *env, Element *list) {
  Element *res = NULL;

  if (list->type != T_SYMBOL) {
    error("Can't evaluate element of type '%d'", list->type);
  }

  Element *l = list;
  Element *args = l->next;
  // TODO: for all primitive functions, handle expressions in args
  if (streq(l->str_v, "`")) {
    res = prim_quote(args);
  } else if (streq(l->str_v, "a?")) {
    res = prim_atom(args);
  } else if (streq(l->str_v, "=?")) {
    res = prim_eq(args);
  } else if (streq(l->str_v, "<|")) {
    res = prim_car(args);
  } else if (streq(l->str_v, "|>")) {
    res = prim_cdr(args);
  } else if (streq(l->str_v, ".")) {
    res = prim_cons(args);
  } else if (streq(l->str_v, "?")) {
    res = prim_cond(args);
  } else if (streq(l->str_v, "+")) {
    res = prim_plus(env, args);
  } else if (streq(l->str_v, "=")) {
    res = prim_assign(env, args);
  } else {
    Element *lambda = env_get(env, list->str_v);
    if (!lambda) {
      error("Can't find symbol '%s'.", list->str_v);
    }
    res = apply(env, lambda, args);
  }

  return res;
}
