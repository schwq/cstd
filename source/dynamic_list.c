#include "dynamic_list.h"

void *has_value_or(optional_t optional, void * or) {
  if (optional.has_value) {
    if (or)
      free(or);
    return optional.data;
  }
  return or ;
}

optional_t create_optional(bool has_value, void *data) {
  return (optional_t){data, has_value, (void *)NULL, false};
}

optional_t create_optional_ud(bool has_value, void *data, void *userdata) {
  return (optional_t){data, has_value, userdata, true};
}

dynamic_list_t *initialize_list(size_t sizeof_item) {
  dynamic_list_t *_list =
      (dynamic_list_t *)malloc(sizeof(struct DYNAMIC_LIST_T));
  _list->lenght = 0;
  _list->pointer = calloc(10, sizeof(void *));
  _list->allocatedSize = 10;
  _list->sizeofItem = sizeof_item;
  _list->reallocGap = 10;
  _list->range = initialize_range(_list->pointer, _list->lenght);
  return _list;
}

dynamic_list_t *initialize_list_allocated(size_t sizeof_item,
                                          size_t initial_alloc) {
  dynamic_list_t *_list =
      (dynamic_list_t *)malloc(sizeof(struct DYNAMIC_LIST_T));
  _list->lenght = 0;
  _list->pointer = calloc(initial_alloc, sizeof(void *));
  _list->allocatedSize = initial_alloc;
  _list->sizeofItem = sizeof_item;
  _list->reallocGap = 10;
  _list->range = initialize_range(_list->pointer, _list->lenght);
  return _list;
}

range_t initialize_range(void *begin, size_t lenght) {
  return (range_t){.begin = begin, .end = begin + lenght, .lenght = lenght};
}

optional_t get_range(range_t *range, size_t index) {
  return (range->begin + index < range->end)
             ? create_optional(false,
                               (void *)NULL) // Review this implementation!
             : create_optional(true, (void *)(range->begin + index));
}

void *append_list(dynamic_list_t *list, void *item) {
  if (list->lenght == list->allocatedSize)
    realloc_list(list, list->allocatedSize + list->reallocGap, e_alloc);

  list->pointer[list->lenght] = item;
  list->lenght += 1;
  debug_list(list);
  list->range = initialize_range(list->pointer, list->lenght);
  return item;
}

void free_list(dynamic_list_t *list, void (*free_fp)(void *)) {
  for (size_t index = 0; index < list->lenght; index++) {
    (free_fp) ? free_fp(list->pointer[index]) : free(list->pointer[index]);
  }
  free(list->pointer);
}

void realloc_list(dynamic_list_t *list, size_t size,
                  enum allocation_type type) {

  list->pointer = realloc(list->pointer, size * sizeof(void *));

  if (!list->pointer)
    return (void)printf("Returned a null pointer from reallocation!");

  list->allocatedSize = size;

  (type == e_alloc) ? printf("Allocating %i\n", size)
                    : printf("Deallocating %i\n", size);
}

void debug_list(dynamic_list_t *list) {
  printf("Allocated size %i, Lenght %i, Realloc gap %i, Size in Kb %i\n",
         list->allocatedSize, list->lenght, list->reallocGap,
         list->lenght * list->sizeofItem / 1000);
}

void shift_left_list(dynamic_list_t *list, size_t index) {
  for (size_t i = index; i < list->lenght; i++)
    list->pointer[i] = list->pointer[i + 1];
}

void remove_list(dynamic_list_t *list, size_t index, void (*free_fp)(void *)) {
  if (index < 0 || index > list->lenght)
    return (void)printf("Out of range to remove from list");

  (free_fp) ? free_fp(list->pointer[index]) : free(list->pointer[index]);

  list->lenght -= 1;

  if (list->lenght < (list->allocatedSize - list->reallocGap))
    realloc_list(list, list->allocatedSize - list->reallocGap, e_dealloc);

  shift_left_list(list, index);
  debug_list(list);
  list->range = initialize_range(list->pointer, list->lenght);
}

void print_contents_list(dynamic_list_t *list, void (*printf_fp)(void *)) {
  if (!printf_fp)
    return (void)printf(
        "Not print function pointer was passed to the print_contents_list");

  for (size_t index = 0; index < list->lenght; index++) {
    printf_fp(list->pointer[index]);
  }
}

bool is_range_inside_range(range_t is_range, range_t inside_of) {
  return (is_range.begin >= inside_of.begin && is_range.end <= inside_of.end);
}