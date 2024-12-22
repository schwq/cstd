#include <lmath.h>

static line_t *new_line_body(size_t dim, fvec_t *origin, fvec_t *end) {
  if (!origin || !end)
    return (line_t *)error_mat("Cannot create line from nullptr vectors!");

  if (origin->dim != dim || end->dim != dim)
    return (line_t *)error_mat("Origin vector of end vector have not the same "
                               "dimensions was the line!");

  line_t *line = (line_t *)MALLOC(sizeof(line_t));
  line->origin = origin;
  line->end = end;
  line->dim = dim;
  return line;
}

line_t *new_line_2d(coords_2d_t origin, coords_2d_t end) {
  return new_line_from_p(2, &origin, &end);
}

line_t *new_line_3d(coords_3d_t origin, coords_3d_t end) {
  return new_line_from_p(3, &origin, &end);
}

line_t *new_line_4d(coords_4d_t origin, coords_4d_t end) {
  return new_line_from_p(4, &origin, &end);
}

line_t *new_line(size_t dim, fvec_t *origin, fvec_t *end) {
  return new_line_body(dim, origin, end);
}

line_t *new_line_from_p(size_t dim, pFloat *origin, pFloat *end) {
  fvec_t *o = new_fvec_from_p(dim, origin, nullptr);
  fvec_t *e = new_fvec_from_p(dim, end, nullptr);
  return new_line(dim, o, e);
}

fvec_t *new_fvec_from_line(line_t *line) {
  pFloat *data = (pFloat *)MALLOC(sizeof(pFloat) * line->dim);
  for (uint x = 0; x < line->dim; x++) {
    pFloat result =
        line->end->mat->values[0][x] - line->origin->mat->values[0][x];
    memmove(data + x, &result, sizeof(pFloat));
  }
  return new_fvec_from_p(line->dim, data, free);
}

void free_line(line_t *line) {
  free_fvec(line->end);
  free_fvec(line->origin);
  FREE(line);
}