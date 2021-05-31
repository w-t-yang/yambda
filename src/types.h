#ifndef YAMBDA_TYPES
#define YAMBDA_TYPES

enum {
  // Types of element
  // Expression type should be eliminated
  // because each expression should be parsed into a list starting with a symbol
  T_EXPRESSION = 0,
  T_SYMBOL,
  T_VALUE,

  // Types of value element
  TV_INT,
  TV_STRING,
};

struct element{
  struct element *next; // 8 bytes
  struct element *args; // 8 bytes
  int type;  int int_v; // 8 bytes
  char str_v[1];        // 8 bytes will be allocated
};

typedef struct element Element;

#endif
