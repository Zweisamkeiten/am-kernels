#include <klibtest.h>
// #include <assert.h>

void (*entry)() = NULL; // mp entry

uint8_t data[N];

void reset() {
  int i;
  for (i = 0; i < N; i ++) {
    data[i] = i + 1;
  }
}

// 检查[l,r)区间中的值是否依次为val, val + 1, val + 2...
void check_seq(int l, int r, uint8_t val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val + i - l);
  }
}

// 检查[l,r)区间中的值是否均为val
void check_eq(int l, int r, uint8_t val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val);
  }
}

static const char *tests[256] = {
  ['w'] = "write test",
  ['r'] = "read test",
  ['f'] = "format print test",
};

int main(const char *args) {
  switch (args[0]) {
    CASE('w', test_write);
    CASE('r', test_read);
    CASE('f', test_format_print);
    default:
      printf("Usage: make run mainargs=*\n");
      for (int ch = 0; ch < 256; ch++) {
        if (tests[ch]) {
          printf("  %c: %s\n", ch, tests[ch]);
        }
      }
  }
  return 0;
}
