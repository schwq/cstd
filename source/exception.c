#include "../include/exception.h"

void initialize_stack_exceptions() {
  stack_exceptions.current_level = 0;
  stack_exceptions.exceptions = initialize_list(sizeof(struct EXCEPTION_T));
}
void end_statck_exceptions() {
  free_list(stack_exceptions.exceptions, free_exception);
}

exception_t *make_exception(const char *what) {
  exception_t *exc = malloc(sizeof(struct EXCEPTION_T));
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

extern inline void throw_exc(exception_t *exc) {}
extern inline void catch_exc() {}
