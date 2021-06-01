#include "env.h"
#include "output.h"

// TODO:
// 1. In order to improve performance, re-implment env by using hashmap or trie
// 2. Implement nested env, instead of using global env

Env *make_env_node(char *s) {
  Env *env = malloc(16 + strlen(s) + 1);
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
  error("Fail to get symbol %s.", symbol);
  return NULL;
}

void env_unset(Env *env, char *symbol) {
  Env *curr = env;
  while (curr) {
    if (streq(curr->symbol, symbol)) {
      //free(curr->list);
      curr->list = NULL;
      return;
    }
    curr = curr->next;
  }
  error("Fail to unset symbol %s.", symbol);
}

Element *env_set(Env *env, char *symbol, Element *list) {
  if (strlen(symbol) == 0) {
    error("Invalid symbol name.");
    return NULL;
  }
  if (!env) {
    error("Environment is not initialized.");
  }

  Env *curr = env;
  while (curr) {
    if (streq(curr->symbol, symbol)) {
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
