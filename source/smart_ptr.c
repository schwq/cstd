#include "../include/smart_ptr.h"
#include "../include/exception.h"

type_data_t *make_type_data(void *data, size_t size) {
  type_data_t *type = (type_data_t *)malloc(sizeof(struct TYPE_DATA_T));
  type->pointer = malloc(size);
  memcpy(type->pointer, data, size);
  type->size = size;
  fdebug("Created type_data_t! Data 0x%x, Size %u\n", data, size);
  return type;
}

void free_type_data(type_data_t *data) {
  fdebug("Free type_data_t! Data 0x%x, Size %u\n", data->pointer, data->size);
  free(data->pointer);
  free(data);
}

bool cmp_type_data(type_data_t *d1, type_data_t *d2) {
  return (memcmp(d1->pointer, d2->pointer, d1->size) == 0);
}

void initialize_stack() {
  fdebug("================[STACK INIT]================\n");
  smart_stack.smart_pointers = initialize_list(sizeof(struct SMART_PTR_T));
  smart_stack.current_level = 0;
  initialize_stack_exceptions();
}

void end_stack() {
  free_list(smart_stack.smart_pointers, kill_smart);
  end_statck_exceptions();
  fdebug("================[STACK END]================\n");
}

smart_ptr_t *smart_alloc(size_t size, void *data) {
  smart_ptr_t *pointer = malloc(sizeof(struct SMART_PTR_T));
  pointer->data = make_type_data(data, size);
  pointer->references = 1;
  pointer->level_created = smart_stack.current_level;
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
    smart_free(ptr);
    return SMART_PTR_FREE;
  }
  return SMART_PTR_HOLD;
}

void kill_smart(smart_ptr_t *ptr) {
  fdebug("Killed smart_ptr_t! Pointer 0x%x\n", ptr->data->pointer);
  free_type_data(ptr->data);
  free(ptr);
}

void smart_free(smart_ptr_t *ptr) {
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
    // TODO: review this
    if (pointer->level_created >= smart_stack.current_level) {
      if (smart_release(pointer) == SMART_PTR_FREE) {
        for (size_t index = pointer_index;
             index < smart_stack.smart_pointers->lenght; index++) {
          ((smart_ptr_t *)smart_stack.smart_pointers->pointer[index])
              ->index_stack -= 1;
        }
      }
    }
  }

  if (smart_stack.current_level != 0)
    smart_stack.current_level -= 1;
}