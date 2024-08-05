#ifndef _DYNAMIC_LIST_H_
#define _DYNAMIC_LIST_H_

#include <malloc.h>

#include "common.h"

#define nullptr (void *)NULL

typedef struct OPTIONAL_T {

  void *data;
  bool has_value;

  void *userdata;
  bool has_userdata;

} optional_t;

void *optional_has_value_or(optional_t optional, void * or);
optional_t create_optional(bool has_value, void *data);
optional_t create_optional_ud(bool has_value, void *data, void *userdata);

typedef struct RANGE_T {
  void *begin;
  void *end;
  size_t lenght;
} range_t;

range_t initialize_range(void *begin, size_t lenght);
optional_t get_range(range_t *range, size_t index);
bool is_range_inside_range(range_t is_range, range_t inside_of);

enum allocation_type { e_alloc, e_dealloc };

typedef struct DYNAMIC_LIST_T {
  void **pointer;
  size_t lenght;
  size_t sizeofItem;
  range_t range;

  size_t allocatedSize;
  size_t reallocGap;

} dynamic_list_t;

dynamic_list_t *initialize_list(size_t sizeof_item);
dynamic_list_t *initialize_list_allocated(size_t sizeof_item,
                                          size_t initial_alloc);

void free_list(dynamic_list_t *list, void (*free_fp)(void *));

void *append_list(dynamic_list_t *list, void *item);

void remove_list(dynamic_list_t *list, size_t index, void (*free_fp)(void *));

static void shift_left_list(dynamic_list_t *list, size_t index);

static void realloc_list(dynamic_list_t *list, size_t size,
                         enum allocation_type type);

void print_contents_list(dynamic_list_t *list, void (*printf_fp)(void *));

bool is_empty(dynamic_list_t *list);

#endif
