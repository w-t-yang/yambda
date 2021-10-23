#include "yambda.h"

#define try ERROR_FLAG=0;
#define catch(x) EXIT_POINT: if(ERROR_FLAG)
#define throw(x) ERROR_FLAG=1;goto EXIT_POINT;

int main(int argc, char **argv) {
  Env *env = env_init();

  int cnt = 0;
  for (;;) {
    cnt ++;

    Element *ele = read_block();
    if (!ele) continue;

    printf("List: ");
    plst(ele);

    Element *res = eval(env, ele);
    printf("\nEval: ");
    plst(res);

    printf("\n\n");

    if (peek() == EOF) { exit(1); }
  }
}
