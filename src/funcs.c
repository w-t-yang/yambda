#include "env.h"
#include "funcs.h"
#include "output.h"
#include "utils.h"

#include "funcs/math.h"
#include "funcs/prim.h"

void init_funcs(Env *env) {
  // Core
  env_set(env, F_DEF, make_func_symbol(F_DEF));
  env_set(env, F_REF, make_func_symbol(F_REF));

  // Math
  env_set(env, F_SUM, make_func_symbol(F_SUM));

  // Primitives
  env_set(env, F_Q, make_func_symbol(F_Q));
  env_set(env, F_A, make_func_symbol(F_A));
  env_set(env, F_EQ, make_func_symbol(F_EQ));
  env_set(env, F_CAR, make_func_symbol(F_CAR));
  env_set(env, F_CDR, make_func_symbol(F_CDR));
  env_set(env, F_CONS, make_func_symbol(F_CONS));
  env_set(env, F_COND, make_func_symbol(F_COND));
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

Element *eval(Env *env, Element *head) {
  Element *res = NULL;
  if (!head) {
    return make_null_symbol();
  } else if (head->type == T_INTEGER || head->type == T_STRING
      || (head->type == T_LISTHEAD && head->int_v == V_LIST_EVALED)) {
    if (head->next) {
      return eval(env, head->next);
    } else {
      return head;
    }
  } else if (head->type == T_LISTHEAD) {
    Element *next = head->next;
    Element *curr = eval(env, head->args);
    if (curr->type == T_LISTHEAD) {
      curr->int_v = V_LIST_EVALED;
    }
    curr->next = next;
    return eval(env, curr);
  } else if (head->type == T_SYMBOL) {
    Element *list = head->next;
    Element *args = head->args;
    // TODO: for all primitive functions, handle expressions in args

    // Prim
    if (streq(head->str_v, F_Q)) {
      res = prim_quote(list);
    } else if (streq(head->str_v, F_A)) {
      res = prim_atom(list);
    } else if (streq(head->str_v, F_EQ)) {
      res = prim_eq(list);
    } else if (streq(head->str_v, F_CAR)) {
      res = prim_car(list);
    } else if (streq(head->str_v, F_CDR)) {
      res = prim_cdr(list);
    } else if (streq(head->str_v, F_CONS)) {
      res = prim_cons(list);
    } else if (streq(head->str_v, F_COND)) {
      res = prim_cond(list);
    }
    // Math
    else if (streq(head->str_v, F_SUM)) {
      res = math_sum(env, list);
    } else if (streq(head->str_v, F_MUL)) {
      res = math_mul(env, list);
    } else if (streq(head->str_v, F_DIV)) {
      res = math_div(env, list);
    }
    // Core
    else if (streq(head->str_v, F_DEF)) {
      res = define(env, list->str_v, list->next);
    } else if (streq(head->str_v, F_REF)) {
      res = reference(env, list->str_v);
    }
    // Else
    else {
      Element *ref = reference(env, head->str_v);
      //if (!lambda) {
      //  error("Can't find symbol '%s'.", head->str_v);
      //}
      //res = apply(env, lambda, list);
      Element *new_head = make_copy(ref);
      new_head->next = list;
      new_head->args = args;
      res = eval(env, new_head);
    }

    return res;
  } else {
    error("Not implemented to evaluate element of type '%d'", head->type);
    return NULL;
  }
}
