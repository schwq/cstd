#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#define nullptr (void *)NULL

static void *mem_alloc(size_t bytes);
static void *mem_free(void *pointer);

#define MALLOC(bytes) mem_alloc(bytes)
#define FREE(ptr) mem_free(ptr)

#endif