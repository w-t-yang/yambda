#include <stdlib.h>
#include <stdio.h>

#include "src/types.h"
#include "src/input.h"
#include "src/output.h"

int main(int argc, char **argv) {
  // Debug flags
  //debug_gc = getEnvFlag("MINILISP_DEBUG_GC");
  //always_gc = getEnvFlag("MINILISP_ALWAYS_GC");

  // Memory allocation
  //memory = alloc_semispace();

  // Constants and primitives
  //Symbols = Nil;
  //void *root = NULL;
  //DEFINE2(env, expr);
  //*env = make_env(root, &Nil, &Nil);
  //define_constants(root, env);
  //define_primitives(root, env);

  // The main loop
  int cnt = 0;
  for (;;) {
    cnt ++;

    Element *ele = read_expression(0);
    if (!ele) return 0;

    printf("%dth list\n", cnt);
    print_list(ele);
  }
}
