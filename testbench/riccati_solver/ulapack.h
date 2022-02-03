/**
 * @file ulapack.h
 * @brief Declarations of uLAPack matrix manipulation functions.
 *
 * @author Sargis Yonan
 * @date July 8, 2017
 *
 * @version 1.0.1
 **/

/**
 * @todo new line the return codes in dox for readability.
 */

/*
 * File header guards.
 */
#ifndef __ULAPACK_H__
#define __ULAPACK_H__

/*
 * Including standard integer header for uintX_t types.
 */
#include <stdint.h>

/*
 * uLAPack options header.
 */
#include "ulapack_options.h"

/*
 * uLAPack type definitions.
 */
#include "ulapack_type.h"

/*
 * If the print function is desired, stdio.h must be included for its definition
 * of the FILE type and fprintf.
 */
#ifdef ULAPACK_USE_PRINT
#include <stdio.h>
#endif

/**
 * @name ulapack_init
 *
 * Initializes uLAPack matrix object.
 *
 * @note If static memory allocation is used, then only the n_rows and n_cols
 *       members of the matrix structure are set. Otherwise, memory is allocated
 *       using the specified memory allocator call.
 * @note If dynamic memory allocation is used, a matrix object can not be 
 *       initialized more than one time. A static allocated object can.
 *
 * @param[in/out] matrix A pointer to a matrix object type.
 * @param n_rows The number of rows the matrix will have.
 * @param n_cols The number of columns the matrix will have.
 *
 * @return ulapack_success is returned upon successful modification and
 *         initialization of the matrix object.
 *
 *         ulapack_oom is returned if dynamic memory allocation is specified for 
 *         use, and the memory required for the object cannot be allocated. 
 *
 *         ulapack_invalid_argument is returned if a NULL pointer was passed in
 *         while using static memory allocation, or a non-NULL pointer was 
 *         passed in using dynamic memory allocation. 
 *
 *         ulapack_invalid_argument is returned when the rows and 
 *         columns passed in exceed the preset maximum values set in the uLAPack
 *         options.
 */
MatrixError_t ulapack_init(
#ifdef ULAPACK_USE_STATIC_ALLOC
    Matrix_t *matrix,
#else
    Matrix_t **matrix,
#endif
    const uint64_t n_rows, const uint64_t n_cols);

/**
 * @name ulapack_destroy
 * Free a dynamically allocated ULAPack matrix object, and reset its pointers.
 *
 * @note This function can only be called in dynamic memory allocation is
 *       specified by explicitly not defining static allocation.
 * @note The free function defined by the macro ULAPACK_FREE is used as the
 *       memory freeing call.
 *
 * @param[in] obj A pointer to the ULAPack matrix object to destroy.
 *
 * @return ULAPack success code ulapack_success is returned if the matrix object
 *         passed in was valid and freed successfully.
 *         ulapack_uninit_obj is returned if the matrix object is not
 *         initialized.
 */
#ifdef ULAPACK_USE_DYNAMIC_ALLOC
MatrixError_t ulapack_destroy(Matrix_t *obj);
#endif

/**
 * @name ulapack_print
 * Print a matrix or vector to a specified output stream.
 *
 * @note The preprocessor macro "ULAPACK_USE_PRINT" must be defined in order to
 *       use this function. This is done to avoid extraneously include stdio.h
 *       since fprintf() is the driving function of this print call.
 * @note The assumption that 
 *
 * @param matrix An initialize matrix object.
 * @param stream The output file stream to print to.
 *
 * @return uLAPack Success code.
 *         ulapack_success is returned if the matrix object passed in was valid.
 *         ulapack_uninit_obj is returned if the matrix was not initialized.
 */
#ifdef ULAPACK_USE_PRINT
MatrixError_t ulapack_print(const Matrix_t *const matrix);
#endif

/**
 * @name ulapack_edit_entry
 * Edit a single uLAPack matrix object's element entry.
 *
 * @param[in/out] matrix The matrix object to edit an entry within.
 * @param row The row to edit the matrix within.
 * @param col The column to edit the matrix within.
 * @param value The value to overwrite the matrix element at the coordinates
 *        (row, col).
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         entry edit.
 *         ulapack_uninit_obj is returned if the matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the coordinates to edit are
 *         out of the range of the initialized matrix dimensions.
 */
MatrixError_t ulapack_edit_entry(Matrix_t *const matrix,
                                 const uint64_t row, const uint64_t col,
                                 const MatrixEntry_t value);

/**
 * @name ulapack_get_entry
 * Get a single uLAPack matrix object's element entry.
 *
 * @param[in] matrix The matrix object to get an entry from.
 * @param row The row to edit the matrix within.
 * @param col The column to edit the matrix within.
 * @param[out] value The variable to copy the matrix value at coordinates
 *             (row,col) into. ulapack_success is passed back if the matrices 
 *             are equal. ulapack_error is returned if the matrices are not 
 *             equal.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         copy of the matrix element at coordinates (row, col).
 *         ulapack_uninit_obj is returned if the matrix object is not
 *         initialized.
 *         ulapack_invalid_argument is returned if value is NULL.
 *         ulapack_invalid_argument is returned if the coordinates to get from
 *         are out of the range of the initialized matrix dimensions.
 */
MatrixError_t ulapack_get_entry(const Matrix_t *const matrix,
                                const uint64_t row, const uint64_t col,
                                MatrixEntry_t *const value);

/**
 * @name ulapack_size
 * Get the dimensions of a matrix object.
 *
 * @param[in] matrix The matrix object to get dimensions of.
 * @param[out] rows The number of rows in the matrix object.
 * @param[out] cols The number of columns in the matrix object.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         copy of matrix dimensions into the references rows and cols.
 *         ulapack_uninit_obj is returned if the matrix object is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if rows or cols is NULL.
 */
MatrixError_t ulapack_size(const Matrix_t *const matrix,
                           uint64_t *const rows, uint64_t *const cols);

/**
 * @name ulapack_set
 * Set every entry in a matrix to a specified value.
 *
 * @param[in/out] matrix An initialized matrix object to modify.
 * @param value The value to set the matrix elements to.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         modification of the matrix object.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 */
MatrixError_t ulapack_set(Matrix_t *const matrix,
                          const MatrixEntry_t value);

/**
 * @name ulapack_is_equal
 * Check if the two matrix operands passed in are equal to each other (A == B).
 *
 * @param[in] A An initialized matrix object operand.
 * @param[in] B An initialized matrix object operand.
 * @param[out] is_equal The conditional that stores the equality (A == B). 
 *             ulapack_error if not equal, ulapack_success if equal.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         comparison between the two matrices.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the is_equal pointer is NULL.
 */
MatrixError_t ulapack_is_equal(const Matrix_t *const A,
                               const Matrix_t *const B,
                               MatrixError_t *const is_equal);

/**
 * @name ulapack_is_vector
 * Check if the input operand is a vector: 1xN or Nx1.
 *
 * @param[in] vector An initialized vector object operand.
 * @param[out] is_vector Pass back if the input is a vector. ulapack_success if
 *             the input is a vector, and ulapack_error if the input is not a
 *             vector.
 *
 * @return ULAPack success code ulapack_success is returned if the input vector
 *         is a initialized and checked for vector dimensions successful.
 *         ulapack_uninit_obj is returned if a vector object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the vector pointer is NULL.
 */
MatrixError_t ulapack_is_vector(const Matrix_t *const vector,
                                MatrixError_t *const is_vector);

/**
 * @name ulapack_eye
 * Set a matrix equal to the identity matrix of that dimension.
 *
 * @note The matrix must be square (n_rows == n_cols).
 *
 * @param[in/out] matrix An initialized matrix object.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         modification of the matrix object.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the matrix is not square.
 */
MatrixError_t ulapack_eye(Matrix_t *const matrix);

/**
 * @name ulapack_max_coeff
 * Find the maximum elements of a matrix or vector and pass the value back by reference.
 *
 * @param[in] matrix An initialized matrix object.
 * @param[out] result The resultant of the maximum of the elements of the matrix
 *             passed in.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         summation of the matrix object.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not
 *         initialized.
 *         ulapack_invalid_argument is returned if the result pointer is NULL.
 */
MatrixError_t ulapack_max_coeff(const Matrix_t *const matrix,
                                MatrixEntry_t *const result);

/**
 * @name ulapack_sum
 * Add the elements of a matrix or vector and pass the value back by reference.
 *
 * @param[in] matrix An initialized matrix object.
 * @param[out] result The resultant of the sum of the elements of the matrix 
 *             passed in.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         summation of the matrix object.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the result pointer is NULL.
 */
MatrixError_t ulapack_sum(const Matrix_t *const matrix,
                          MatrixEntry_t *const result);

/**
 * @name ulapack_add
 * Add two matrices or vectors together.
 *
 * @note the dimensions of the two matrices must be identical.
 * @note if static allocation is specified, the dimensions of the result matrix
 *       are modified to equal the dimensions of the operand matrices.
 *
 * @param[in] A An initialized operand matrix.
 * @param[in] B An initialized operand matrix.
 * @param[out] An initialized matrix object to store the result of A + B.
 *
 * @return Upon a successful summation ulapack_success is returned.
 *         ulapack_invalid_argument is returned if size(A) != size(B).
 *         ulapack_uninit_obj is returned if the operand objects passed in are 
 *         not initialized.
 *         ulapack_invalid_argument is returned if the result pointer is NULL.
 *         ulapack_invalid_argument is returned if dynamic allocation is 
 *         specified, and the dimensions of the result matrix do not equal that
 *         of the operands.
 */
MatrixError_t ulapack_add(const Matrix_t *const A,
                          const Matrix_t *const B,
                          Matrix_t *const result);

/**
 * @name ulapack_subtract
 * Take the difference between two matrices or vectors.
 *
 * @note the dimensions of the two matrices must be identical.
 * @note if static allocation is specified, the dimensions of the result matrix
 *       are modified to equal the dimensions of the operand matrices.
 *
 * @param[in] A An initialized operand matrix.
 * @param[in] B An initialized operand matrix.
 * @param[out] result An initialized matrix object to store the result of A - B.
 *
 * @return Upon a successful subtraction ulapack_success is returned.
 *         ulapack_invalid_argument is returned if size(A) != size(B).
 *         ulapack_uninit_obj is returned if the operand objects passed in are 
 *         not initialized.
 *         ulapack_invalid_argument is returned if the result pointer is NULL.
 *         ulapack_invalid_argument is returned if dynamic allocation is 
 *         specified, and the dimensions of the result matrix do not equal that
 *         of the operands.
 */
MatrixError_t ulapack_subtract(const Matrix_t *const A,
                               const Matrix_t *const B,
                               Matrix_t *const result);

/**
 * @name ulapack_scale
 * Multiply a matrix/vector by a scalar.
 *
 * @param[in] matrix An initialized operand matrix object to scale.
 * @param scalar A value to scale a matrix by.
 * @param[out] result The matrix to store the result of the scaling into.
 *
 * @note If static memory is specified, the dimensions of the result matrix are
 *       set to that of the operand matrix.
* @note If dynamic memory allocation is specified, the initialized dimensions 
 *       of the result matrix must be equal to that of the input matrix.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if dynamic allocation is 
 *         specified, and the dimensions of the result matrix do not equal that
 *         of the operands.
 */
MatrixError_t ulapack_scale(Matrix_t *const matrix,
                            const MatrixEntry_t scalar,
                            Matrix_t *const result);

/**
 * @name ulapack_norm
 * Take the Frobenius norm of a matrix or norm of a vector.
 *
 * @param[in] matrix An initialized matrix object to take the norm of.
 * @param[out] norm A value to scale a matrix by.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the norm pointer is NULL.
 */
MatrixError_t ulapack_norm(const Matrix_t *const matrix,
                           MatrixEntry_t *const norm);

/**
 * @name ulapack_trace
 * Take the trace of a matrix.
 *
 * @param[in] matrix An initialized matrix object to take the trace of.
 * @param[out] trace The trace output of the matrix.
 *
 * @return ULAPack success code. ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not
 *         initialized.
 *         ulapack_invalid_argument is returned if the trace pointer is NULL.
 */
MatrixError_t ulapack_trace(const Matrix_t *const matrix,
                            MatrixEntry_t *const trace);

/**
 * @name ulapack_dot
 * Take the dot product of two vectors.
 *
 * @param[in] vector_a The first vector in the dot product term.
 * @param[in] vector_b The second vector in the dot product term.
 * @param[out] dot The dot product of the vectors a and b.
 *
 * @return ULAPack success code. ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a vector object passed in is not
 *         initialized.
 *         ulapack_invalid_argument is returned if the dot pointer is NULL, or
 *         one or more of the inputs are not vectors, or if the dimensions of
 *         vectors are not identical.
 */
MatrixError_t ulapack_dot(const Matrix_t *const vector_a,
                          const Matrix_t *const vector_b,
                          MatrixEntry_t *const dot);

/**
 * @name ulapack_product
 * Multiply two matrices/vectors.
 *
 * @param[in] A An initialized matrix object operand.
 * @param[in] B An initialized matrix object operand.
 * @param[out] result An initialized matrix object to store the product of A and
 *             B. result = AB.
 *
 * @note For a matrix, A, with dimensions NxM, and a matrix B, with dimensions
 *       MxK, the product of the two matrices, AB, will have dimensions NxK.
 * @note If dynamic memory allocation is specified, the initialized dimensions 
 *       of the result matrix must be NxK.
 * @note If static memory allocation is specified, the dimensions of the result
 *       matrix are set to NxK.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the result pointer is NULL.
 *         ulapack_invalid_argument is returned if dynamic memory allocation is
 *         specified and the dimensions of the result matrix do not equal the
 *         required dimensions of the matrix/vector multiplication.
 */
MatrixError_t ulapack_product(const Matrix_t *const A,
                              const Matrix_t *const B,
                              Matrix_t *const result);

/**
 * @name ulapack_transpose
 * Take the transpose of a matrix.
 *
 * @param[in] matrix An initialized matrix object operand.
 * @param[out] transpose The transpose of the input matrix.
 *
 * @note For a matrix, with dimensions NxM, the transpose result matrix will
 *       have dimensions MxN.
 * @note If static memory allocation is specified, the dimensions of the result
 *       matrix are set to NxK.
 * @note If dynamic memory allocation is used, the dimensions of the result
 *       matrix must be MxN.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the result pointer is NULL.
 *         ulapack_invalid_argument is returned if dynamic memory allocation is
 *         specified and the dimensions of the result matrix do not equal the
 *         required dimensions of the matrix/vector transpose.
 */
MatrixError_t ulapack_transpose(const Matrix_t *const matrix,
                                Matrix_t *const transpose);

/**
 * @name ulapack_copy
 * Copy a matrix.
 *
 * @param[in] matrix An initialized matrix object operand.
 * @param[out] result The copy of the input matrix.
 *
 * @note For a matrix, with dimensions NxM, the copy result matrix must have
 *       dimensions JxK, where J >= N and K >= M.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_uninit_obj is returned if the result pointer is NULL.
 *         ulapack_invalid_argument is returned if the result matrix is not
 *         greater than or equal to in size over the input matrix.
 */
MatrixError_t ulapack_copy(const Matrix_t *const matrix,
                           Matrix_t *const result);

/**
 * @name ulapack_diag
 * Put a vector on the diagonal of a matrix.
 *
 * @note For an Nx1 vector, the matrix must be of size NxN.
 * @note For a 1xN vector, the matrix must be of size NxN.
 * @note The off diagonal elements of the matrix are set to 0.
 *
 * @param[in] vector The vector to put on the diagonal of a matrix.
 * @param[out] matrix The matrix to put the vector on the diagonal of.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_uninit_obj is returned if the result pointer is NULL.
 *         ulapack_invalid_argument is returned if the result matrix does not
 *         have proper dimensions, or the vector is not a vector.
 */
MatrixError_t ulapack_diag(const Matrix_t *const vector,
                           Matrix_t *const matrix);
/**
 * @name ulapack_det
 * Take the determinant of a matrix.
 *
 * @note The determinant is calculated via an LU decomposition of the matrix.
 *       For matrices of square dimensions of 4 or more, the matrix is 
 *       decomposed into an upper and lower triangular form first to assist
 *       in the determinant calculation.
 * @note A closed form solution is used for matrices of size 3x3 and smaller.
 *
 * @param[in] matrix The matrix to take the determinant of.
 * @param[out] det The determinant result.
 *
 * @return ULAPack success code. ulapack_success is returned if the input was
 *         valid and the matrix determinant was returned successfully.
 *         ulapack_invalid_argument returned if the input matrix is not square.
 *         ulapack_invalid_argument if static memory allocation is specified and
 *         a matrix passed in is larger than the maximum row size limitation set
 *         by the ULAPACK_MAX_MATRIX_N_ROWS macro.
 *         ulapack_uninit_obj if the matrix object is not initialized.
 */
MatrixError_t ulapack_det(const Matrix_t *const matrix,
                          MatrixEntry_t *const det);

/**
 * @ulapack_lu
 * LU (lower upper) decompose a matrix.
 *
 * @note If static memory allocation is specified, the return matrices will have
 *       equal dimensions to that of the input matrix.
 *
 * @param[in] matrix The matrix to make upper triangular.
 * @param[out] upper_matrix The output upper triangular matrix result.
 * @param[out] lower_matrix The output lower triangular matrix result.
 *
 * @return ULAPack success codes. ulapack_success is returned if both matrices
 *         passed in are valid to either read from or write to.
 *         ulapack_uninit_obj is returned if the one or more of the inputs
 *         are not initialized.
 *         ulapack_invalid_argument is returned if the input matrix object is
 *         not square. This is also returned if dynamic memory allocation is
 *         specified and the upper/lower_matrix argument does not have equal 
 *         dimensions to the input matrix.
 */
MatrixError_t ulapack_lu(const Matrix_t *const matrix,
                         Matrix_t *const upper_matrix,
                         Matrix_t *const lower_matrix);

/**
 * @name ulapack_inverse
 * Invert a square matrix.
 *
 * @note If static memory allocation is specified, the dimensions of the result
 *       matrix are set to that of the input operand matrix.
 * @note To compute matrix inversions using LU decomposition, define 
 *       ULAPACK_INVERSE_VIA_LU. Leave undefined for pivot finding/triangulation
 *       elimination method.
 *
 * param[in] matrix An initialized matrix to invert.
 * param[out] inverse An initialized matrix to store the inverse into.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the result pointer is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if dynamic memory allocation is
 *         specified and the dimensions of the result matrix do not equal the
 *         required dimensions of the matrix inversion.
 *         ulapack_invalid_argument is returned if the input matrix is not 
 *         square.
 */
MatrixError_t ulapack_inverse(const Matrix_t *const matrix,
                              Matrix_t *const inverse);

/**
 * @name ulapack_pinverse
 * Take the pseudo (Moore-Penrose) inverse, A^*, of a matrix, A.
 *
 * @note The pseudo inverse operation is computed via A^* = (A^T * A)^-1 * A^T
 *       As a result, the ulapack_inverse is used and is therefore dependent
 *       on the method used for matrix inversion operation.
 * @note For an input matrix with dimensions NxM, the pseudo inverse will be of
 *       size MxN.
 *
 * @todo Make this work with SVD to take the pseudo inverse of poorly 
 *       conditioned matrices.
 *
 * @param[in] matrix The matrix to take the pseudo inverse of.
 * @param[out] pinverse The result pseudo inverse result.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the result pointer is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if dynamic memory allocation is
 *         specified and the dimensions of the result matrix do not equal the
 *         required dimensions of the matrix inversion.
 */
MatrixError_t ulapack_pinverse(const Matrix_t *const matrix,
                               Matrix_t *const pinverse);

/**
 * @name ulapack_least_squares
 * Minimize the sum of the squares of the residuals of a system of equations.
 *
 * @brief For a system of equations: y = Ax
 *        Let A be a matrix representing a system of linear equations, or 
 *        mapping from the input to an output of the system.
 *        Let y be the output vector of that of a system. 
 *        Let x be the independent variable of the system.
 *        The least squares solutions will yield an approximate value for x 
 *        given a vector of observations, y, and a system, A.
 *        i.e. x ~= ( (A^T * A)^-1 * A^T ) * y = (A^*)y
 * 
 * @todo Use SVD as the back-end for this function instead of LU decomposition
 *       within the pinverse function.
 *
 * @note For a matrix A of dimension NxM, y must have dimensions Nx1, and x must
 *       have dimensions Mx1.
 *
 * @param[in] A the matrix representing the system of linear equations.
 * @param[in] y The vector of observations or measurements
 * @param[out] x The vector to approximate.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the result pointer is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if dynamic memory allocation is
 *         specified and the dimensions of the result matrix do not equal the
 *         required dimensions of the matrix inversion.
 *         ulapack_invalid_argument is returned if the input matrix and vectors
 *         do not match their expected dimensions.
 */
MatrixError_t ulapack_least_squares(const Matrix_t *const A,
                                    const Matrix_t *const y,
                                    Matrix_t *const x);

/**
 * @name ulapack_vandermonde
 * Create a Vandermonde matrix from a vector.
 *
 * @brief Let V be the Vandermonde matrix created, V_{i,j} = x_{i}^(j-1).
 *
 * @note x must be a column vector.
 *
 * @param[in] x The vector to make the Vandermonde matrix out of.
 * @param order The order of the matrix.
 * @param[out] V the Vandermonde matrix created.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the result pointer is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if dynamic memory allocation is
 *         specified and the dimensions of the result matrix do not equal the
 *         required dimensions of the matrix operations.
 *         ulapack_invalid_argument is returned if the input matrix and vectors
 *         do not match their expected dimensions.
 */
MatrixError_t ulapack_vandermonde(const Matrix_t *const x,
                                  const Index_t order,
                                  Matrix_t *const V);

/**
 * @name ulapack_power
 * Take every element in a matrix to the power of a specified value.
 *
 * @param[in] matrix The matrix to take the power of.
 * @param power The power to take the matrix to the power of.
 * @param[out] result The result of the operation.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the result pointer is not 
 *         initialized.
 */
MatrixError_t ulapack_power(const Matrix_t *const matrix,
                            const uint64_t power,
                            Matrix_t *const result);

/**
 * @name ulapack_polyfit
 * Fit an nth degree polynomial.
 *
 * @note x and y must have equal dimensions, Mx1.
 * @note x and y must be column vectors.
 * @note The result coefficient vector, p, must have dimension n+1 x 1.
 * @note n must be greater than or equal to M+1.
 *
 * @param[in] x The independent variable in a system.
 * @param[in] y The dependent variable in a system.
 * @param n The number of degrees to run a polynomial regression on.
 * @param[out] p The polynomial coefficients of the regression.
 *
 * @return ULAPack success code ulapack_success is returned upon a successful
 *         operation.
 *         ulapack_uninit_obj is returned if a matrix object passed in is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if the result pointer is not 
 *         initialized.
 *         ulapack_invalid_argument is returned if dynamic memory allocation is
 *         specified and the dimensions of the result matrix do not equal the
 *         required dimensions of the matrix operations.
 *         ulapack_invalid_argument is returned if the input matrix and vectors
 *         do not match their expected dimensions.
 */
MatrixError_t ulapack_polyfit(const Matrix_t *const x,
                              const Matrix_t *const y,
                              const uint64_t n,
                              Matrix_t *const p);

/* 
 * @name ulapack_svd
 * Decompose a matrix using Singular Value Decomposition (SVD). 
 *
 * @brief Takes an MxN matrix and decomposes it into UDV, where U and V are left
 *        and right orthogonal transformation matrices, and D is a diagonal 
 *        matrix of singular values.
 *
 *        This routine is adapted from svdecomp.c in XLISP-STAT 2.1 which is 
 *        code from Numerical Recipes adapted by Luke Tierney and David Betz.
 *        This code has been modified for use in ULAPack in the following ways:
 *            - Safer type checking.
 *            - Use with both statically and dynamically allocated memory.
 *            - Adapted with non-clib calls (except for sqrt()).
 *            - Use of ULAPack object types and error codes.
 *
 * @note The number of rows of the input matrix must be greater than or equal to
 *       the number of columns in the input matrix, i.e. (M >= N).
 *
 * @param[in] matrix An MxN tall (m > n) matrix to be decomposed.
 * @param[out] U The left orthogonal transformation matrix. U is an MxN matrix.
 * @param[out] S The vector of singular values of the matrix. S is an N vector.
 * @param[out] V The right orthogonal transformation matrix. V is an NxN matrix.
 *
 * @return ULAPack error code. 
 *         ulapack_success is returned upon a successful decomposition. 
 *         ulapack_error is returned if the matrix cannot be decomposed due to a 
 *         lack of numerical convergence (1000 * MAX_CONV_LOOPS).
 *         ulapack_unint_obj is returned if one or more of the passed in matrix
 *         objects have not been initialized. 
 *         ulapack_invalid_argument is returned if the matrix dimensions are not 
 *         equal to their expected values for the SVD operation if dynamic
 *         memory allocation is specified. If static allocation is specified
 *         the dimensions of the passed in parameters are modified.
 */
#define MAX_CONV_LOOPS (30)
MatrixError_t ulapack_svd(const Matrix_t *const matrix,
                          Matrix_t *const U, Matrix_t *const S, Matrix_t *const V);

/**
 * @name ulapack_pca
 * Principle Component Analysis
 *
 * @brief A statistical procedure that uses an orthogonal transformation to 
 *        convert a set of observations of possibly correlated variables into a
 *        set of values of linearly uncorrelated variables called principal 
 *        components.
 *
 * @note ulapack_svd is needed to compute the PCA of a matrix.
 *
 * @param matrix[in] matrix The matrix to analyze.
 * @param matrix[out] T The score matrix.
 *
 * @return ULAPack error code. 
 *         ulapack_success is returned upon a successful PCA operation. 
 *         ulapack_error is returned if the matrix cannot be decomposed due to a 
 *         lack of numerical convergence (1000 * MAX_CONV_LOOPS) from SVD.
 *         ulapack_unint_obj is returned if one or more of the passed in matrix
 *         objects have not been initialized. 
 *         ulapack_invalid_argument is returned if the dimensions of T is not 
 *         equal to its expected value if dynamic memory allocation is 
 *         specified. If static allocation is specified  the dimensions of the 
 *         passed in parameters are modified.
 */
MatrixError_t ulapack_pca(const Matrix_t *const matrix, Matrix_t *const T);

/*
 * End header guard definition.
 */
#endif
