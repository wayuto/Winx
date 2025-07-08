#include "cmos.h"

#include "global.h"
#include "math.h"
#include "print.h"
#include "stdio.h"

struct tm time;

struct timespec times;

static bool is_leap_year(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int days_in_month(int month, int year) {
  if (month == 2) {
    return is_leap_year(year) ? 29 : 28;
  } else if (month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;
  } else {
    return 31;
  }
}

uint64_t datetime_to_timestamp(struct tm *tm) {
  if (tm == NULL) {
    return 0;
  }

  uint64_t total_seconds = 0;

  for (uint32_t y = 1970; y < tm->tm_year + 1900; y++) {
    total_seconds += is_leap_year(y) ? 31622400 : 31536000;
  }

  for (uint32_t m = 0; m < tm->tm_mon; m++) {
    total_seconds += days_in_month(m + 1, tm->tm_year + 1900) * 24 * 60 * 60;
  }

  total_seconds += (tm->tm_mday - 1) * 24 * 60 * 60;

  total_seconds += tm->tm_hour * 60 * 60 + tm->tm_min * 60 + tm->tm_sec;

  return total_seconds;
}

void timestamp_to_datetime(uint64_t timestamp, struct tm *datetime) {
  if (datetime == NULL) {
    return;
  }

  uint32_t days = (uint32_t)divide_u64_u32_no_mod(timestamp, (24 * 3600));
  uint32_t year = 1970;

  while (days >=
         365u + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
    days -= 365u + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    year++;
  }

  datetime->tm_year = year;

  uint32_t month = 1;
  uint32_t days_in_mon;
  while (days >= (days_in_mon = days_in_month(month, year))) {
    days -= days_in_mon;
    month++;
  }

  datetime->tm_year = year - 1900;
  datetime->tm_mon = month - 1;
  datetime->tm_mday = days + 1;

  datetime->tm_hour =
      divide_u64_u32_no_mod(mod_u64_u32(timestamp, (24 * 3600)), 3600);
  datetime->tm_min = divide_u64_u32_no_mod(mod_u64_u32(timestamp, (3600)), 60);
  datetime->tm_sec = mod_u64_u32(timestamp, 60);
}

static unsigned int fake_time = 0;
unsigned int get_time(void) { return ++fake_time; }

static uint8_t cmos_read(uint8_t addr) {
  outb(0x70, (0x80 | (addr)));
  return inb(0x71);
};

void time_init(void) {
  put_str("time_init begin!\n");
  do {
    time.tm_sec = cmos_read(0);
    time.tm_min = cmos_read(2);
    time.tm_hour = cmos_read(4);
    time.tm_mday = cmos_read(7);
    time.tm_mon = cmos_read(8);
    time.tm_year = cmos_read(9);
  } while (time.tm_sec != cmos_read(0));
  BCD_TO_BIN(time.tm_sec);
  BCD_TO_BIN(time.tm_min);
  BCD_TO_BIN(time.tm_hour);
  BCD_TO_BIN(time.tm_mday);
  BCD_TO_BIN(time.tm_mon);
  BCD_TO_BIN(time.tm_year);

  time.tm_year += 100;
  time.tm_mon -= 1;

  times.tv_sec = datetime_to_timestamp(&time);
  times.tv_msec = times.tv_sec;

  put_str("time_init end!\n");
}
