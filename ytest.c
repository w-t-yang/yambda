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
  p("Test file: %s", argv[1]);

  Env *env = env_init();

  int cnt = 0;
  int failed_cnt = 0;
  for (;;) {
    cnt ++;

    Element *l1 = read_block();
    Element *l2 = read_block();
    Element *e1 = eval(env, l1);
    Element *e2 = eval(env, l2);

    boolean failed = !lsteq(e1, e2);
    if (failed) {
      failed_cnt++;
      p("The following test failed");
      printf("List 1: ");
      plst(l1);
      printf("\nEval 1: ");
      plst(e1);
      printf("\nList 2: ");
      plst(l2);
      printf("\nEval 2: ");
      plst(e2);
      printf("\n\n");
    }

    if (peek() == EOF) {
      if (failed_cnt) {
        p("Summary: %d out of %d tests failed.", failed_cnt, cnt);
      } else {
        p("Summary: all %d tests passed.", cnt);
      }
      p("---------------------------");
      exit(1);
    }
  }
}
