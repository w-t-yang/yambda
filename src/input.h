#ifndef YAMBDA_INPUT
#define YAMBDA_INPUT

#include "types.h"

void set_instream(FILE **instream, char *fname);
char peek(void);
Element *read_line();
Element *read_block();

#endif
