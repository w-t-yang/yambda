#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "input.h"
#include "output.h"

// Size for 'type' and pointer for Element
#define ELEMENT_MIN_SIZE 32
#define BUFFER_SIZE 128

int peek(void) {
  int c = getchar();
  ungetc(c, stdin);
  return c;
}
int end_of_element(char c) {
  if (c == ' ' || c == EOF || c == '\r' || c == '\n') {
    return 1;
  } else {
    return 0;
  }
}

Element *alloc(int type, size_t size) {
  // This might allocate a little bit more memory than needed
  // Because the last 8 bytes of Element struct were only used by str_v[1]
  // Possible to save 6-7 bytes by some optimization
  size += ELEMENT_MIN_SIZE;
  Element *ele = malloc(size);
  ele->type = type;
  return ele;
}

Element *make_integer(char *buffer) {
  Element *ele = alloc(TV_INT, sizeof(int));
  ele->int_v = atoi(buffer);
  return ele;
}

Element *make_string(char *buffer) {
  Element *ele = alloc(TV_STRING, strlen(buffer) + 1);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_expression(char *buffer) {
  // TODO: make real expression
  Element *ele = alloc(T_EXPRESSION, strlen(buffer) + 1);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *make_symbol(char *buffer) {
  // TODO: validate symbol
  Element *ele = alloc(T_SYMBOL, strlen(buffer) + 1);
  strcpy(ele->str_v, buffer);
  return ele;
}

Element *read_element(char s) {
  char *buffer = malloc(BUFFER_SIZE);
  buffer[0] = s;
  buffer[1] = '\0';
  int expect_digit = (s == '-' || isdigit(s));

  for (;;) {
    char c = getchar();
    if (end_of_element(c)) {
      // Put '\n' '\r' back to stdin
      ungetc(c, stdin);

      if (expect_digit) {
        // Special case when buffer = "-"
        if (strlen(buffer) == 1 && buffer[0] == '-') {
          return make_symbol(buffer);
        } else {
          return make_integer(buffer);
        }
      } else {
        return make_symbol(buffer);
      }
    } else {
      if (expect_digit && !isdigit(c)) {
        error("Expecting digit for %s", buffer);
      }
      strncat(buffer, &c, 1);
    }
  }
}

Element *read_expression_string() {
  char *buffer = malloc(BUFFER_SIZE);
  buffer[0] = '\0';
  char s = '(';
  char e = ')';
  int s_count = 0;

  for (;;) {
    char c = getchar();
    if (c == s) {
      s_count += 1;
      strncat(buffer, &c, 1);
    } else if (c == e) {
      if (s_count > 0) {
        s_count -= 1;
        strncat(buffer, &c, 1);
      } else {
        char next = peek();
        if (end_of_element(next)){
          return make_expression(buffer);
        } else {
          error("Invalid expression format. End of expression should be followed by ' '.");
        }
      }
    } else if (c == EOF) {
      error("Invalid expression format. End of file reached.");
    } else {
      strncat(buffer, &c, 1);
    }
  }
}

Element *read_string(char quote) {
  char *buffer = malloc(BUFFER_SIZE);
  buffer[0] = '\0';
  for (;;) {
    char c = getchar();
    if (c == quote) {
      char next = peek();
      if (end_of_element(next)){
        return make_string(buffer);
      } else {
        error("Invalid string format. End of string should be followed by ' '.");
      }
    } else if ( c == '\\') {
      strncat(buffer, &c, 1);
      c = getchar();
      strncat(buffer, &c, 1);
    } else {
      strncat(buffer, &c, 1);
    }
  }
}

Element *read_expression(int with_paren) {
  // There are 2 syntaxes of expressions
  // 1. whole line (and more lines followed with proper indentation, not supported yet)
  // 2. strings surrounded by ()
  Element *head = NULL;
  Element *tail = NULL;

  for (;;) {
    char c = getchar();
    Element *ele = NULL;

    if (c == '\t') {
      error("Do NOT use \\t in yambda script");
    } else if (c == '"' || c == '\'') {
      ele = read_string(c);
    } else if (c == '(') {
      ele = read_expression_string();
    } else if (c == EOF) {
      return head;
    } else if (c == '\n' || c == '\r') {
      // TODO: for \n and \r,
      // if next line started with indentation,
      // consider it as an expression appended to current line
      if (!head) continue;
      return head;
    } else if (c == ' ') {
      continue;
    } else {
      ele = read_element(c);
    }

    //debug("Read element of type: %d", ele->type);

    if (!ele) {
      // Do nothing
    } else if (!head) {
      head = ele;
    } else if (!tail) {
      tail = ele;
      head->args = tail;
    } else {
      tail->next = ele;
      tail = ele;
    }
  }
}
