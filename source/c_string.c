#include "../include/c_string.h"

allocator_char_t *init_allocator_char_t(const char *ch) {
  const size_t size = strlen(ch);
  allocator_char_t *allocator = malloc(sizeof(struct ALLOCATOR_CHAR_T));
  allocator->pointer = calloc(size, sizeof(char));
  allocator->lenght = size;
  memcpy(allocator->pointer, ch, size * sizeof(char));
  allocator->range = initialize_range(allocator->pointer, allocator->lenght);
  return allocator;
}

char *get_allocator_char_t(allocator_char_t *allocator) {
  return (const char *)allocator->pointer;
}

void free_allocator_char_t(allocator_char_t *pointer) {
  if (pointer->pointer)
    free(pointer->pointer);
  free(pointer);
}

cstring_t *cinti_cstring_t(const char *str) {
  cstring_t *string = malloc(sizeof(struct CSTRING_T));
  string->allocator = init_allocator_char_t(str);
  return string;
}

cstring_t *sinti_cstring_t(cstring_t *str) { return (void *)NULL; }

void free_cstring_t(cstring_t *str) {
  free_allocator_char_t(str->allocator);
  free(str);
}

void realloc_allocator_char_t(allocator_char_t *allocator, const char *ch) {
  if (!allocator)
    return (void)printf(
        "No allocator passed to the function realloc_allocator_char_t");

  const size_t size = strlen(ch);

  if (size == allocator->lenght) {
    memcpy(allocator->pointer, ch, size * sizeof(char));
    return;
  }
  free(allocator->pointer);
  allocator->pointer = malloc(size * sizeof(char));
  allocator->lenght = size;
  allocator->range = initialize_range(allocator->pointer, allocator->lenght);
  memcpy(allocator->pointer, ch, size * sizeof(char));
}

void equals_cstring_t(cstring_t *str, const char *equals) {
  realloc_allocator_char_t(str->allocator, equals);
}
