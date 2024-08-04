#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

#include "dynamic_list.h"

typedef struct VECTOR3_T {
  float x;
  float y;
  float z;
} vec3_t;

vec3_t *create_vec3(float x, float y, float z);
void print_vec3(vec3_t *vec);

void benchmark_random_append_list_vec3(dynamic_list_t *list, size_t times);

#endif