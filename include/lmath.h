#ifndef _MATH_H_
#define _MATH_H_

#include "common.h"
#include "matrix.h"

typedef struct LINE_T {
  fvec_t *origin;
  fvec_t *end;
  size_t dim;
} line_t;

typedef struct COORDS_2D_T {
  pFloat x, y;
} coords_2d_t;

typedef struct COORDS_3D_T {
  pFloat x, y, z;
} coords_3d_t;

typedef struct COORDS_4D_T {
  pFloat x, y, z, w;
} coords_4d_t;

static line_t *new_line_body(size_t dim, fvec_t *origin, fvec_t *end);

line_t *new_line_2d(coords_2d_t origin, coords_2d_t end);

line_t *new_line_3d(coords_3d_t origin, coords_3d_t end);

line_t *new_line_4d(coords_4d_t origin, coords_4d_t end);

line_t *new_line(size_t dim, fvec_t *origin, fvec_t *end);

line_t *new_line_from_p(size_t dim, pFloat *origin, pFloat *end);

fvec_t *new_fvec_from_line(line_t *line);

void free_line(line_t *line);

#endif