#ifndef YAMBDA_INPUT
#define YAMBDA_INPUT

#include "types.h"

void set_instream(FILE **instream);
int peek(void);
Element *read_block();

#endif
