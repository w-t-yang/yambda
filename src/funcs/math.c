#include "math.h"

#include "../env.h"
#include "../output.h"
#include "../utils.h"
#include "../funcs.h"

Element *math_sum(Element *x) {
  if (!x) {return make_error("Invalid parameters for operation SUM"); }

  int res = 0;
  if (x->type == T_INTEGER) {
    while (x) {
      if (x->type != T_INTEGER) {
        return make_error("Expecting numbers for operation SUM.");
      }
      res += x->int_v;
      x = x->next;
    }
  } else if (x->type == T_LISTHEAD && x->next == NULL) {
    return math_sum(x->sub);
  } else {
    return make_error("Expecting numbers or a single list for operation SUM.");
  }
  return make_integer(res);
}

Element *math_mul(Element *x) {
  if (!x) {return make_error("Invalid parameters for operation MULTIPLY"); }

  int res = 1;
  if (x->type == T_INTEGER) {
    while (x) {
      if (x->type != T_INTEGER) {
        return make_error("Expecting numbers for operation MULTIPLY.");
      }
      res *= x->int_v;
      x = x->next;
    }
  } else if (x->type == T_LISTHEAD && x->next == NULL) {
    return math_mul(x->sub);
  } else {
    return make_error(
        "Expecting numbers or a single list for operation MULTIPLY.");
  }
  return make_integer(res);
}

Element *math_div(Element *x) {
  if (!x) {return make_error("Invalid parameters for operation MULTIPLY"); }

  int res = 0;
  int *p = NULL;
  if (x->type == T_INTEGER) {
    while (x) {
      if (x->type != T_INTEGER) {
        return make_error("Expecting numbers for operation DIVIDE.");
      } else if (p != NULL && x->int_v == 0) {
        return make_error("Divisor cannot be 0.");
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
    return make_error(
        "Expecting numbers or a single list for operation DIVIDE.");
  }
  return make_integer(res);
}
