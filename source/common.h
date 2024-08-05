#ifndef _COMMON_H_
#define _COMMON_H_

#include <malloc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CSTD_DEBUG_ENABLED

#ifdef _CSTD_DEBUG_ENABLED
#define fdebug(...)                                                            \
  fprintf(stderr, "(DEBUG) -> ");                                              \
  fprintf(stderr, __VA_ARGS__);
#else
#define fdebug(...)
#endif

#endif