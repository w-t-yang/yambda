#ifndef YAMBDA_TYPES
#define YAMBDA_TYPES

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum {
  // Types of element
  T_INTEGER = 0,
  T_STRING,
  T_SYMBOL,
  T_LISTHEAD,
};

enum {
  // Values
  V_NORMAL_SYMBOL = 0,
  V_FUNC_SYMBOL,
};

struct element{
  struct element *next; // 8 bytes
  struct element *args; // 8 bytes
  int type;  int int_v; // 8 bytes
  char str_v[1];        // 8 bytes will be allocated
};

typedef struct element Element;

struct env{
  struct env *next;     // 8 bytes
  struct element *list; // 8 bytes
  char symbol[1];       // 8 bytes will be allocated
};
typedef struct env Env;

#define streq(x, y) !strcmp(x, y)

#define ARGS_SYMBOL "@_"

static int ERROR_FLAG = 0;

#endif
