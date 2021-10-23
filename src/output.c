#include "output.h"

void throw(char *fmt, ...) {
  fprintf(stdout, "\nException: ");
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

void warn(char *fmt, ...){
  fprintf(stdout, "\nWarning: ");
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  fprintf(stdout, "\n");
  va_end(ap);
}

void p(char *fmt, ...){
  printf("\n");
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  fprintf(stdout, "\n");
  va_end(ap);
}

void print_ele(Element *ele) {
  if (!ele) {
    throw("Trying to print NULL pointer.");
  } else if (ele->type == T_NONE) {
    printf("None ");
  } else if (ele->type == T_ERROR) {
    throw(ele->str_v);
  } else if (ele->type == T_INTEGER) {
    printf("%d ", ele->int_v);
  } else if (ele->type == T_STRING) {
    printf("\"%s\" ", ele->str_v);
  } else if (ele->type == T_SYMBOL) {
    printf("%s ", ele->str_v);
  } else if ( ele->type == T_LISTHEAD) {
    printf("(");
    if (ele->sub) {
      print_list(ele->sub);
    } else {
      printf(" ");
    }
    printf("\b) ");
  } else if (ele->type == T_FUNC) {
    printf("%s ", ele->str_v);
  } else if (ele->type == T_LAMBDA) {
    printf("%s ", ele->str_v);
  } else {
    throw("Unknown element type %d.", ele->type);
  }
}

void print_list(Element *curr) {
  while (curr) {
    print_ele(curr);
    curr = curr->next;
  }
}

void print_env(Env *env) {
  p("Printing ENV:");

  Env *curr = env;
  while (curr) {
    printf("%s ", curr->symbol);
    curr = curr->next;
  }

  p("End of ENV");
}

void print_env_node(Env *env) {
  printf("Printing ENV node:");

  printf("Symbol: %s\n", env->symbol);
  print_list(env->list);

  printf("End of ENV node");
}
