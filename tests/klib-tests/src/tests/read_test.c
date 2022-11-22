#include <klibtest.h>

void test_memcmp() {
  void *zero_ptr1 = NULL;
  void *zero_ptr2 = NULL;
  assert(memcmp(zero_ptr1, zero_ptr2, 0) == 0);
  assert(memcmp("foo", "foobar", 2) == 0);
  assert(memcmp("foo", "foobar", 3) == 0);
  assert(memcmp("foo", "foobar", 4) != 0);
  assert(memcmp("foo", "bar", 1) != 0);
  assert(memcmp("foo", "bar", 3) != 0);

  /* Test less / equal / greater distinction.  */
  assert(memcmp("foo", "moo", 4) < 0);
  assert(memcmp("moo", "foo", 4) > 0);
  assert(memcmp("oomph", "oops", 3) < 0);
  assert(memcmp("oops", "oomph", 3) > 0);
  assert(memcmp("foo", "foobar", 4) < 0);
  assert(memcmp("foobar", "foo", 4) > 0);

  {
    char foo[21];
    char bar[21];
    int i;
    for (i = 0; i < 4; i++)
      {
        char *a = foo + i;
        char *b = bar + i;
        strcpy (a, "--------01111111");
        strcpy (b, "--------10000000");
        assert(memcmp(a, b, 16) < 0);
      }
  }
}

void test_strlen() {
  assert(strlen("foo") == 3);
  assert(strlen("foobar") == 6);
}

void test_read() {
  test_memcmp();
  test_strlen();
}
