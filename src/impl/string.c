#include "string.h"

#include "assert.h"
#include "global.h"

void memset(void *dst_, uint8_t value, uint32_t size) {
  assert(dst_ != NULL);
  uint8_t *dst = (uint8_t *)dst_;
  while (size-- > 0)
    *dst++ = value;
}

void memcpy(void *dst_, const void *src_, uint32_t size) {
  assert(dst_ != NULL && src_ != NULL);
  uint8_t *dst = dst_;
  const uint8_t *src = src_;
  while (size-- > 0)
    *dst++ = *src++;
}

int memcmp(const void *a_, const void *b_, uint32_t size) {
  const char *a = a_;
  const char *b = b_;
  assert(a != NULL || b != NULL);
  while (size-- > 0) {
    if (*a != *b) {
      return *a > *b ? 1 : -1;
    }
    a++;
    b++;
  }
  return 0;
}

char *strcpy(char *dst_, const char *src_) {
  assert(dst_ != NULL && src_ != NULL);
  char *r = dst_;
  while ((*dst_++ = *src_++))
    ;
  return r;
}

uint32_t strlen(const char *str) {
  assert(str != NULL);
  const char *p = str;
  while (*p++)
    ;
  return (p - str - 1);
}

int8_t strcmp(const char *a, const char *b) {
  assert(a != NULL && b != NULL);
  while (*a != 0 && *a == *b) {
    a++;
    b++;
  }

  return *a < *b ? -1 : *a > *b;
}

char *strchr(const char *str, const uint8_t ch) {
  assert(str != NULL);
  while (*str != 0) {
    if (*str == ch) {
      return (char *)str;
    }
    str++;
  }
  return NULL;
}

char *strrchr(const char *str, const uint8_t ch) {
  assert(str != NULL);
  const char *last_char = NULL;

  while (*str != 0) {
    if (*str == ch) {
      last_char = str;
    }
    str++;
  }
  return (char *)last_char;
}

char *strcat(char *dst_, const char *src_) {
  assert(dst_ != NULL && src_ != NULL);
  char *str = dst_;
  while (*str++)
    ;
  --str;
  while ((*str++ = *src_++))
    ;
  return dst_;
}

uint32_t strchrs(const char *str, uint8_t ch) {
  assert(str != NULL);
  uint32_t ch_cnt = 0;
  const char *p = str;
  while (*p != 0) {
    if (*p == ch) {
      ch_cnt++;
    }
    p++;
  }
  return ch_cnt;
}
