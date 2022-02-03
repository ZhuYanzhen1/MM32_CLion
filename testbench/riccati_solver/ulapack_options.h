/**
 * @file ulapack_options.h
 * @brief Settings profile for uLAPack operations and usage.
 *
 * @author Sargis Yonan
 * @date July 8, 2017
 *
 * @version 1.0.1
 **/

/*
 * File header guards.
 */
#ifndef __ULAPACK_OPTIONS_H__
#define __ULAPACK_OPTIONS_H__

/*
 * Define the entry data type.
 */
#define ULAPACK_MATRIX_ENTRY_TYPE double

/*
 * The data type for the row and column elements.
 * Unsigned type recommended.
 */
#define ULAPACK_INDEX_TYPE unsigned char
/*
 * The signed equivalent data type for ULAPACK_INDEX_TYPE.
 */
#define ULAPACK_SIGNED_INDEX_TYPE int

/*
 * Define ULAPACK_USE_PRINT to enable the ulapack_print function.
 */
#define ULAPACK_USE_PRINT
#ifdef ULAPACK_USE_PRINT
/*
 * The fprintf delimiter must be defined if ULAPACK_USE_PRINT is defined.
 * ULAPACK_MATRIX_ENTRY_TYPE is currently set to double, and so the delimiter is
 * set to %lf including the '%' symbol.
 */
#define ULAPACK_PRINT_DELIMITER "%lf"

#ifndef ULAPACK_PRINT_DELIMITER
#error "ULAPACK_PRINT_DELIMITER must be defined when ULAPACK_USE_PRINT is enabled."
#endif
#endif

/*
 * Small value for floating point tolerance considerations
 * for inversions and FP tolerance.
 */
#define MINIMUM_THRESHOLD_TOLERANCE (.00001)

/*
 * Defined if memory should be initialized to zeros.
 */
#define ULAPACK_INITIALIZE_MEMORY

/*
 * Option for using LU decomposition for matrix inversions.
 */
#define ULAPACK_INVERSE_VIA_LU

/*
 * Keep defined for static memory allocation declarations.
 */
#define ULAPACK_USE_STATIC_ALLOC

/*
 * Keep defined for dynamic memory allocation.
 */
// #define ULAPACK_USE_DYNAMIC_ALLOC

#ifndef ULAPACK_USE_STATIC_ALLOC
#ifndef ULAPACK_USE_DYNAMIC_ALLOC
#error "Error: Neither ULAPACK_USE_DYNAMIC_ALLOC or ULAPACK_USE_STATIC_ALLOC are defined."
#endif
#endif

#ifdef ULAPACK_USE_STATIC_ALLOC
#ifdef ULAPACK_USE_DYNAMIC_ALLOC
#error "Error: Both ULAPACK_USE_DYNAMIC_ALLOC and ULAPACK_USE_STATIC_ALLOC can not be simultaneously defined."
#endif
#endif

#ifdef ULAPACK_USE_DYNAMIC_ALLOC
/*
 * Specify the memory allocator function to use which takes the form of stdlib's
 * malloc function : void *alloc(size_t bytes).
 */
#include <stdlib.h>
#define ULAPACK_ALLOC malloc

/*
 * Specify the memory freeing function to use which takes the form of stdlib's
 * free function : void free(void* ptr).
 */
#include <stdlib.h>
#define ULAPACK_FREE free
#endif

#ifdef ULAPACK_USE_DYNAMIC_ALLOC
#ifndef ULAPACK_ALLOC
#error "Error: ULAPACK_ALLOC is not defined even though ULAPACK_USE_DYNAMIC_ALLOC is defined."
#endif

#ifndef ULAPACK_FREE
#error "Error: ULAPACK_FREE is not defined even though ULAPACK_USE_DYNAMIC_ALLOC is defined."
#endif
#endif

#ifdef ULAPACK_USE_STATIC_ALLOC
/*
 * Set the largest row and column sizes for uLAPack matrix types.
 *
 * @note only required to be specified when static allocation is used.
 */
#ifndef ULAPACK_MAX_MATRIX_N_ROWS
#define ULAPACK_MAX_MATRIX_N_ROWS (10u)
#endif
#ifndef ULAPACK_MAX_MATRIX_N_COLS
#define ULAPACK_MAX_MATRIX_N_COLS (10u)
#endif

/*
 * The maximum number of rows and columns must be specified
 * for static matrix allocation.
 */
#ifndef ULAPACK_MAX_MATRIX_N_ROWS
#error "ULAPACK_MAX_MATRIX_N_ROWS not defined."
#endif
#ifndef ULAPACK_MAX_MATRIX_N_COLS
#error "ULAPACK_MAX_MATRIX_N_COLS not defined."
#endif
#endif

/*
 * End header guard definition.
 */
#endif