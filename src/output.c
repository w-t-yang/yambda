#include "output.h"

void throw(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  fprintf(stderr, "Exception: ");
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

void print_ele(Element *ele, boolean show_type) {
  if (!ele) {
    //throw("Trying to print NULL pointer.");
    printf("NULL ");
    return;
  }

  if (show_type) {
    printf("Type: %d Value: ", ele->type);
  }

  switch (ele->type) {
  case T_NONE:
    printf("NONE ");
    break;
  case T_ERROR:
    printf("ERROR: %s ", ele->str_v);
    break;
  case T_INTEGER:
    printf("%d ", ele->int_v);
    break;
  case T_STRING:
    printf("\"%s\" ", ele->str_v);
    break;
  case T_SYMBOL:
    printf("%s ", ele->str_v);
    break;
  case T_LISTHEAD:
    printf("(");
    if (ele->sub) {
      print_list(ele->sub);
    } else {
      printf(" ");
    }
    printf("\b) ");
    break;
  case T_FUNC:
    printf("%s ", ele->str_v);
    break;
  case T_LAMBDA:
    printf("%s ", ele->str_v);
    break;
  default:
    throw("Unknown element type %d.", ele->type);
  }
}

void print_list(Element *curr) {
  if (!curr) { print_ele(curr, false); }

  while (curr) {
    print_ele(curr, false);
    curr = curr->next;
  }
}

void print_env(Env *env) {
  p("ENV:");

  Env *curr = env;
  while (curr) {
    printf("%s ", curr->symbol);
    curr = curr->next;
  }

  p("End of ENV");
}

void print_env_node(Env *env) {
  printf("ENV node:");

  printf("Symbol: %s\n", env->symbol);
  print_list(env->list);

  printf("End of ENV node");
}
