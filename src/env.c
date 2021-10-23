#include "env.h"
#include "output.h"
#include "utils.h"

boolean is_keyword(char *k) {
  for (int i = 0; i < KEYWORDS_COUNT; i++) {
    if (streq(k, KEYWORDS[i])) {
      return true;
    }
  }
  return false;
}

void validate_symbol(char *k) {
  if (is_keyword(k)) {
    throw("Cannot use \"%s\" as environment symbol.", k);
  }
}

// TODO:
// 1. In order to improve performance, re-implment env by hashmap or trie
// 2. Use nested env, instead of global env

Env *make_env_node(char *buffer) {
  validate_symbol(buffer);

  Env *env = malloc(24 + EXTRA_SPACE);
  env->list = NULL;
  env->next = NULL;
  strcpy(env->symbol, buffer);
  return env;
}

Env *env_init() {
  return make_env_node("");
}

Element *env_get(Env *env, char *symbol) {
  Env *curr = env;
  while (curr) {
    if (streq(curr->symbol, symbol)) {
      return curr->list;
    }
    curr = curr->next;
  }
  return make_error("Symbol \"%s\" not found.", symbol);
}

Element *env_unset(Env *env, char *symbol) {
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

Element *env_set(Env *env, char *symbol, Element *list) {
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
      Env *e = make_env_node(symbol);
      e->list = list;
      curr->next = e;
      return e->list;
    }
  }
  return make_error("This error is not expected.");
}

Element *define(Env *env, char *symbol, Element *x) {
  env_set(env, symbol, x);
  return x;
}

Element *reference(Env *env, char *symbol) {
  Element *e = env_get(env, symbol);
  if (e->type == T_SYMBOL) {
    return reference(env, e->str_v);
  } else {
    return e;
  }
}
