#include <klibtest.h>

void test_memset() {
  int l, r;
  for (l = 0; l < N; l ++) {
    for (r = l + 1; r <= N; r ++) {
      reset();
      uint8_t val = (l + r) / 2;
      memset(data + l, val, r - l);
      check_seq(0, l, 1);
      check_eq(l, r, val);
      check_seq(r, N, r + 1);
    }
  }
}

void test_memcpy() {
  int l, r;
  for (l = 0; l < N; l ++) {
    for (r = l + 1; r <= N; r ++) {
      reset();
      srand(l);
      int src_length = rand() % (r - l);
      uint8_t src[src_length];
      for (int i = 0; i < src_length; i++) {
        src[i] = rand() % 256;
      }
      memcpy(data + l, src, src_length);
      check_seq(0, l, 1);
      for (int j = l; j < l + src_length; j++) {
        check_eq(j, j+1, src[j - l]);
      };
      check_seq(r, N, r + 1);
    }
  }
}

void test_memmove() {
  int l, r;
  for (l = 0; l < N; l ++) {
    for (r = l + 1; r <= N; r ++) {
      reset();
      srand(l);
      int src_length = rand() % (r - l);
      uint8_t src[src_length];
      for (int i = 0; i < src_length; i++) {
        src[i] = rand() % 256;
      }
      memcpy(data + l, src, src_length);
      check_seq(0, l, 1);
      for (int j = l; j < l + src_length; j++) {
        check_eq(j, j+1, src[j - l]);
      };
      check_seq(r, N, r + 1);
    }
  }
}

void test_write() {
  test_memset();
  test_memcpy();
  test_memmove();
}

