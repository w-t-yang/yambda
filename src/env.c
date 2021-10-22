#include "env.h"
#include "output.h"

// TODO:
// 1. In order to improve performance, re-implment env by hashmap or trie
// 2. Use nested env, instead of global env

Env *make_env_node(char *s) {
  // We need strlen(s) bytes, and 1 byte for '\0'
  // We have already allocated 8 bytes when defining `char symbol[1];`
  int needed = strlen(s) + 1;
  Env *env = malloc(24 + (needed > 8 ? needed - 8 : 0));
  env->list = NULL;
  env->next = NULL;
  strcpy(env->symbol, s);
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
  error("Symbol \"%s\" not found.", symbol);
  return NULL;
}

void env_unset(Env *env, char *symbol) {
  Env *curr = env;
  while (curr) {
    if (streq(curr->symbol, symbol)) {
      // TODO: review the use of `free`
      free(curr->list);
      curr->list = NULL;
      return;
    }
    curr = curr->next;
  }
  error("Symbol \"%s\" not found.", symbol);
}

Element *env_set(Env *env, char *symbol, Element *list) {
  if (strlen(symbol) == 0) {
    error("Invalid symbol name \"%s\".", symbol);
    return NULL;
  }
  if (!env) {
    error("Environment is not initialized.");
  }

  Env *curr = env;
  while (curr) {
    if (streq(curr->symbol, symbol)) {
      // TODO: review the use of `free`
      free(curr->list);
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
  error("This error is not expected.");
  return NULL;
}

Element *define(Env *env, char *symbol, Element *x) {
  env_set(env, symbol, x);
  return x;
}

Element *reference(Env *env, char *symbol) {
  Element *e = env_get(env, symbol);
  if (e->type == T_INTEGER || e->type == T_STRING || e->type == T_LISTHEAD) {
    return e;
  } else if (e->type == T_SYMBOL) {
    if (e->int_v == V_FUNC_SYMBOL) {
      return e;
    } else {
      return reference(env, e->str_v);
    }
  }
  error("Unknown reference type");
  return NULL;
}
