#include "../include/memory.h"

void *mem_alloc(size_t bytes) { return malloc(bytes); }

void *mem_free(void *pointer) {
  if (pointer) {
    free(pointer);
    pointer = nullptr;
  }
  return pointer;
}
