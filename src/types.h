#ifndef YAMBDA_TYPES
#define YAMBDA_TYPES

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "keywords.h"

enum {
  // Types of element
  T_NONE = 0,
  T_ERROR,
  T_RETURN,
  T_INTEGER,
  T_STRING,
  T_SYMBOL,
  T_LISTHEAD,
  T_FUNC,
  T_LAMBDA,

  TYPES_COUNT
};

#define TYPE_MAX_LENGTH 9
static char TYPES[TYPES_COUNT][TYPE_MAX_LENGTH] = {
  "None",
  "Error",
  "Integer",
  "String",
  "Symbol",
  "List",
  "Function",
  "Lambda",
};

enum {
  // Values
  V_NORMAL_SYMBOL = 0,
  V_ARG_SYMBOL,
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

struct node{
  struct node *next;     // 8 bytes
  struct element *list; // 8 bytes
};
typedef struct node Node;
#define NODE_SIZE 16

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

#define IND_UNIT 4

#endif
