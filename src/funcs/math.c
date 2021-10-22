#include "math.h"

#include "../env.h"
#include "../output.h"
#include "../utils.h"
#include "../funcs.h"

Element *math_sum(Env *env, Element *x) {
  int res = 0;
  while (x) {
    if (x->type == T_SYMBOL) {
      Element *v = reference(env, x->str_v);
      Element *r = math_sum(env, v);
      res += r->int_v;
    } else if (x->type == T_INTEGER) {
      res += x->int_v;
    } else if (x->type == T_LISTHEAD) {
      Element *v = eval(env, x->args);
      Element *r = math_sum(env, v);
      res += r->int_v;
    } else {
      error("Expecting numbers for operation PLUS.");
    }

    x = x->next;
  }
  return make_integer(res);
}

Element *math_mul(Env *env, Element *x) {
  int res = 1;
  while (x) {
    if (x->type == T_SYMBOL) {
      Element *v = reference(env, x->str_v);
      Element *r = math_mul(env, v);
      res *= r->int_v;
    } else if (x->type == T_INTEGER) {
      res *= x->int_v;
    } else if (x->type == T_LISTHEAD) {
      Element *v = eval(env, x->args);
      Element *r = math_mul(env, v);
      res *= r->int_v;
    } else {
      error("Expecting numbers for operation MULTIPLY.");
    }

    x = x->next;
  }
  return make_integer(res);
}

Element *math_div(Env *env, Element *x) {
  int res = 0;
  int *p = NULL;
  while (x) {
    if (x->type == T_SYMBOL) {
      Element *v = reference(env, x->str_v);
      Element *r = math_div(env, v);
      if (p == NULL) {
        res = r->int_v;
        p = &res;
      } else {
        res /= r->int_v;
      }
    } else if (x->type == T_INTEGER) {
      if (x->int_v == 0) {
        error("Divisor cannot be 0.");
        return NULL;
      }
      if (p == NULL) {
        res = x->int_v;
        p = &res;
      } else {
        res /= x->int_v;
      }
    } else if (x->type == T_LISTHEAD) {
      Element *v = eval(env, x->args);
      Element *r = math_div(env, v);
      if (p == NULL) {
        res = r->int_v;
        p = &res;
      } else {
        res /= r->int_v;
      }
    } else {
      error("Expecting numbers for operation DIVIDE.");
    }

    x = x->next;
  }
  return make_integer(res);
}
