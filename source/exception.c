#include "../include/exception.h"

void initialize_stack_exceptions() {
  stack_exceptions.current_level = 0;
  stack_exceptions.exceptions = initialize_list(sizeof(struct EXCEPTION_T));
}
void end_statck_exceptions() {
  free_list(stack_exceptions.exceptions, free_exception);
}

exception_t *make_exception(const char *what) {
  exception_t *exc = MALLOC(sizeof(struct EXCEPTION_T));
  exc->catched = false;
  exc->level_created = stack_exceptions.current_level;
  exc->stack_index = stack_exceptions.exceptions->lenght;
  append_list(stack_exceptions.exceptions, exc);
  exc->what = cinti_cstring_t(what);
  return exc;
}

void free_exception(exception_t *exc) {
  free_cstring_t(exc->what);
  remove_list(stack_exceptions.exceptions, exc->stack_index, nullptr);
}

void try_exc() { clean_list(stack_exceptions.exceptions, free_exception); }

inline void throw_exc(exception_t *exc) {
  stack_exceptions.current_exc = exc;
  stack_exceptions.current_level = smart_stack.current_level;
}

inline void catch_exc() {
  if (stack_exceptions.current_exc != nullptr) {
    fprintf(stderr, "[EXCEPTION] -> ");
    fprintf(stderr, stack_exceptions.current_exc->what->allocator->pointer);
    stack_exceptions.current_exc->catched = true;
    append_list(stack_exceptions.exceptions, stack_exceptions.current_exc);
    stack_exceptions.current_exc = nullptr;
  }
}
