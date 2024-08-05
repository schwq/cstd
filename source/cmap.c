#include "cmap.h"

cspair_t *make_smart_pair(smart_ptr_t *first, smart_ptr_t *second) {
  cspair_t *pair = malloc(sizeof(struct CSPAIR_T));
  pair->first = smart_grab(first);
  pair->second = smart_grab(second);
  return pair;
}

void free_smart_pair(cspair_t *pair) {
  smart_release(pair->first);
  smart_release(pair->second);
  free(pair);
}

cmap_t *make_map() {
  cmap_t *map = (cmap_t *)malloc(sizeof(struct CMAP_T));
  map->elements = initialize_list(sizeof(struct CSPAIR_T));
  return map;
}

void free_map(cmap_t *map) {
  free_list(map->elements, free_smart_pair);
  free(map);
}

cspair_t *append_map(cmap_t *map, cspair_t *pair) {
  return (cspair_t *)append_list(map->elements, pair);
}

smart_ptr_t *get_map_by_key(cmap_t *map, type_data_t *data) {
  for (size_t index = 0; index < map->elements->lenght; index++) {
    cspair_t *pair = map->elements->pointer[index];
    if (memcmp(pair->first->data->pointer, data->pointer,
               pair->first->data->size) == 0) {
      free_type_data(data);
      return pair->second;
    }
  }
  free_type_data(data);
  return (void *)0;
}