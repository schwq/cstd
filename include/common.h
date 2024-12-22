#ifndef _COMMON_H_
#define _COMMON_H_

#include <malloc.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#if defined(_WIN64)
#define ENV_64BIT
#else
#define ENV_32BIT
#endif
#endif

#if __GNUCC__
#if __x86_64__ || __ppc64__
#define ENV_64BIT
#else
#define ENV_32BIT
#endif
#endif

#if !defined(ENV_32BIT) || !defined(ENV_64BIT)
#define ENV_64BIT
#endif

#if defined(ENV_64BIT)
typedef double pFloat;
#else
typedef float pFloat;
#endif

#define uint unsigned int

#define _CSTD_DEBUG_ENABLED

#ifdef _CSTD_DEBUG_ENABLED
#define fdebug(...)                                                            \
  fprintf(stderr, "(DEBUG) -> ");                                              \
  fprintf(stderr, __VA_ARGS__);
#else
#define fdebug(...)
#endif

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#endif