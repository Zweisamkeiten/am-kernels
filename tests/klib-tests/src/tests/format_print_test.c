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
}

void test_format_print() { test_sprintf(); }
