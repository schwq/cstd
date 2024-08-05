#ifndef _C_MAP_H_
#define _C_MAP_H_

#include "dynamic_list.h"
#include "smart_ptr.h"

// C smart pointer pair type
typedef struct CSPAIR_T {

  smart_ptr_t *first;
  smart_ptr_t *second;

} cspair_t;

cspair_t *make_smart_pair(smart_ptr_t *first, smart_ptr_t *second);
void free_smart_pair(cspair_t *pair);

typedef struct CMAP_T {

  dynamic_list_t *elements;

} cmap_t;

cmap_t *make_map();
void free_map(cmap_t *map);

cspair_t *append_map(cmap_t *map, cspair_t *pair);
smart_ptr_t *get_map_by_key(cmap_t *map, type_data_t *data);

#endif