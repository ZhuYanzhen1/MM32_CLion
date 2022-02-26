//
// Created by 16625 on 2022-02-26.
//

#ifndef MM32F3277_MATH_FUSION_SENSOR_H_
#define MM32F3277_MATH_FUSION_SENSOR_H_

#include "hal_device.h"

#define FIXMATRIX_MAX_SIZE  6  // TODO 记得改这里
#define matrix_set(matrix, row, column, value)  matrix->data[row][column] = value
#define FIXMATRIX_OVERFLOW  0x01
#define FIXMATRIX_DIMERR    0x02
#define FIXMATRIX_USEERR    0x04
#define FIXMATRIX_SINGULAR  0x08
#define FIXMATRIX_NEGATIVE  0x10
#define fix16_t             int
#define fix16_add(x, y)     ((x)+(y))
#define clz(x)              (__builtin_clzl(x) - (8 * sizeof(long) - 32))
#define F16(x)              ((fix16_t)(((x) >= 0) ? ((x) * 65536.0 + 0.5) : ((x) * 65536.0 - 0.5)))
//#define __CHAR_BIT__        8
#define CHAR_BIT            8
//TODO 这两个宏不确定要不要用
#define FIXMATH_NO_OVERFLOW
#define FIXMATH_NO_ROUNDING

static inline fix16_t fix16_abs(fix16_t x) { return (x < 0 ? -x : x); }

typedef struct {
    char rows;
    char columns;
    char errors;
    fix16_t data[FIXMATRIX_MAX_SIZE][FIXMATRIX_MAX_SIZE];
} mf16;

typedef struct {
    fix16_t x;
    fix16_t y;
    fix16_t z;
} v3d;

/*!
* \brief Kalman Filter measurement structure
* \see kalman16_t
*/
typedef struct {
    /*!
    * \brief Measurement vector (#measurements x 1)
    */
    mf16 z;

    /*!
    * \brief Measurement transformation matrix (#measurements x #states)
    * \see R
    */
    mf16 H;

    /*!
    * \brief Observation process noise covariance matrix (#measurements x #measurements)
    * \see H
    */
    mf16 R;

} kalman16_observation_t;

/*!
* \brief Kalman Filter structure
* \see kalman16_observation_t
* \see kalman16_uc_t
*/
typedef struct {
    /*!
    * \brief State vector The state transition matrix (#states x #states)
    */
    mf16 x;

    /*!
    * \brief System matrix (#states x 1)
    * \see P
    */
    mf16 A;

    /*!
    * \brief System covariance matrix (#states x #states)
    * \see A
    */
    mf16 P;

    /*!
    * \brief Input vector (#inputs x 1)
    */
    mf16 u;

    /*!
    * \brief Input matrix (#inputs x #inputs)
    * \see Q
    */
    mf16 B;

    /*!
    * \brief Input covariance/uncertainty matrix (#inputs x #inputs)
    * \see B
    */
    mf16 Q;

} kalman16_t;

typedef struct {
    fix16_t a; // Real part
    fix16_t b; // i
    fix16_t c; // j
    fix16_t d; // k
} qf16;

/*!
* \brief Kalman Filter structure for a system without control inputs
* \see kalman16_observation_t
* \see kalman16_t
*/
typedef struct {
    /*!
    * \brief State vector The state transition matrix (#states x #states)
    */
    mf16 x;//

    /*!
    * \brief System matrix (#states x 1)
    * \see P
    */
    mf16 A;

    /*!
    * \brief System covariance matrix (#states x #states)
    * \see A
    */
    mf16 P;

    /*!
    * \brief System process noise matrix (#states x #states)
    * \see P
    */
    mf16 Q;

} kalman16_uc_t;

static const fix16_t fix16_maximum = 0x7FFFFFFF; /*!< the maximum value of fix16_t */
static const fix16_t fix16_minimum = 0x80000000; /*!< the minimum value of fix16_t */
static const fix16_t
    fix16_overflow = 0x80000000; /*!< the value used to indicate overflows when FIXMATH_NO_OVERFLOW is not specified */

static const fix16_t fix16_pi = 205887;     /*!< fix16_t value of pi */
static const fix16_t fix16_e = 178145;     /*!< fix16_t value of e */
static const fix16_t fix16_one = 0x00010000; /*!< fix16_t value of 1 */

/* Conversion functions between fix16_t and float/integer.
 * These are inlined to allow compiler to optimize away constant numbers
 */
static inline fix16_t fix16_from_int(int a) { return a * fix16_one; }

__attribute__((unused)) extern v3d m_gyroscope;
extern v3d m_accelerometer;
extern v3d m_magnetometer;

fix16_t fix16_mul(fix16_t inArg0, fix16_t inArg1);

void get_sensors();
void fusion_fetch_angles(register fix16_t *const roll, register fix16_t *const pitch, register fix16_t *const yaw);
void fusion_fetch_quaternion(register qf16 *const quat);
void fusion_update(register const fix16_t deltaT);
void fusion_predict(register const fix16_t deltaT);

#endif //MAIN_C_MATH_FUSION_SENSOR_H_
