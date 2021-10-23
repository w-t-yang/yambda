#ifndef YAMBDA_TYPES
#define YAMBDA_TYPES

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keywords.h"

enum {
  // Types of element
  T_NONE = 0,
  T_ERROR,
  T_INTEGER,
  T_STRING,
  T_SYMBOL,
  T_LISTHEAD,
  T_PRIM,
  T_LAMBDA,
};

enum {
  // Values
  V_LIST_UNEVALED = 0,
  V_LIST_EVALED,
};

struct element{
  struct element *next; // 8 bytes
  struct element *sub; // 8 bytes
  int type;  int int_v; // 8 bytes
  char str_v[1];        // 8 bytes will be allocated
};
typedef struct element Element;
#define ELEMENT_MIN_SIZE 32

struct env{
  struct env *next;     // 8 bytes
  struct element *list; // 8 bytes
  char symbol[1];       // 8 bytes will be allocated
};
typedef struct env Env;
#define ENV_MIN_SIZE 24

#define streq(x, y) !strcmp(x, y)

//#define ARGS_SYMBOL "@_"
//static int ERROR_FLAG = 0;

// When allocating space for a string,
// We need strlen(buffer) bytes, and 1 byte for '\0'
// We have already allocated 8 bytes when defining `char str_v[1]` or `char symbol[1]`
// So the extra needed space is defined as below
#define ADJUST_NEEDED_SPACE(x) (x > 8 ? x - 8 : 0)
#define EXTRA_SPACE (strlen(buffer)+1 > 8 ? strlen(buffer) - 7 : 0)

typedef int boolean;
#define true 1
#define false 0

#endif
