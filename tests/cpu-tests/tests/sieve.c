#include "trap.h"

typedef struct Setting {
  int size;
  unsigned long mlim, ref;
  uint32_t checksum;
} Setting;
Setting setting = {100, 1 * 1024, 0, 0x00000019};
static int N;

static int ans;
static uint32_t *primes;


static inline int get(int n) {
  return (primes[n >> 5] >> (n & 31)) & 1;
}

static inline void clear(int n) {
  primes[n >> 5] &= ~(1ul << (n & 31));
}

void bench_sieve_prepare() {
  N = setting.size;
  primes = (uint32_t*)malloc(N / 8 + 128);
  for (int i = 0; i <= N / 32; i ++) {
    primes[i] = 0xffffffff;
  }
}

void bench_sieve_run() {
  for (int i = 1; i <= N; i ++)
    if (!get(i)) return;
  for (int i = 2; i * i <= N; i ++) {
    if (get(i)) {
      for (int j = i + i; j <= N; j += i)
        clear(j);
    }
  }
  ans = 0;
  for (int i = 2; i <= N; i ++)
    if (get(i)) {
      ans ++;
    }
}

void bench_sieve_validate() {
  check(ans == setting.checksum);
}

int main() {
  bench_sieve_prepare();
  bench_sieve_run();
  bench_sieve_validate();
  return 0;
}
