#include "yambda.h"

int main(int argc, char **argv) {
  if (argc == 1) {
    p("Shell mode not implemented.");
    exit(1);
  } else {
    FILE *in = fopen(argv[1] , "r");
    if (!in) { throw("File not found."); }
    p("File to parse: %s", argv[1]);
    set_instream(&in);
  }

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
