#include "elmt.h"

#include "../env.h"
#include "../funcs.h"
#include "../output.h"
#include "../utils.h"

Element *elmt_foreach(Env *env, Element *e) {
  if (!e || !e->next) {
    return make_error("Expect at least 2 element for FOREACH.");
  }

  Element *items = make_copy(e);
  items->next = NULL;
  items = eval(env, items);
  // ` will return a single T_LISTHEAD element without next
  // while other functions may return a list
  // TODO: check if this will be problematic
  if (items && items->type == T_LISTHEAD && !items->next) {
    items = items->sub;
  }
  Element *code = e->next;

  Element *res = NULL;
  int cnt = 0;
  while (items) {
    Element *i = make_copy(items);
    i->next = NULL;
    Element *arg = env_set(env, "$_", i);
    res = eval(env, code);
    cnt++;
    items = items->next;
  }

  return res;
}

Element *elmt_print(Element *e) {
  plst(e);
  printf("\n");
  return make_none();
}
