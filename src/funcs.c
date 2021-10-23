#include "env.h"
#include "funcs.h"
#include "output.h"
#include "utils.h"

#include "funcs/math.h"
#include "funcs/prim.h"

Element *apply(Env *env, Element *lambda, Element *args) {
  Element *res = NULL;

  //env_set(env, ARGS_SYMBOL, args);

  //res = progn(env, lambda);
  /* while (lambda) { */
  /*   if (lambda->type == T_EXPRESSION){ */
  /*     error("Not implemented yet."); */
  /*   } else { */
  /*     eval(env, lambda); */
  /*   } */
  /*   lambda = lambda->next; */
  /* } */

  //env_unset(env, ARGS_SYMBOL);

  return res;
}

Element *call_prim(Env *env, Element *head) {
  if (!head) { return make_error("Cannot call prim function with NULL list."); }
  if (head->type != T_PRIM) { return make_error("Invalid type for prim function."); }

  Element *list = head->next;

  switch (head->int_v) {
  case K_LET:
    return let(env, list);
  case K_DEF:
    return define(env, list);
  case K_REF:
    return reference(env, list);

  case K_SUM:
    return math_sum(list);
  case K_MUL:
    return math_mul(list);
  case K_DIV:
    return math_div(list);

  case K_QUOTE:
    return prim_quote(list);
  case K_ATOM:
    return prim_atom(list);
  case K_EQ:
    return prim_eq(list);
  case K_CAR:
    return prim_car(list);
  case K_CDR:
    return prim_cdr(list);
  case K_CONS:
    return prim_cons(list);
  case K_COND:
    return prim_cond(list);

  default:
    return make_error("Unknow prim type %d.", head->int_v);
  }
}

Element *last(Element *head) {
  if (!head) { return make_error("Cannot get the last element of NULL list."); }
  Element *curr = head;
  while (curr->next) {
    curr = curr->next;
  }
  return curr;
}

Element *pre_eval(Env *env, Element *head) {
  Element *curr = head;
  Element *prev = NULL;
  while (curr) {
    Element *l = NULL;

    if (curr->type == T_LISTHEAD) {
      l = eval(env, curr->sub);
    } else if (curr->type == T_SYMBOL) {
      l = reference(env, curr);
    }

    if (l) {
      if (l->next) {
        Element *h = make_list_head();
        h->sub = l;
        l = h;
      }

      if (prev) {
        prev->next = l;
        l->next = curr->next;
        curr = l;
      } else {
        l->next = curr->next;
        head = l;
        curr = l;

        if (head->type == T_PRIM &&
            (
             head->int_v == K_LET
             || head->int_v == K_DEF
             || head->int_v == K_REF
             || head->int_v == K_QUOTE
             ) ) {
          return head;
        }
      }
    }

    prev = curr;
    curr = curr->next;
  }
  return head;
}

Element *eval(Env *env, Element *head) {
  Element *res = NULL;
  if (!head) { return NULL; }

  head = pre_eval(env, head);

  switch (head->type) {
  case T_NONE:
    return none;
  case T_ERROR:
    return head;
  case T_INTEGER:
    return last(head);
  case T_STRING:
    return last(head);
  case T_SYMBOL:
    return make_error("Unknown symbol %d.", head->str_v);
  case T_LISTHEAD:
    return last(head);
  case T_PRIM:
    return call_prim(env, head);
  case T_LAMBDA:
    //TODO: call_lambda(env, head);
    return make_error("Evaluating of lambda is not implemented");
  default:
    return make_error("Unknown element type %d.", head->type);
  }
}
