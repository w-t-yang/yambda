#include "env.h"
#include "output.h"
#include "utils.h"

boolean _is_keyword(char *k) {
  for (int i = 0; i < KEYWORDS_COUNT; i++) {
    if (streq(k, KEYWORDS[i])) {
      return true;
    } else if(k[0] == '$') {
      return true;
    }
  }
  return false;
}

Element *_validate_symbol(Element *symbol) {
  if (_is_keyword(symbol->str_v)) {
    return make_error(ERR_SYMBOL_X_IS_RESERVED, symbol->str_v);
  }
  return NULL;
}

// TODO: 1. In order to improve performance, re-implment env by hashmap or trie
// TODO: 2. Use nested env, instead of global env

Env *_make_env_node(char *buffer) {
  Env *env = malloc(ENV_MIN_SIZE + EXTRA_SPACE);
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
  return make_error(ERR_SYMBOL_X_NOT_FOUND, symbol);
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
  return make_error(ERR_SYMBOL_X_NOT_FOUND, symbol);
}

Element *env_set(Env *env, char *symbol, Element *list) {
  // TODO: Whenever setting an env node, we need to make a deep copy of the list
  if (strlen(symbol) == 0) {
    return make_error(ERR_SYMBOL_X_INVALID, symbol);
  }
  if (!env) {
    return make_error(ERR_ENV_NOT_INITED);
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
  return make_error(ERR_UNEXPECTED_FOR_X, "env_set");
}

Env *env_init() {
  Env *env = malloc(24 + 0);
  env->list = NULL;
  env->next = NULL;
  strcpy(env->symbol, "");

  for (int i = 0; i < KEYWORDS_COUNT; i++) {
    env_set(env, KEYWORDS[i], make_func(i));
  }
  return env;
}

Element *let(Env *env, Element *x) {
  if (x == NULL
      || x->next == NULL
      || x->type != T_SYMBOL
      || x->next->next) {
    return make_error(ERR_FUNCTION_X_EXPECTS_Y, "let", "a symbol and a list");
  }
  Element *error = _validate_symbol(x);
  if (error) { return error; }

  // TODO: check whether x can be changed
  env_set(env, x->str_v, x->next);
  return make_copy(x->next);
}

Element *define(Env *env, Element *x) {
  if (x == NULL
      || x->next == NULL
      || x->type != T_SYMBOL) {
    return make_error(ERR_FUNCTION_X_EXPECTS_Y, "def", "a symbol and a list");
  }

  Element *error = _validate_symbol(x);
  if (error) { return error; }

  // TODO: check whether x can be changed
  return make_error(ERR_FUNCTION_X_NOT_IMPLEMENTED, "def");
  //return x->next;
}

Element *reference(Env *env, Element *x) {
  if (x == NULL || x->type != T_SYMBOL) {
    return make_error(ERR_FUNCTION_X_EXPECTS_Y, "ref", "a symbol");
  }

  // TODO: check whether e/e->tail/e->sub can be changed
  Element *e = _env_get(env, x->str_v);
  if (e && e->type == T_SYMBOL) {
    return reference(env, e);
  } else {
    return make_copy(e);
  }
}
