#ifndef _C_STRING_H_
#define _C_STRING_H_

#include "dynamic_list.h"

#define ALLOCATOR_TYPE_T(type, name)                                           \
  typedef struct {                                                             \
    type *pointer;                                                             \
    size_t lenght;                                                             \
    range_t range;                                                             \
  } name;

ALLOCATOR_TYPE_T(char, allocator_char_t);

allocator_char_t *init_allocator_char_t(const char *ch);
char *get_allocator_char_t(allocator_char_t *allocator);
void free_allocator_char_t(allocator_char_t *pointer);
static void realloc_allocator_char_t(allocator_char_t *allocator,
                                     const char *ch);

typedef struct CSTRING_T {

  allocator_char_t *allocator;

} cstring_t;

cstring_t *cinti_cstring_t(const char *str);
cstring_t *sinti_cstring_t(cstring_t *str);
void equals_cstring_t(cstring_t *str, const char *equals);
void equals_str_cstring(cstring_t *str, cstring_t *equals);
void free_cstring_t(cstring_t *str);
char *get_cstring(cstring_t *str);
bool is_cstring_empty(cstring_t *str);

void conc_cstring(cstring_t *str, const char *c1, const char *c2);

#endif