#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "common.h"
#include "memory.h"

/**
 * @brief Matrix of floating points values
 */
typedef struct FMATRIX_T {

  pFloat **values;
  size_t size, rows, cols;

} fmatrix_t;

/**
 * @brief Vector of floating points values
 */
typedef struct FVEC_T {

  fmatrix_t *mat;
  size_t dim;

} fvec_t;

/**
 * @brief Creates a new vector of dim dimensions and all values with the v
 * constant
 */
static fvec_t *new_fvec_body(size_t dim, pFloat v);

/**
 * @brief Creates a new vector of dim dimensions and all values set to 0
 */
fvec_t *new_fvec_ep(size_t dim);

/**
 * @brief Creates a new vector of dim dimensions and all values are set to v
 * value
 */
fvec_t *new_fvec_ct(size_t dim, pFloat v);

/**
 * @brief Creates a new vector of dim dimensions and moves the mem of data to
 * the matrix!
 */
fvec_t *new_fvec_from_p(size_t dim, pFloat *data, void (*fun_free)(void *));

/**
 * @brief Creates a new vector of dim dimensions and all values are random
 * numbers between [a,b]
 */
fvec_t *new_fvec_rand(size_t dim, pFloat a, pFloat b);

/**
 * @brief Gives back the lenght of vector
 */

pFloat fvec_lenght(fvec_t *vec);

/**
 * @brief Generates a random number between [a,b]
 */
static pFloat gen_randf_range(pFloat a, pFloat b);

/**
 * @brief Creates a new matrix of r rows and c columns with all values set to
 * the v constant
 */
static fmatrix_t *new_mat_body(size_t r, size_t c, pFloat v);

/**
 * @brief Creates a new matrix of r rows and c columns with all values set to
 * the v constant
 */
fmatrix_t *new_fmat_ct(const pFloat v, size_t r, size_t c);

/**
 * @brief Creates a new square matrix identity of rows and columns of size
 */
fmatrix_t *new_fmat_id(size_t size);

/**
 * @brief Creates a new matrix of r rows and c columns with all values set to 0
 */
fmatrix_t *new_fmat_ep(size_t r, size_t c);

/**
 * @brief Creates a new matrix of r rows and c columns with all random values
 * between [a,b]
 */
fmatrix_t *new_fmat_rand(size_t r, size_t c, pFloat a, pFloat b);

/**
 * @brief Get a value of a matrix in row r and column c
 */
pFloat get_fmat(fmatrix_t *mat, size_t r, size_t c);

/**
 * @brief Set a value in matrix in row r and column c
 */
fmatrix_t *set_fmat(fmatrix_t *mat, size_t r, size_t c, pFloat v);

/**
 * @brief Creates a new matrix and clones all information of mat to the new
 * created matrix
 */
fmatrix_t *fmat_clone(fmatrix_t *mat);

/**
 * @brief Prints all contents of matrix in a formatted way
 */
void print_fmat(fmatrix_t *mat);

/**
 * @brief Prints all contents of vector
 */
void print_fvec(fmatrix_t *mat);

/**
 * @brief Frees all pointers in matrix and the matrix itself
 */
void free_fmat(fmatrix_t *mat);

/**
 * @brief Frees a vector and all its contents
 */
void free_fvec(fvec_t *vec);

/**
 * @brief Negates all values in a matrix, its similar to scalar the matrix to -1
 */
fmatrix_t *negative_fmat(fmatrix_t *mat);

/**
 * Creates a new matrix with the contents being the adition of two matrices
 */
fmatrix_t *add_fmat(fmatrix_t *m1, fmatrix_t *m2);

/**
 * @brief Adds the from matrix to the to matrix
 */
void add_fmat_to(fmatrix_t *to, fmatrix_t *from);

/**
 * @brief Scalar the matrix to a given v value
 */
fmatrix_t *scalar_fmat(fmatrix_t *mat, pFloat v);

/**
 * @brief Creates a new matrix with the contents being the subtraction of two
 * matrices
 */
fmatrix_t *sub_fmat(fmatrix_t *m1, fmatrix_t *m2);

/**
 * @brief Substracts the from matrix to the to matrix
 */
void sub_fmat_to(fmatrix_t *to, fmatrix_t *from);

/**
 * @brief Transpose a given matrix
 */
fmatrix_t *transpose_fmat(fmatrix_t *mat);

/**
 * @brief Apply a given fun operation of all values of a given matrix mat
 */
fmatrix_t *apply_op1_fmat(fmatrix_t *mat, pFloat (*fun)(pFloat));

/**
 * @brief Applys a given fun operation in all values that is included in a given
 * condition cond
 */
fmatrix_t *apply_op_cond_fmat(fmatrix_t *mat, pFloat (*fun)(pFloat),
                              bool (*cond)(pFloat));

/**
 * @brief Check is two matrices are equal
 */
bool is_fmat_equal(fmatrix_t *m1, fmatrix_t *m2);

/**
 * @brief Checks if matrix is vector
 */
bool is_fmat_vector(fmatrix_t *mat);

/**
 * @brief Calculates the determinant of a 2x2 matrix
 */
pFloat det_fmat_2d(fmatrix_t *mat);

/**
 * @brief Calculates the determinant of a 3x3 matrix
 */
pFloat det_fmat_3d(fmatrix_t *mat);

/**
 * @brief Creates a new matrix with it contents being the multiplication of m1
 * and m2
 */
fmatrix_t *mul_fmat(fmatrix_t *m1, fmatrix_t *m2);

/**
 * @brief Applys multiplication in to matrix and from matrix and stores the
 * result in the to matrix
 */
void mul_fmat_to(fmatrix_t *to, fmatrix_t *from);

/**
 * @brief Logs a error and returns a nullptr
 */
void *error_mat(const char *fmt, ...);

#endif