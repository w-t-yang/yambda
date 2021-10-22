#include "yambda.h"

#define try ERROR_FLAG=0;
#define catch(x) EXIT_POINT: if(ERROR_FLAG)
#define throw(x) ERROR_FLAG=1;goto EXIT_POINT;

int main(int argc, char **argv) {
  Env *env = env_init();
  init_funcs(env);
  //print_env(env);

  int cnt = 0;
  for (;;) {
    cnt ++;

    Element *ele = read_list();
    if (!ele) continue;

    printf("\nList: ");
    print_list(ele);

    Element *res = eval(env, ele);
    printf("\nEval: ");
    print_list(res);

    printf("\n");
  }
}
