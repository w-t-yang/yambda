#include <ctype.h>

#include "input.h"
#include "output.h"
#include "utils.h"

#define BUFFER_SIZE 128

Element *read_list();

FILE **_in;

void set_instream(FILE **f) {
  _in = f;
}

char _getc() {
  return getc(*_in);
}

void _ungetc(char c) {
  ungetc(c, *_in);
}

char peek(void) {
  char c = _getc();
  _ungetc(c);
  return c;
}

void _kill_line() {
  char c = _getc();
  while(c != '\r' && c != '\n' && c != EOF) {
    c = _getc();
  }
  _getc();
}

void _kill_line_with_msg(int size, char **buffer) {
  int index = 0;
  char c = _getc();
  while(c != '\r' && c != '\n' && c != EOF) {
    if (index < size-1) {
      (*buffer)[index] = c;
      index++;
    }
    c = _getc();
  }
  c = _getc();
  (*buffer)[index] = '\0';
}

int _get_indentation() {
  int ind = 0;
  char c = _getc();
  while (c == ' ' || c == '\r' || c == '\n') {
    if (c == ' ') { ind++; }
    else { ind = 0; }
    c = _getc();
  }
  if (c == EOF) { return 0; }
  else {
    _ungetc(c);
    return ind;
  }
}

void _reset_indentation(int ind) {
  for (int i = 0; i < ind; i++) { _ungetc(' '); }
}

int _peek_indentation() {
  int ind = _get_indentation();
  for (int i = 0; i < ind; i++) { _ungetc(' '); }
  return ind;
}

#define MSG_BUFFER_SIZE 10
Element *_make_indentation_error() {
  char *msg = malloc(MSG_BUFFER_SIZE);
  _kill_line_with_msg(MSG_BUFFER_SIZE, &msg);
  return make_error("Invalid indentation at \"%s\".", msg);
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
    _getc();
    isnegative = 1;
  }

  char *buffer = malloc(BUFFER_SIZE);
  buffer[0] = '\0';

  for (;;) {
    char c = _getc();
    if (end_of_element(c)) {
      // Put '\n', '\r', etc. back to instream
      _ungetc(c);

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
  char quote = _getc();
  char *buffer = malloc(BUFFER_SIZE);
  buffer[0] = '\0';

  for (;;) {
    char c = _getc();
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
      c = _getc();
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
  // T_FUNCS and T_LAMBDA will be read as T_SYMBOL.
  // This is not a problem because,
  // when referencing symbol of T_FUNCS/T_LAMBDA,
  // we will eventually get the T_FUNCS/T_LAMBDA element from the Environment.
  char *buffer = malloc(BUFFER_SIZE);
  buffer[0] = '\0';

  for (;;) {
    char c = _getc();
    if (end_of_element(c)) {
      _ungetc(c);
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
    _getc();
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
    _getc();
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
        _getc();
        Element *lh = make_list_head();
        lh->sub = head;
        return lh;
      } else if (c == '\n' || c == '\r') {
        // TODO: support indentation
        _getc();
        continue;
      } else if (c == ' ') {
        _getc();
        continue;
      } else {
        ele = read_element();
      }
    } else {
      if (c == EOF) {
        return head;
      } else if (c == ')') {
        _getc();
        return make_error("Invalid parenthesis ')'");
      } else if (c == '\n' || c == '\r') {
        // TODO: support indentation
        _getc();
        return head;
      } else if (c == ' ') {
        _getc();
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

Element *read_line() {
  Element *head = NULL;
  int start_with_parenthesis = (peek() == '(');
  head = read_list();
  if (start_with_parenthesis) {
    Element *rest = read_line();
    head->next = rest;
  }
  return head;
}

Element *read_line_as_single_ele() {
  Element *list = read_line();
  Element *head = make_list_head();
  head->sub = list;
  return head;
}

#define IND_UNIT 4
Element *read_block() {
  int ind = _get_indentation();
  if (ind != 0) { return _make_indentation_error(); }

  Element *head = read_line();
  if (!head) { return NULL; }

  Element *fake_head = make_list_head();
  fake_head->sub = head;
  Node *tails = NULL;
  push(&tails, fake_head);
  int stack_h = 0;

  int next_ind = _get_indentation();
  while(next_ind > ind) {
    int prev_ind = IND_UNIT * stack_h;
    if (next_ind / IND_UNIT * IND_UNIT != next_ind) {
      return _make_indentation_error();
    } else if (next_ind > prev_ind + IND_UNIT) {
      return _make_indentation_error();
    } else if (next_ind == prev_ind) {
      Element *e = read_line_as_single_ele();
      Element *tail = tails->list;
      tail->next = e;
      tail = e;
      pop(&tails);
      push(&tails, tail);

      next_ind = _get_indentation();
    } else if (next_ind == prev_ind + IND_UNIT) {
      Element *e = read_line_as_single_ele();
      Element *tail = tails->list;
      tail = tail_of(tail->sub);
      tail->next = e;
      tail = e;
      push(&tails, tail);
      stack_h++;

      next_ind = _get_indentation();
    } else {
      pop(&tails);
      stack_h--;
    }
  }
  return head;
}
