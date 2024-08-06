#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_

#include "common.h"
#include "dynamic_list.h"

#define SMART_PTR_FREE 0
#define SMART_PTR_HOLD 1

typedef struct TYPE_DATA_T {
  void *pointer;
  size_t size;
} type_data_t;

type_data_t *make_type_data(void *data, size_t size);
void free_type_data(type_data_t *data);

bool cmp_type_data(type_data_t *d1, type_data_t *d2);

typedef struct SMART_PTR_T {

  size_t references;
  type_data_t *data;
  size_t index_stack;
  size_t level_created;
} smart_ptr_t;

static void kill_smart(smart_ptr_t *ptr);

typedef struct SMART_PTR_STACK {
  dynamic_list_t *smart_pointers;
  size_t current_level;
} smart_ptr_stack;

static smart_ptr_stack smart_stack;

void initialize_stack();
void end_stack();

void push_stack();
void pop_stack();

smart_ptr_t *smart_alloc(size_t size, void *data);

smart_ptr_t *smart_grab(smart_ptr_t *ptr);

void *smart_get(smart_ptr_t *ptr);

/**
 * Can return SMART_PTR_FREE if the reference counter was 1 and the pointer was
 * killed or can return SMART_PTR_HOLD if the reference counter was bigger that
 * 1, and there is something holding the pointer!
 */
int smart_release(smart_ptr_t *ptr);

void smart_free(smart_ptr_t *ptr);

#endif