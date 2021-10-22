#include "math.h"

#include "../env.h"
#include "../output.h"
#include "../utils.h"

Element *math_plus(Env *env, Element *x) {
  int res = 0;
  while (x) {
    if (x->type == T_SYMBOL) {
      Element *v = reference(env, x->str_v);
      Element *r = math_plus(env, v);
      res += r->int_v;
    } else if (x->type == T_INTEGER) {
      res += x->int_v;
    } else {
      error("Expecting numbers for operation PLUS.");
    }

    x = x->next;
  }
  return make_integer(res);
}
