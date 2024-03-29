#include <klibtest.h>
#include <limits.h>

int length[] = {0,           INT_MAX / 17,  INT_MAX,      INT_MIN,
                INT_MIN + 1, UINT_MAX / 17, INT_MAX / 17, UINT_MAX};
char *num_s[] = {"0",           "126322567", "2147483647", "-2147483648",
             "-2147483647", "252645135", "126322567",  "-1"};

char *s[] = {"Hello, world!", "Hello\n", " Hello", "\tHello\t\n", "Hello%"};
#define NR_NUM_DATA LENGTH(num_s)
#define NR_STR_DATA LENGTH(s)

void test_sprintf() {
  for (int i = 0; i < NR_NUM_DATA; i++) {
    char str[20];
    sprintf(str, "%d", length[i]);
    assert(strcmp(num_s[i], str) == 0);
  }

  for (int i = 0; i < NR_STR_DATA; i++) {
    char str[20];
    sprintf(str, "%s", s[i]);
    assert(strcmp(s[i], str) == 0);
  }

  char str[20];
  sprintf(str, "%s", s[0]);
  assert(strcmp(s[0], str) == 0);

  sprintf(str, "%s\n", "Hello");
  assert(strcmp(s[1], str) == 0);

  sprintf(str, " %s", "Hello");
  assert(strcmp(s[2], str) == 0);

  sprintf(str, "\t%s\t\n", "Hello");
  assert(strcmp(s[3], str) == 0);

  sprintf(str, "%s%%", "Hello");
  assert(strcmp(s[4], str) == 0);

  sprintf(str, "%s%% %d", "Hello", 123);
  assert(strcmp("Hello% 123", str) == 0);

  uintptr_t a = 0x0000000a00001800;
  sprintf(str, "%lx\n", a);
  printf("%lx\n", a);
  uintptr_t b = 0xffffffffffffffff;
  sprintf(str, "%lx\n", b);
  printf("%lx\n", b);
  uintptr_t c = 0x80012fc0;
  printf("%lx\n", c);

  int x = 50;
  int *ptr = &x;
  printf("The address is: %p, the value is %d\n", ptr, *ptr);

  char buf[64];
  assert(printf("%d", 1) == 1);
  assert(printf("1") == 1);
  assert(snprintf(buf, 1, "1") == 1);
  assert(snprintf(buf, 3, "123") == 3);
}

void test_format_print() { test_sprintf(); }
