#include "math.h"

#include "../env.h"
#include "../output.h"
#include "../utils.h"
#include "../funcs.h"

Element *math_sum(Element *x) {
  if (!x) {return make_error(ERR_FUNCTION_X_WITH_NULL_LIST, "sum"); }

  int res = 0;
  if (x->type == T_INTEGER) {
    while (x) {
      if (x->type != T_INTEGER) {
        return make_error(ERR_FUNCTION_X_EXPECTS_Y, "sum", "numbers only");
      }
      res += x->int_v;
      x = x->next;
    }
  } else if (x->type == T_LISTHEAD && x->next == NULL) {
    return math_sum(x->sub);
  } else {
    return make_error(ERR_FUNCTION_X_EXPECTS_Y, "sum", "numbers only");
  }
  return make_integer(res);
}

Element *math_mul(Element *x) {
  if (!x) {return make_error(ERR_FUNCTION_X_WITH_NULL_LIST, "multiply"); }

  int res = 1;
  if (x->type == T_INTEGER) {
    while (x) {
      if (x->type != T_INTEGER) {
        return make_error(ERR_FUNCTION_X_EXPECTS_Y, "multiply", "numbers only");
      }
      res *= x->int_v;
      x = x->next;
    }
  } else if (x->type == T_LISTHEAD && x->next == NULL) {
    return math_mul(x->sub);
  } else {
    return make_error(ERR_FUNCTION_X_EXPECTS_Y, "multiply", "numbers only");
  }
  return make_integer(res);
}

Element *math_div(Element *x) {
  if (!x) {return make_error(ERR_FUNCTION_X_WITH_NULL_LIST, "divide"); }

  int res = 0;
  int *p = NULL;
  if (x->type == T_INTEGER) {
    while (x) {
      if (x->type != T_INTEGER) {
        return make_error(ERR_FUNCTION_X_EXPECTS_Y, "divide", "numbers only");
      } else if (p != NULL && x->int_v == 0) {
        return make_error(ERR_FUNCTION_DIV_WITH_DIVISOR_ZERO);
      }

      if (p == NULL) {
        res = x->int_v;
        p = &res;
      } else {
        res /= x->int_v;
      }

      x = x->next;
    }
  } else if (x->type == T_LISTHEAD && x->next == NULL) {
    return math_div(x->sub);
  } else {
    return make_error(ERR_FUNCTION_X_EXPECTS_Y, "divide", "numbers only");
  }
  return make_integer(res);
}
