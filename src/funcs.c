#include "env.h"
#include "funcs.h"
#include "output.h"
#include "utils.h"

#include "funcs/elmt.h"
#include "funcs/math.h"
#include "funcs/prim.h"

Element *call_func(Env *env, Element *head) {
  if (!head) { return make_error(ERR_FUNCTION_X_WITH_NULL_LIST, "eval"); }
  if (head->type != T_FUNC) { return make_error(ERR_FUNCTION_INVALID); }

  Element *list = head->next;

  switch (head->int_v) {
  case K_NONE:
    return make_none();
  case K_ERROR:
    return make_error(ERR_WITH_NO_MESSAGE);
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
    return prim_cond(env, list);

  case K_FOREACH:
    return elmt_foreach(env, list);
  case K_PRINT:
    return elmt_print(list);
  case K_RETURN:
    return elmt_return(list);

  default:
    return make_error(ERR_FUNCTION_INT_X_UNKNOWN, head->int_v);
  }
}

#define MAX_NUM_OF_ARGS 99
#define MAX_LOCAL_ARG_NAME_LENGTH 4
#define LOCAL_ARG_ZERO "$0"
#define LOCAL_ARG_PREFIX '$'
Element *call_lambda(Env *env, Element *head) {
  if (!head || head->type != T_LAMBDA) {
    return make_error(ERR_LAMBDA_INVALID);
  }

  Element *lambda = head->sub;
  Element *args = head->next;
  head->next = NULL;
  env_set(env, LOCAL_ARG_ZERO, head);
  int i = 1;
  while (args) {
    if (i > MAX_NUM_OF_ARGS - 1) {
      return make_error(ERR_LAMBDA_TOO_MANY_ARGS, MAX_NUM_OF_ARGS);
    }

    Element *next = args->next;
    args->next = NULL;

    char *arg_name = malloc(MAX_LOCAL_ARG_NAME_LENGTH);
    sprintf(arg_name, "%c%d", LOCAL_ARG_PREFIX, i);
    // TODO: Should use nested env
    env_set(env, arg_name, args);

    args = next;
    i++;
  }

  Element *res = eval(env, lambda);

  for (int j = 0; j < i; j++) {
    char *arg_name = malloc(MAX_LOCAL_ARG_NAME_LENGTH);
    sprintf(arg_name, "%c%d", LOCAL_ARG_PREFIX, j);
    env_unset(env, arg_name);
  }

  if (res && res->type == T_RETURN) {
    res = res->sub;
  }
  return res;
}

boolean _is_pointer_symbol(Element *x) {
  if (x && x->type == T_SYMBOL) {
    // Any symbol ends with ':' will be skipped, this is referred as pointer symbol
    // TODO: make ':' an invalid char if it's not at the end
    // TODO: when getting a pointer symbol, add a pointer in env, pointing to the next element
    // For now, just simply ignore this kind of symbols
    // Symbol ends with ',' will be skipped as well, but will not be used as pointer
    int len = strlen(x->str_v);
    if (len > 0 && (x->str_v[len-1] == ':' || x->str_v[len-1] == ',')) {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

Element *pre_eval(Env *env, Element *source) {
  // TODO: optimize the use of make_deep_copy when pre-evaluating a list
  Element *head = make_deep_copy(source);

  // Remove pointer symbols
  while (head && _is_pointer_symbol(head)) {
    head = head->next;
  }

  Element *curr = head->next;
  Element *prev = head;
  while (curr) {
    if (curr->type == T_SYMBOL && _is_pointer_symbol(curr)) {
      curr = curr->next;
      prev->next = curr;
    } else {
      prev = curr;
      curr = curr->next;
    }
  }

  curr = head;
  prev = NULL;
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

        // If the keyword is let/def/ref/cond/foreach, skip pre_eval the rest of the list
        if (head->type == T_FUNC &&
            (
             head->int_v == K_LET
             || head->int_v == K_DEF
             || head->int_v == K_REF
             || head->int_v == K_COND
             || head->int_v == K_FOREACH
             ) ) {

          // If the keyword is let, pre_eval the value that will be assigned to symbol
          if ((head->int_v == K_LET)
              && head->next
              && head->next->next) {
            Element *r = pre_eval(env, head->next->next);
            head->next->next = r;
          }

          return head;
        }
      }
    }

    if (curr && curr->type == T_ERROR) {
      free(curr->next);
      curr->next = NULL;
      return curr;
    }
    prev = curr;
    curr = curr->next;
  }
  return head;
}

Element *eval(Env *env, Element *head) {
  if (!head) { return NULL; }

  head = pre_eval(env, head);

  switch (head->type) {
  case T_NONE:
    return none;
  case T_ERROR:
    return head;
  case T_RETURN:
    return head;
  case T_INTEGER:
    return make_copy(tail_of(head));
  case T_STRING:
    return make_copy(tail_of(head));
  case T_SYMBOL:
    return make_error(ERR_SYMBOL_X_NOT_FOUND, head->str_v);
  case T_LISTHEAD:
    // TODO: 1. review the use of make_copy
    // TODO: 2. review if this could happen in other situations
    // TODO: 3. write tests
    // We need to make a copy of the tail of a list because,
    // pre_eval will try to use the tail to replace a list.
    // Then it can point its next pointer to another Element,
    // Thus the original list may be changed.
    //return tail_of(head);
    return make_copy(tail_of(head));
  case T_FUNC:
    return call_func(env, head);
  case T_LAMBDA:
    return call_lambda(env, head);
  default:
    return make_error(ERR_ELEMENT_INT_X_UNKNOWN, head->type);
  }
}
