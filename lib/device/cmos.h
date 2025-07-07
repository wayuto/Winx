#ifndef __DEVICE_CMOS_H
#define __DEVICE_CMOS_H

#include "io.h"
#include "stdint.h"

#define BCD_TO_BIN(val) ((val) = ((val) & 15) + ((val) >> 4) * 10)

struct tm {
    uint32_t tm_sec;
    uint32_t tm_min;
    uint32_t tm_hour;
    uint32_t tm_mday;
    uint32_t tm_mon;
    uint32_t tm_year;
} __attribute__((packed));

struct timespec {
    uint64_t tv_sec;
    uint64_t tv_msec;
};

extern struct tm time;

extern struct timespec times;

void time_init(void);

uint64_t datetime_to_timestamp(struct tm *tm);

void timestamp_to_datetime(uint64_t timestamp, struct tm *datetime);

unsigned int get_time(void);

#endif
