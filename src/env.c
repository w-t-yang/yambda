#include "env.h"
#include "output.h"
#include "utils.h"

boolean _is_keyword(char *k) {
  for (int i = 0; i < KEYWORDS_COUNT; i++) {
    if (streq(k, KEYWORDS[i])) {
      return true;
    }
  }
  return false;
}

void _validate_symbol(char *k) {
  if (_is_keyword(k)) {
    throw("Cannot use \"%s\" as environment symbol.", k);
  }
}

// TODO: 1. In order to improve performance, re-implment env by hashmap or trie
// TODO: 2. Use nested env, instead of global env

Env *_make_env_node(char *buffer) {
  Env *env = malloc(24 + EXTRA_SPACE);
  env->list = NULL;
  env->next = NULL;
  strcpy(env->symbol, buffer);
  return env;
}

Element *_env_get(Env *env, char *symbol) {
  Env *curr = env;
  while (curr) {
    if (streq(curr->symbol, symbol)) {
      return curr->list;
    }
    curr = curr->next;
  }
  return make_error("Symbol \"%s\" not found.", symbol);
}

Element *_env_unset(Env *env, char *symbol) {
  Env *curr = env;
  while (curr) {
    if (streq(curr->symbol, symbol)) {
      free_list(curr->list);
      curr->list = NULL;
      return none;
    }
    curr = curr->next;
  }
  return make_error("Symbol \"%s\" not found.", symbol);
}

Element *_env_set(Env *env, char *symbol, Element *list) {


  if (strlen(symbol) == 0) {
    return make_error("Invalid symbol name \"%s\".", symbol);
  }
  if (!env) {
    return make_error("Environment is not initialized.");
  }

  Env *curr = env;
  while (curr) {
    if (streq(curr->symbol, symbol)) {
      free_list(curr->list);
      curr->list = list;
      return list;
    }

    if (curr->next) {
      curr = curr->next;
    } else {
      Env *e = _make_env_node(symbol);
      e->list = list;
      curr->next = e;
      return e->list;
    }
  }
  return make_error("This error is not expected.");
}

Env *env_init() {
  Env *env = malloc(24 + 0);
  env->list = NULL;
  env->next = NULL;
  strcpy(env->symbol, "");

  for (int i = 0; i < KEYWORDS_COUNT; i++) {
    _env_set(env, KEYWORDS[i], make_prim(i));
  }
  return env;
}

Element *let(Env *env, Element *x) {
  if (x == NULL
      || x->next == NULL
      || x->type != T_SYMBOL) {
    return make_error("Invalid params for operation LET");
  }
  _validate_symbol(x->str_v);

  // TODO: check whether x can be changed
  _env_set(env, x->str_v, x->next);
  return x->next;
}

Element *define(Env *env, Element *x) {
  if (x == NULL
      || x->next == NULL
      || x->type != T_SYMBOL) {
    return make_error("Invalid params for operation DEF");
  }

  _validate_symbol(x->str_v);

  // TODO: check whether x can be changed
  return make_error("`DEF` not implemented.");
  //return x->next;
}

Element *reference(Env *env, Element *x) {
  if (x == NULL || x->type != T_SYMBOL) {
    return make_error("Invalid params for operation REF");
  }

  // TODO: check whether e can be changed
  Element *e = _env_get(env, x->str_v);
  if (e && e->type == T_SYMBOL) {
    return reference(env, e);
  } else {
    return make_copy(e);
  }
}
