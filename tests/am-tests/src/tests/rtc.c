#include <amtest.h>
#include <klib.h>
#include <limits.h>

static uint64_t uptime() { return io_read(AM_TIMER_UPTIME).us; }

void rtc_test() {
  // while(1) printf("us: %lu\n", uptime());
  // AM_TIMER_RTC_T rtc;
  // int sec = 1;
  while (1) {
    printf("before uptime: %lu\n", uptime());
    long int i = 0;
    while(i < 10000000) i++;
    printf("after uptime: %lu\n", uptime());
    // rtc = io_read(AM_TIMER_RTC);
    // printf("%d-%d-%d %02d:%02d:%02d GMT (", rtc.year, rtc.month, rtc.day, rtc.hour, rtc.minute, rtc.second);
    // if (sec == 1) {
    //   printf("%d second).\n", sec);
    // } else {
    //   printf("%d seconds).\n", sec);
    // }
    // sec ++;
  }
}
