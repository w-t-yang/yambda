#include <ctype.h>

#include "input.h"
#include "output.h"
#include "utils.h"

#define BUFFER_SIZE 128

Element *read_list();

int peek(void) {
  int c = getchar();
  ungetc(c, stdin);
  return c;
}

int end_of_element(char c) {
  if (c == ' ' || c == EOF || c == '\r' || c == '\n' || c == ')') {
    return 1;
  } else {
    return 0;
  }
}

// TODO: For all `strncat(buffer...);`, allocate more space when buffer is used up

Element *read_integer() {
  int isnegative = 0;
  if (peek() == '-') {
    getchar();
    isnegative = 1;
  }

  char *buffer = malloc(BUFFER_SIZE);
  buffer[0] = '\0';

  for (;;) {
    char c = getchar();
    if (end_of_element(c)) {
      // Put '\n', '\r', etc. back to stdin
      ungetc(c, stdin);

      if (strlen(buffer) == 0) {
        return make_error("Invalid integer.");
      } else {
        int i = atoi(buffer);
        if (isnegative) { i = -i; }
        return make_integer(i);
      }
    } else {
      if (isdigit(c)) {
        strncat(buffer, &c, 1);
      } else {
        return make_error("Invalid integer \"%s%c...\"", buffer, c);
      }
    }
  }
}

Element *read_string() {
  char quote = getchar();
  char *buffer = malloc(BUFFER_SIZE);
  buffer[0] = '\0';

  for (;;) {
    char c = getchar();
    if (c == EOF) {
      return make_error("Invalid string. End of file reached.");
    } else if (c == quote) {
      if (end_of_element(peek())){
        return make_string(buffer);
      } else {
        return make_error("End of string should be followed by whitespace.");
      }
    } else if ( c == '\\') {
      strncat(buffer, &c, 1);
      c = getchar();
      if (c == EOF) {
        return make_error("Invalid string. End of file reached.");
      }
      strncat(buffer, &c, 1);
    } else {
      strncat(buffer, &c, 1);
    }
  }
}

Element *read_symbol() {
  // Notice:
  // T_FUNC and T_LAMBDA will be read as T_SYMBOL.
  // This is not a problem because,
  // when referencing symbol of T_FUNC/T_LAMBDA,
  // we will eventually get the T_FUNC/T_LAMBDA element from the Environment.
  char *buffer = malloc(BUFFER_SIZE);
  buffer[0] = '\0';

  for (;;) {
    char c = getchar();
    if (end_of_element(c)) {
      ungetc(c, stdin);
      return make_symbol(buffer);
    } else {
      // TODO: check invalid character
      strncat(buffer, &c, 1);
    }
  }
}

Element *read_element() {
  char s = peek();
  // Types of elements that can be read
  // 1. Number, s is digit or s == '-', TODO: Support numbers other than integer
  // 2. String, s == '"' or s == '\''
  // 3. List, s == '()'
  // 4. Symbol, TODO: Define invalid starting char for a symbol

  if (end_of_element(s)) {
    getchar();
    return none;
  } else if (isdigit(s) || s == '-') {
    return read_integer();
  } else if (s == '"' || s == '\'') {
    return read_string();
  } else if (s == '(') {
    return read_list();
  } else {
    return read_symbol();
  }
}

Element *read_list() {
  // Types of list
  // 1. whole line
  // 2. lines started with proper indentation, not supported yet [TODO]
  // 3. strings surrounded by ()
  Element *head = NULL;
  Element *tail = NULL;

  int start_with_parenthesis = 0;
  if (peek() == '(') {
    start_with_parenthesis = 1;
    getchar();
  }

  for (;;) {
    char c = peek();
    Element *ele = NULL;

    if (c == '\t') {
      return make_error("DO NOT use \\t in yambda script.");
    }

    if (start_with_parenthesis) {
      if (c == EOF) {
        return make_error("Unclosed parenthesis.");
      } else if (c == ')') {
        getchar();
        Element *lh = make_list_head();
        lh->sub = head;
        return lh;
      } else if (c == '\n' || c == '\r') {
        // TODO: support indentation
        getchar();
        continue;
      } else if (c == ' ') {
        getchar();
        continue;
      } else {
        ele = read_element();
      }
    } else {
      if (c == EOF) {
        return head;
      } else if (c == ')') {
        getchar();
        return make_error("Invalid parenthesis ')'");
      } else if (c == '\n' || c == '\r') {
        // TODO: support indentation
        getchar();
        return head;
      } else if (c == ' ') {
        getchar();
        continue;
      } else {
        ele = read_element();
      }
    }

    if (!ele) {
      // Do nothing
    } else if (!head) {
      head = tail = ele;
    } else {
      tail->next = ele;
      tail = ele;
    }
  }
}

Element *read_block() {
  Element *head = NULL;
  int start_with_parenthesis = (peek() == '(');
  head = read_list();
  if (start_with_parenthesis) {
    Element *rest = read_block();
    head->next = rest;
  }
  return head;
}
