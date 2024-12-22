#include "../include/benchmark.h"

vec3_t *create_vec3(float x, float y, float z) {
  vec3_t *vec = (vec3_t *)MALLOC(sizeof(struct VECTOR3_T));
  *vec = (vec3_t){.x = x, .y = y, .z = z};
  return vec;
}

void print_vec3(vec3_t *vec) {
  printf("Vector3 : [x: %.2f, y: %.2f, z: %.2f]\n", vec->x, vec->y, vec->z);
}

void benchmark_random_append_list_vec3(dynamic_list_t *list, size_t times) {
  for (size_t i = 0; i < times; i++) {
    append_list(list, create_vec3(rand(), rand(), rand()));
  }
}