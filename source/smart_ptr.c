#include "../include/smart_ptr.h"
#include "../include/exception.h"

type_data_t *make_type_data(void *data, size_t size, void (*free_fp)(void *)) {
  type_data_t *type = (type_data_t *)MALLOC(sizeof(struct TYPE_DATA_T));
  type->pointer = copy_data_ptr(size, data);
  type->size = size;
  type->free_fp = free_fp;
  fdebug("Created type_data_t! Data 0x%x, Size %u\n", data, size);
  return type;
}

void free_type_data(type_data_t *data) {
  fdebug("Free type_data_t! Data 0x%x, Size %u\n", data->pointer, data->size);
  (data->free_fp) ? data->free_fp(data->pointer) : FREE(data->pointer);
  FREE(data);
}

bool cmp_type_data(type_data_t *d1, type_data_t *d2) {
  return (memcmp(d1->pointer, d2->pointer, MIN(d1->size, d2->size)) == 0);
}

void initialize_stack() {
  fdebug("================[STACK INIT]================\n");
  smart_stack.smart_pointers = initialize_list(sizeof(struct SMART_PTR_T));
  smart_stack.current_level = 0;
  initialize_stack_exceptions();
}

int end_stack(int code) {
  free_list(smart_stack.smart_pointers, kill_smart);
  end_statck_exceptions();
  fdebug("================[STACK END]================\n");
  return code;
}

smart_ptr_t *smart_alloc(size_t size, void *data, void (*free_fp)(void *)) {
  smart_ptr_t *pointer = MALLOC(sizeof(struct SMART_PTR_T));
  pointer->data = make_type_data(data, size, free_fp);
  pointer->references = 1;
  pointer->level_created = smart_stack.current_level;
  pointer->free_by_scope = false;
  if (smart_stack.smart_pointers)
    append_list(smart_stack.smart_pointers, pointer);
  pointer->index_stack = smart_stack.smart_pointers->lenght - 1;

  fdebug("Created smart_ptr_t Size: %i, Data: 0x%x, Level created %u, Index in "
         "stack %u\n",
         size, data, smart_stack.current_level, pointer->index_stack);
  return pointer;
}

smart_ptr_t *smart_alloc_f(size_t size, void *data, void (*free_fp)(void *)) {
  smart_ptr_t *pointer = smart_alloc_c(size, data, free_fp);
  (free_fp) ? free_fp(data) : FREE(data);
  return pointer;
}

smart_ptr_t *smart_alloc_c(size_t size, void *data, void (*free_fp)(void *)) {
  smart_ptr_t *pointer = MALLOC(sizeof(struct SMART_PTR_T));
  pointer->data = make_type_data(data, size, free_fp);
  pointer->references = 1;
  pointer->level_created = smart_stack.current_level;
  pointer->free_by_scope = false;
  if (smart_stack.smart_pointers)
    append_list(smart_stack.smart_pointers, pointer);
  pointer->index_stack = smart_stack.smart_pointers->lenght - 1;

  fdebug("Created smart_ptr_t Size: %i, Data: 0x%x, Level created %u, Index in "
         "stack %u\n",
         size, data, smart_stack.current_level, pointer->index_stack);
  return pointer;
}

smart_ptr_t *smart_grab(smart_ptr_t *ptr) {
  fdebug("Grabs smart_ptr_t! Pointer 0x%x, References %u\n", ptr->data->pointer,
         ptr->references);
  ptr->references += 1;
  return ptr;
}

void *smart_get(smart_ptr_t *ptr) {
  fdebug("Gets smart_ptr_t data! Pointer: 0x%x\n", ptr->data->pointer);
  return ptr->data->pointer;
}

int smart_release(smart_ptr_t *ptr) {
  ptr->references -= 1;
  fdebug("Released smart_ptr_t! Pointer 0x%x, References left %u\n",
         ptr->data->pointer, ptr->references);
  if (ptr->references == 0) {
    fdebug("References from smart_ptr_t has reached 0! Pointer 0x%x\n",
           ptr->data->pointer);
    smart_FREE(ptr);
    return SMART_PTR_FREE;
  }
  return SMART_PTR_HOLD;
}

void kill_smart(smart_ptr_t *ptr) {
  fdebug("Killed smart_ptr_t! Pointer 0x%x\n", ptr->data->pointer);
  free_type_data(ptr->data);
  FREE(ptr);
}

void smart_FREE(smart_ptr_t *ptr) {
  if (ptr->references != 0)
    fdebug("Freeing a smart_ptr with %u references left! Forgot to call "
           "smart_release?\n",
           ptr->references);
  remove_list(smart_stack.smart_pointers, ptr->index_stack, kill_smart);
}

void push_stack() { smart_stack.current_level += 1; }

void pop_stack() {
  for (size_t index = 0; index < smart_stack.smart_pointers->lenght; index++) {
    smart_ptr_t *pointer =
        ((smart_ptr_t *)smart_stack.smart_pointers->pointer[index]);
    size_t pointer_index = pointer->index_stack;
    if (pointer->level_created == smart_stack.current_level &&
        !pointer->free_by_scope) {
      if (smart_release(pointer) == SMART_PTR_FREE) {
        for (size_t index = pointer_index;
             index < smart_stack.smart_pointers->lenght; index++) {
          ((smart_ptr_t *)smart_stack.smart_pointers->pointer[index])
              ->index_stack -= 1;
        }
      }
      pointer->free_by_scope = true;
    }
  }

  if (smart_stack.current_level != 0)
    smart_stack.current_level -= 1;
}

void *copy_data_ptr(size_t size, void *data) {
  void *ptr = MALLOC(size);
  memcpy(ptr, data, size);
  return ptr;
}