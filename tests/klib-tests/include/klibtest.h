#include <stdint.h>
#include <klib.h>

#define N 32
#define LENGTH(arr)         (sizeof(arr) / sizeof((arr)[0]))

extern void (*entry)();
extern uint8_t data[];
void reset();
void check_seq(int l, int r, uint8_t val);
void check_eq(int l, int r, uint8_t val);

#define CASE(id, entry_, ...) \
  case id: { \
    void entry_(); \
    entry = entry_; \
    __VA_ARGS__; \
    entry_(); \
    break; \
  }
