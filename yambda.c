#include <stdlib.h>
#include <stdio.h>

#include "src/env.h"
#include "src/input.h"
#include "src/output.h"
#include "src/primitives.h"
#include "src/types.h"

#define try ERROR_FLAG=0;
#define catch(x) EXIT_POINT: if(ERROR_FLAG)
#define throw(x) ERROR_FLAG=1;goto EXIT_POINT;

int main(int argc, char **argv) {
  Env *env = env_init();

  int cnt = 0;
  for (;;) {
    cnt ++;

    Element *ele = read_expression(0);
    if (!ele) continue;

    printf("\nList: ");
    print_list(ele);

    Element *res = eval(env, ele);
    printf("Eval: ");
    print_list(res);

    printf("\n");
  }
}
