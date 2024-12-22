#include "../include/matrix.h"
#include <math.h>

fvec_t *new_fvec_body(size_t dim, pFloat v) {
  fvec_t *vec = (fvec_t *)MALLOC(sizeof(fvec_t));
  vec->mat = new_mat_body(1, dim, v);
  vec->dim = dim;
  return vec;
}

fvec_t *new_fvec_from_p(size_t dim, pFloat *data, void (*fun_free)(void *)) {
  fvec_t *vec = (fvec_t *)MALLOC(sizeof(fvec_t));
  vec->mat = new_mat_body(1, dim, 0);
  memmove(vec->mat->values[0], data, sizeof(pFloat) * dim);
  vec->dim = dim;
  if (fun_free)
    fun_FREE(data);
  return vec;
}

fvec_t *new_fvec_ep(size_t dim) { return new_fvec_body(dim, 0); }

fvec_t *new_fvec_ct(size_t dim, pFloat v) { return new_fvec_body(dim, v); }

fvec_t *new_fvec_rand(size_t dim, pFloat a, pFloat b) {
  fvec_t *vec = new_fvec_body(dim, 0);
  for (uint x = 0; x < dim; x++) {
    vec->mat->values[0][x] = gen_randf_range(a, b);
  }
  return vec;
}

fmatrix_t *new_mat_body(size_t r, size_t c, pFloat v) {

  fmatrix_t *mat = (fmatrix_t *)MALLOC(sizeof(fmatrix_t));

  if (!mat)
    return (fmatrix_t *)error_mat("Cannot allocate matrix!");

  mat->values = (pFloat **)MALLOC(r * sizeof(pFloat *));

  if (!mat->values)
    return (fmatrix_t *)error_mat("Cannot allocate matrix rows!");

  for (uint i = 0; i < r; i++) {

    mat->values[i] = (pFloat *)MALLOC(c * sizeof(pFloat));

    if (!mat->values[i])
      return (fmatrix_t *)error_mat("Cannot allocate matrix collumn!");

    for (uint j = 0; j < c; j++) {
      mat->values[i][j] = v;
    }
  }
  mat->cols = c;
  mat->rows = r;
  mat->size = c * r;
  return mat;
}

fmatrix_t *apply_op1_fmat(fmatrix_t *mat, pFloat (*fun)(pFloat)) {
  if (!mat)
    return (fmatrix_t *)error_mat(
        "Cannot apply single operation on nullptr matrix!");

  for (uint x = 0; x < mat->rows; x++) {
    for (uint y = 0; y < mat->cols; y++) {
      mat->values[x][y] = fun(mat->values[x][y]);
    }
  }
  return mat;
}

fmatrix_t *apply_op_cond_fmat(fmatrix_t *mat, pFloat (*fun)(pFloat),
                              bool (*cond)(pFloat)) {
  if (!mat)
    return (fmatrix_t *)error_mat(
        "Cannot apply single operation on nullptr matrix!");

  for (uint x = 0; x < mat->rows; x++) {
    for (uint y = 0; y < mat->cols; y++) {
      if (cond(mat->values[x][y]))
        mat->values[x][y] = fun(mat->values[x][y]);
    }
  }
  return mat;
}

fmatrix_t *mul_fmat(fmatrix_t *m1, fmatrix_t *m2) {
  if (!m1 || !m2)
    return (fmatrix_t *)error_mat("Cannot multiply nullptr matrices!");

  if (m1->cols != m2->rows)
    return (fmatrix_t *)error_mat(
        "Cannot multiply incompatible matrices dimensions!");

  fmatrix_t *mat = new_mat_body(m1->rows, m2->cols, 0);

  for (uint i = 0; i < m1->rows; i++) {
    for (uint j = 0; j < m2->cols; j++) {
      for (uint k = 0; k < m2->rows; k++) {
        mat->values[i][j] += m1->values[i][k] * m2->values[k][j];
      }
    }
  }
  return mat;
}

void mul_fmat_to(fmatrix_t *to, fmatrix_t *from) {
  if (!to || !from)
    return (void)error_mat("Cannot multiply nullptr matrices!");

  if (to->cols != from->rows)
    return (void)error_mat("Cannot multiply incompatible matrices dimensions!");

  fmatrix_t *tmp = fmat_clone(to);

  free_fmat(to);

  to = new_mat_body(tmp->rows, from->cols, 0);

  for (uint i = 0; i < tmp->rows; i++) {
    for (uint j = 0; j < from->cols; j++) {
      for (uint k = 0; k < from->rows; k++) {
        to->values[i][j] += tmp->values[i][k] * from->values[k][j];
      }
    }
  }
  free_fmat(tmp);
}

fmatrix_t *transpose_fmat(fmatrix_t *mat) {
  if (!mat)
    return (fmatrix_t *)error_mat("Given matrix to transpose is nullptr!");

  fmatrix_t *m = new_mat_body(mat->cols, mat->rows, 0);
  if (!m)
    return (fmatrix_t *)error_mat("Cannot allocate matrix to transpose!");

  for (uint x = 0; x < m->rows; x++) {
    for (uint y = 0; y < m->cols; y++) {
      m->values[x][y] = mat->values[y][x];
    }
  }
  return m;
}

fmatrix_t *fmat_clone(fmatrix_t *mat) {

  if (!mat)
    return (fmatrix_t *)error_mat("Matrix passed to clone is nullptr!");

  fmatrix_t *m = new_mat_body(mat->rows, mat->cols, 0);

  if (!m)
    return (fmatrix_t *)error_mat("Cannot create new matrix to clone!");

  for (uint x = 0; x < m->rows; x++) {
    for (uint y = 0; y < m->cols; y++) {
      m->values[x][y] = mat->values[x][y];
    }
  }
  return m;
}

void print_fmat(fmatrix_t *mat) {

  if (!mat)
    return (void)error_mat("Cannot print nullptr matrix!");

  if (is_fmat_vector(mat))
    return print_fvec(mat);

  uint x = 0, y = 0;
  printf("Matrix %dx%d:\n[", mat->rows, mat->cols);
  for (x = 0; x < mat->rows; x++) {
    printf("[");
    for (y = 0; y < mat->cols; y++) {
      printf("%.1lf%s", mat->values[x][y], (y != mat->cols - 1) ? ", " : "");
    }
    printf("]");
    (x != mat->rows - 1) ? printf("\n") : printf("");
  }
  printf("]\n");
}
void print_fvec(fmatrix_t *mat) {
  printf("Vector of %d dimensions: [", mat->cols);
  for (uint x = 0; x < mat->cols; x++) {
    printf("%.1lf%s", mat->values[0][x], (x != mat->cols - 1) ? ", " : "");
  }
  printf("]\n");
}

fmatrix_t *new_fmat_ct(const pFloat v, size_t r, size_t c) {
  return new_mat_body(r, c, v);
}

fmatrix_t *new_fmat_id(size_t size) {
  fmatrix_t *mat = new_mat_body(size, size, 0);

  if (!mat)
    return (fmatrix_t *)error_mat("Cannot allocate matrix identity!");

  for (uint x = 0; x < size; x++) {
    mat->values[x][x] = 1;
  }
  return mat;
}

fmatrix_t *set_fmat(fmatrix_t *mat, size_t r, size_t c, pFloat v) {
  if (!mat)
    (fmatrix_t *)error_mat("Given matrix to set value is nullptr!");

  if (mat->rows <= r || mat->cols <= c)
    (fmatrix_t *)error_mat("Given row or collumn is out of range of matrix!");

  mat->values[r][c] = v;
  return mat;
}

fmatrix_t *new_fmat_ep(size_t r, size_t c) { return new_mat_body(r, c, 0); }

pFloat get_fmat(fmatrix_t *mat, size_t r, size_t c) {
  if (mat->rows <= r || mat->cols <= c)
    return -1;

  return mat->values[r][c];
}

static pFloat gen_randf_range(pFloat a, pFloat b) {
  pFloat random = (pFloat)rand() / RAND_MAX;
  return a + random * (b - a);
}

fmatrix_t *new_fmat_rand(size_t r, size_t c, pFloat a, pFloat b) {
  fmatrix_t *mat = new_mat_body(r, c, 0);
  for (uint x = 0; x < mat->rows; x++) {
    for (uint y = 0; y < mat->cols; y++) {
      mat->values[x][y] = gen_randf_range(a, b);
    }
  }
  return mat;
}

fmatrix_t *negative_fmat(fmatrix_t *mat) { return scalar_fmat(mat, -1); }

fmatrix_t *add_fmat(fmatrix_t *m1, fmatrix_t *m2) {
  if (!m1 || !m2)
    return (fmatrix_t *)error_mat("Cannot add matrix, is nullptr!");

  if (m1->rows != m2->rows || m1->cols != m2->cols)
    return (fmatrix_t *)error_mat("Rows or columns of matrices dont match up!");

  fmatrix_t *mat = new_mat_body(m1->rows, m1->cols, 0);

  if (!mat)
    return (fmatrix_t *)error_mat("Cannot allocate matrix to adition");

  for (uint x = 0; x < m1->rows; x++) {
    for (uint y = 0; y < m1->cols; y++) {
      mat->values[x][y] = m1->values[x][y] + m2->values[x][y];
    }
  }
  return mat;
}

fmatrix_t *scalar_fmat(fmatrix_t *mat, pFloat v) {
  if (!mat)
    return (fmatrix_t *)error_mat("Cannot scalar a nullptr matrix");

  for (uint x = 0; x < mat->rows; x++) {
    for (uint y = 0; y < mat->cols; y++) {
      mat->values[x][y] *= v;
    }
  }
  return mat;
}

void add_fmat_to(fmatrix_t *to, fmatrix_t *from) {
  if (!to || !from)
    return (fmatrix_t *)error_mat("Cannot add matrix, is nullptr!");

  if (to->rows != from->rows || to->cols != from->cols)
    return (fmatrix_t *)error_mat("Rows or columns of matrices dont match up!");

  for (uint x = 0; x < to->rows; x++) {
    for (uint y = 0; y < to->cols; y++) {
      to->values[x][y] += from->values[x][y];
    }
  }
}

fmatrix_t *sub_fmat(fmatrix_t *m1, fmatrix_t *m2) {
  if (!m1 || !m2)
    return (fmatrix_t *)error_mat("Cannot add matrix, is nullptr!");

  if (m1->rows != m2->rows || m1->cols != m2->cols)
    return (fmatrix_t *)error_mat("Rows or columns of matrices dont match up!");

  fmatrix_t *mat = new_mat_body(m1->rows, m1->cols, 0);

  if (!mat)
    return (fmatrix_t *)error_mat("Cannot allocate matrix to adition");

  for (uint x = 0; x < m1->rows; x++) {
    for (uint y = 0; y < m1->cols; y++) {
      mat->values[x][y] = m1->values[x][y] - m2->values[x][y];
    }
  }
  return mat;
}

void sub_fmat_to(fmatrix_t *to, fmatrix_t *from) {
  if (!to || !from)
    return (fmatrix_t *)error_mat("Cannot add matrix, is nullptr!");

  if (to->rows != from->rows || to->cols != from->cols)
    return (fmatrix_t *)error_mat("Rows or columns of matrices dont match up!");

  for (uint x = 0; x < to->rows; x++) {
    for (uint y = 0; y < to->cols; y++) {
      to->values[x][y] -= from->values[x][y];
    }
  }
}

void free_fmat(fmatrix_t *mat) {
  for (uint x = 0; x < mat->rows; x++) {
    FREE(mat->values[x]);
  }
  FREE(mat->values);
  FREE(mat);
}

bool is_fmat_equal(fmatrix_t *m1, fmatrix_t *m2) {
  if (!m1 || !m2)
    return false;
  if (m1->rows != m2->rows || m1->cols != m2->cols)
    return false;
  for (uint x = 0; x < m1->rows; x++) {
    for (uint y = 0; y < m1->cols; y++) {
      if (m1->values[x][y] != m2->values[x][y])
        return false;
    }
  }
  return true;
}

bool is_fmat_vector(fmatrix_t *mat) {
  if (!mat)
    return false;

  return mat->cols == 1 || mat->rows == 1;
}

pFloat det_fmat_2d(fmatrix_t *mat) {
  if (!mat)
    return -1;
  if (mat->cols != 2 || mat->rows != 2)
    return -1;
  return mat->values[0][0] * mat->values[1][1] -
         mat->values[0][1] * mat->values[0][1];
}

pFloat det_fmat_3d(fmatrix_t *mat) {
  if (!mat)
    return -1;
  if (mat->cols != 3 || mat->rows != 3)
    return -1;

  pFloat **v = mat->values;
  const pFloat a = v[0][0] * v[1][1] * v[2][2];
  const pFloat b = v[0][1] * v[1][2] * v[2][0];
  const pFloat c = v[0][2] * v[1][0] * v[2][1];
  const pFloat d = v[0][2] * v[1][1] * v[2][0];
  const pFloat e = v[0][1] * v[1][0] * v[2][2];
  const pFloat f = v[0][0] * v[1][2] * v[2][1];
  return a + b + c - d - e - f;
}

void *error_mat(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  return nullptr;
}

pFloat fvec_lenght(fvec_t *vec) {
  pFloat result = 0;
  for (uint x = 0; x < vec->dim; x++) {
    result += pow(vec->mat->values[0][x], 2);
  }
  return sqrt(result);
}

void free_fvec(fvec_t *vec) {
  free_fmat(vec->mat);
  FREE(vec);
}