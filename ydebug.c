#include "yambda.h"

int main(int argc, char **argv) {
  if (argc == 1) {
    p("Shell mode not implemented.");
    exit(1);
  } else {
    FILE *in = fopen(argv[1] , "r");
    if (!in) { throw("File not found."); }
    set_instream(&in);
  }

  Env *env = env_init();

  int cnt = 0;
  for (;;) {
    cnt ++;

    Element *ele = read_block();
    if (!ele) {
      if (peek() == EOF) { exit(1); }
      continue;
    }

    p("List: ");
    plst(ele);
    p("Deep Copy: ");
    Element *copy = make_deep_copy(ele);
    plst(copy);

    Element *res = eval(env, copy);
    p("Eval: ");
    plst(res);

    printf("\n\n");

    if (peek() == EOF) { exit(1); }
  }
}
