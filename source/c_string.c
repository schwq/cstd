#include "../include/c_string.h"

allocator_char_t *init_allocator_char_t(const char *ch) {
  const size_t size = strlen(ch);
  allocator_char_t *allocator = MALLOC(sizeof(allocator_char_t));
  allocator->pointer = (char *)MALLOC(size * sizeof(char));
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
    FREE(pointer->pointer);
  FREE(pointer);
}

cstring_t *cinti_cstring_t(const char *str) {
  cstring_t *string = MALLOC(sizeof(struct CSTRING_T));
  string->allocator = init_allocator_char_t(str);
  return string;
}

cstring_t *sinti_cstring_t(cstring_t *str) { return (void *)NULL; }

void free_cstring_t(cstring_t *str) {
  free_allocator_char_t(str->allocator);
  FREE(str);
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
  FREE(allocator->pointer);
  allocator->pointer = calloc(size, sizeof(char));
  allocator->lenght = size;
  allocator->range = initialize_range(allocator->pointer, allocator->lenght);
  memcpy(allocator->pointer, ch, size * sizeof(char));
}

void equals_cstring_t(cstring_t *str, const char *equals) {
  realloc_allocator_char_t(str->allocator, equals);
}

char *get_cstring(cstring_t *str) { return (char *)str->allocator->pointer; }

bool is_cstring_empty(cstring_t *str) { return (str->allocator->lenght == 0); }

void equals_str_cstring(cstring_t *str, cstring_t *equals) {
  const size_t size = strlen(get_cstring(equals));
  char *data = calloc(size, sizeof(char));
  memcpy(data, get_cstring(equals), size);
  realloc_allocator_char_t(str->allocator, data);
  FREE(data);
}

void conc_cstring(cstring_t *str, const char *c1, const char *c2) {
  const size_t size = strlen(c1) + strlen(c2);
  char *data = calloc(size, sizeof(char));
  memcpy(data, c1, strlen(c1));
  memcpy(data + strlen(c1), c2, strlen(c2));
  realloc_allocator_char_t(str->allocator, data);
  FREE(data);
}