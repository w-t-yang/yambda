#ifndef YAMBDA_KEYWORDS
#define YAMBDA_KEYWORDS

enum {
  K_EMPTY = 0,
  K_NONE,
  K_ERROR,

  K_LET,
  K_DEF,
  K_REF,

  K_SUM,
  K_MUL,
  K_DIV,

  K_QUOTE,
  K_ATOM,
  K_EQ,
  K_CAR,
  K_CDR,
  K_CONS,
  K_COND,

  KEYWORDS_COUNT,
};

#define KEYWORD_MAX_LENGTH 6
static char KEYWORDS[KEYWORDS_COUNT][KEYWORD_MAX_LENGTH] = {
  "",
  "NONE",
  "ERROR",

  "let",
  "def",
  "ref",

  "+",
  "*",
  "/",

  "`",
  "a?",
  "=?",
  "car",
  "cdr",
  "cons",
  "cond",
};

#endif
