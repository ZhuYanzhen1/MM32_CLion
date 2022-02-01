/**
 * @file ulapack_type.h
 * @brief Matrix structure type definitions for the uLAPack library.
 *
 * This file contains the structure definition for both statically
 * and dynamically allocated matrix types.
 *
 * @author Sargis Yonan
 * @date July 8, 2017
 *
 * @version 1.0.1
 **/

/*
 * File header guards.
 */
#ifndef __ULAPACK_TYPE_H__
#define __ULAPACK_TYPE_H__

/*
 * Including standard integer header for uintX_t types.
 */
#include <stdint.h>

/*
 * uLAPack options header.
 */
#include "ulapack_options.h"

/*
 * Define the matrix type.
 */
typedef ULAPACK_MATRIX_ENTRY_TYPE MatrixEntry_t;

/*
 * Define the index types.
 */
typedef ULAPACK_INDEX_TYPE Index_t;
typedef ULAPACK_SIGNED_INDEX_TYPE SIndex_t;

#ifdef ULAPACK_USE_STATIC_ALLOC
    /*
     * Definition for the static ulapck_matrix object.
     */
    typedef struct ulapack_matrix {
        /*@{*/
        MatrixEntry_t entry[ULAPACK_MAX_MATRIX_N_ROWS]
                           [ULAPACK_MAX_MATRIX_N_COLS]; /**< the matrix elements. */

        uint64_t n_rows; /**< number of rows in the matrix. */
        uint64_t n_cols; /**< number of columns in the matrix. */
        /*@}*/
    } Matrix_t;
#endif

#ifdef ULAPACK_USE_DYNAMIC_ALLOC
    /**
     * Definition for the dynamic ulapck_matrix object.
     */
    typedef struct ulapack_matrix {
        /*@{*/
        MatrixEntry_t **entry; /**< pointer to the matrix elements. */
        uint64_t n_rows; /**< number of rows in the matrix. */
        uint64_t n_cols; /**< number of columns in the matrix. */
        /*@}*/
    } Matrix_t;
#endif

/*
 * Definition of error types.
 */
typedef enum {
    /*@{*/
    ulapack_error = -1, /**< general error code. */
    ulapack_oom = -2, /**< out of memory error code. */
    ulapack_invalid_argument = -3, /**< bad argument given to function. */
    ulapack_uninit_obj = -4, /**< uninitialized object passed into function. */
    ulapack_success = 1, /**< general success code. */
    /*@}*/
} MatrixError_t;

/*
 * End header guard definition.
 */
#endif