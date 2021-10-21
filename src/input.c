#include <ctype.h>
#include <stdio.h>

#include "input.h"
#include "output.h"
#include "utils.h"

#define BUFFER_SIZE 128

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

// To be removed
/* Element *read_element_old(char s) { */
/*   char *buffer = malloc(BUFFER_SIZE); */
/*   buffer[0] = s; */
/*   buffer[1] = '\0'; */
/*   int expecting_digit = (s == '-' || isdigit(s)); */

/*   for (;;) { */
/*     char c = getchar(); */
/*     if (end_of_element(c)) { */
/*       // Put '\n' '\r' back to stdin */
/*       ungetc(c, stdin); */

/*       if (expecting_digit) { */
/*         // Special case when buffer starts with "-" */
/*         if (strlen(buffer) == 1 && buffer[0] == '-') { */
/*           return make_symbol(buffer); */
/*         } else { */
/*           return make_integer(atoi(buffer)); */
/*         } */
/*       } else { */
/*         return make_symbol(buffer); */
/*       } */
/*     } else { */
/*       if (expecting_digit && !isdigit(c)) { */
/*         error("Expecting digit after \"%s\"", buffer); */
/*       } */
/*       // TODO: Allocate more space when buffer is used up */
/*       strncat(buffer, &c, 1); */
/*     } */
/*   } */
/* } */

// To be removed
/* Element *read_expression_string() { */
/*   char *buffer = malloc(BUFFER_SIZE); */
/*   buffer[0] = '\0'; */
/*   char s = '('; */
/*   char e = ')'; */
/*   int s_count = 0; */

/*   for (;;) { */
/*     char c = getchar(); */
/*     if (c == s) { */
/*       s_count += 1; */
/*       strncat(buffer, &c, 1); */
/*     } else if (c == e) { */
/*       if (s_count > 0) { */
/*         s_count -= 1; */
/*         strncat(buffer, &c, 1); */
/*       } else { */
/*         char next = peek(); */
/*         if (end_of_element(next)){ */
/*           return make_expression(buffer); */
/*         } else { */
/*           error("Invalid expression format. End of expression should be followed by ' '."); */
/*         } */
/*       } */
/*     } else if (c == EOF) { */
/*       error("Invalid expression format. End of file reached."); */
/*     } else { */
/*       strncat(buffer, &c, 1); */
/*     } */
/*   } */
/* } */

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
      // Put '\n' '\r' back to stdin
      ungetc(c, stdin);

      if (strlen(buffer) == 0) {
        error("Invalid integer.");
        return NULL;
      } else {
        int i = atoi(buffer);
        if (isnegative) {
          i = -i;
        }
        return make_integer(i);
      }
    } else {
      if (isdigit(c)) {
        strncat(buffer, &c, 1);
      } else {
        error("Invalid integer \"%s%c...\"", buffer, c);
        return NULL;
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
      error("Invalid string. End of file reached.");
      return NULL;
    } else if (c == quote) {
      char next = peek();
      if (end_of_element(next)){
        return make_string(buffer);
      } else {
        error("Invalid string. End of string should be followed by whitespace.");
      }
    } else if ( c == '\\') {
      strncat(buffer, &c, 1);

      c = getchar();
      if (c == EOF) {
        error("Invalid string. End of file reached.");
        return NULL;
      }
      strncat(buffer, &c, 1);
    } else {

      strncat(buffer, &c, 1);
    }
  }
}

Element *read_symbol() {
  char *buffer = malloc(BUFFER_SIZE);
  buffer[0] = '\0';

  for (;;) {
    char c = getchar();
    if (end_of_element(c)) {
      // Put '\n' '\r' back to stdin
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
  // Types of elements
  // 1. Number, s is digit or s == '-', TODO: Support numbers other than integer
  // 2. String, s == '"' or s == '\''
  // 3. List, s == '()'
  // 4. Symbol, TODO: Define invalid starting char for a symbol

  if (end_of_element(s)) {
    getchar();
    return NULL;
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
  // 2. lines started with proper indentation, not supported yet
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
      error("Do NOT use \\t in yambda script");
      return NULL;
    }

    if (start_with_parenthesis) {
      if (c == EOF) {
        error("Unclosed parenthesis.");
        return NULL;
      } else if (c == ')') {
        getchar();
        Element *lh = make_list_head();
        lh->args = head;
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
        error("Invalid parenthesis ')'");
        return NULL;
      } else if (c == '\n' || c == '\r') {
        // TODO: support indentation
        getchar();
        if (!head) continue;
        return head;
      } else if (c == ' ') {
        getchar();
        continue;
      } else {
        ele = read_element();
      }
    }

    //debug("Read element of type: %d", ele->type);

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
