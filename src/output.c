#include <stdarg.h>
#include <stdio.h>

#include "output.h"

void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}

void debug(char *fmt, ...){
  //ifndef DEBUG
  //return;
  //endif
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  fprintf(stdout, "\n");
  va_end(ap);
}

void print_ele(Element *ele) {
  if (!ele) {
    printf("<Null>");
  } else if ( ele->type == T_LISTHEAD) {
    printf("(");
    print_list(ele->args);
    printf("\b) ");
  } else if (ele->type == T_SYMBOL) {
    //printf("<Symbol: %s>", ele->str_v);
    printf("%s ", ele->str_v);
  } else if (ele->type == T_INTEGER) {
    //printf("<Integer: %d>", ele->int_v);
    printf("%d ", ele->int_v);
  } else if (ele->type == T_STRING) {
    //printf("<String: \"%s\">", ele->str_v);
    printf("\"%s\" ", ele->str_v);
  } else {
    printf("<Unknown element type>");
  }
}

void print_list(Element *curr) {
  while (curr) {
    print_ele(curr);
    curr = curr->next;
  }
}

void print_env(Env *env) {
  printf("\nPrinting ENV: \n");

  Env *curr = env;
  while (curr) {
    printf("%s -> ", curr->symbol);
    print_list(curr->list);
    curr = curr->next;
  }
  printf("End of ENV\n");
}

void print_env_node(Env *env) {
  printf("\nPrinting ENV node: \n");
  printf("Symbol: %s\n", env->symbol);
  print_list(env->list);
  printf("End of ENV node\n");
}
