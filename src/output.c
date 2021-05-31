#include <stdarg.h>
#include <stdlib.h>
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
  #ifndef DEBUG
  return;
  #endif
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}

void print_element(Element *ele) {
  if (!ele) {
    printf("<Null>");
  } else if ( ele->type == T_EXPRESSION) {
    printf("<Expression: %s>", ele->str_v);
  } else if ( ele->type == T_SYMBOL) {
    printf("<Symbol: %s>", ele->str_v);
  } else if ( ele->type == TV_INT) {
    printf("<Integer: %d>", ele->int_v);
  } else if ( ele->type == TV_STRING) {
    printf("<String: \"%s\">", ele->str_v);
  } else {
    printf("<Unknown element type>");
  }
}

void print_list(Element *curr) {
  print_element(curr);
  curr = curr->args;

  while (curr) {
    print_element(curr);
    curr = curr->next;
  }
  printf("\n");
}
