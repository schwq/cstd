#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include "c_string.h"
#include "dynamic_list.h"
#include "memory.h"
#include "smart_ptr.h"

typedef struct EXCEPTION_T {

  cstring_t *what; // What happens
  size_t stack_index;
  size_t level_created;
  bool catched;

} exception_t;

exception_t *make_exception(const char *what);
void free_exception(exception_t *exc);

typedef struct RUNTIME_EXCEPTION_T {
  exception_t base; // TODO:
} runtime_exception_t;

typedef struct STACK_EXCEPTIONS_T {

  dynamic_list_t *exceptions; // past exceptions
  exception_t *current_exc;   // current one
  size_t current_level;

} stack_exceptions_t;

static stack_exceptions_t stack_exceptions;
void initialize_stack_exceptions();
void end_statck_exceptions();

void try_exc();

extern void throw_exc(exception_t *exc);
extern void catch_exc();

#define throw_exception(name, exc)                                             \
  throw_exc(exc);                                                              \
  goto name;

#define catch_exception(name)                                                  \
  name:                                                                        \
  catch_exc();

#endif