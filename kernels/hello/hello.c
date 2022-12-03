#include <am.h>
#include <klib.h>
#include <klib-macros.h>

int main(const char *args) {
  const char *fmt =
    "Hello, AbstractMachine!\n"
    "mainargs = '%'.\n";

  for (const char *p = fmt; *p; p++) {
    (*p == '%') ? putstr(args) : putch(*p);
  }

  const char *string = "I love YSYX";
  printf("%s\n", string);
  printf("Finish printf\n");
  return 0;
}
