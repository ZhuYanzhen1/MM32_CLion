//
// Created by 16625 on 2022-02-26.
//

#include "fusion_sensor.h"
#include "hal_device.h"
#include "adis16470.h"
#include "hmc5883l.h"
#include <string.h>

extern adis16470_t imu;
extern hmc5883l magnetometer;

static kalman16_uc_t kf_orientation;
static kalman16_uc_t kf_attitude;
static kalman16_observation_t kfm_gyro;
static const fix16_t attitude_threshold = F16(0.14);
static kalman16_observation_t kfm_accel;

__attribute__((unused)) v3d m_gyroscope = {0, 0, 0};
v3d m_accelerometer = {0, 0, 0};
v3d m_magnetometer = {0, 0, 0};

/*
* \brief Observation axis uncertainty (accelerometer)
*/
static const fix16_t initial_r_axis = F16(0.05);
/*
* \brief Tuning factor for the axis observation
*/
static const fix16_t alpha1 = F16(5);
/*
* \brief Observation gyro uncertainty
*/
static const fix16_t initial_r_gyro = F16(0.02);
/*
* \brief Tuning factor for the gyro observation
*/
static const fix16_t alpha2 = F16(.8);
/*!
* \brief The Kalman filter observation instance used to update the prediction with magnetometer data
*/
static kalman16_observation_t kfm_magneto;
/*
* \brief Observation projection uncertainty (magnetometer)
*/
static const fix16_t initial_r_projection = F16(0.02);

mf16 *kalman_get_state_vector_uc(kalman16_uc_t *kf) {
    return &(kf->x);
}
mf16 *kalman_get_system_covariance_uc(kalman16_uc_t *kf) {
    return &(kf->P);
}
static fix16_t fix16_atan_cache_value[4096] = {0};
static fix16_t fix16_atan_cache_index[2][4096] = {{0}, {0}};
fix16_t fix16_sub(fix16_t a, fix16_t b) {
    uint32_t _a = a, _b = b;
    uint32_t diff = _a - _b;

    // Overflow can only happen if sign of a != sign of b, and then
    // it causes sign of diff != sign of a.
    if (((_a ^ _b) & 0x80000000) && ((_a ^ diff) & 0x80000000))
        return fix16_overflow;

    return diff;
}
fix16_t fix16_mul(fix16_t inArg0, fix16_t inArg1) {
    int64_t product = (int64_t) inArg0 * inArg1;

#ifndef FIXMATH_NO_OVERFLOW
    // The upper 17 bits should all be the same (the sign).
    uint32_t upper = (product >> 47);
#endif

    if (product < 0) {
#ifndef FIXMATH_NO_OVERFLOW
        if (~upper)
            return fix16_overflow;
#endif

#ifndef FIXMATH_NO_ROUNDING
        // This adjustment is required in order to round -1/2 correctly
        product--;
#endif
    } else {
#ifndef FIXMATH_NO_OVERFLOW
        if (upper)
            return fix16_overflow;
#endif
    }

#ifdef FIXMATH_NO_ROUNDING
    return product >> 16;
#else
    fix16_t result = product >> 16;
    result += (product & 0x8000) >> 15;

    return result;
#endif
}
fix16_t fix16_sqrt(fix16_t inValue) {
    uint8_t neg = (inValue < 0);
    uint32_t num = (neg ? -inValue : inValue);
    uint32_t result = 0;
    uint32_t bit;
    uint8_t n;

    // Many numbers will be less than 15, so
    // this gives a good balance between time spent
    // in if vs. time spent in the while loop
    // when searching for the starting value.
    if (num & 0xFFF00000)
        bit = (uint32_t) 1 << 30;
    else
        bit = (uint32_t) 1 << 18;

    while (bit > num) bit >>= 2;

    // The main part is executed twice, in order to avoid
    // using 64 bit values in computations.
    for (n = 0; n < 2; n++) {
        // First we get the top 24 bits of the answer.
        while (bit) {
            if (num >= result + bit) {
                num -= result + bit;
                result = (result >> 1) + bit;
            } else {
                result = (result >> 1);
            }
            bit >>= 2;
        }

        if (n == 0) {
            // Then process it again to get the lowest 8 bits.
            if (num > 65535) {
                // The remainder 'num' is too large to be shifted left
                // by 16, so we have to add 1 to result manually and
                // adjust 'num' accordingly.
                // num = a - (result + 0.5)^2
                //	 = num + result^2 - (result + 0.5)^2
                //	 = num - result - 0.5
                num -= result;
                num = (num << 16) - 0x8000;
                result = (result << 16) + 0x8000;
            } else {
                num <<= 16;
                result <<= 16;
            }

            bit = 1 << 14;
        }
    }

#ifndef FIXMATH_NO_ROUNDING
    // Finally, if next bit would have been 1, round the result upwards.
    if (num > result) {
        result++;
    }
#endif

    return (neg ? -(fix16_t) result : (fix16_t) result);
}
static inline fix16_t fix16_sq(fix16_t x) {
    return fix16_mul(x, x);
}
fix16_t fix16_div(fix16_t a, fix16_t b) {
    // This uses the basic binary restoring division algorithm.
    // It appears to be faster to do the whole division manually than
    // trying to compose a 64-bit divide out of 32-bit divisions on
    // platforms without hardware divide.

    if (b == 0)
        return fix16_minimum;

    uint32_t remainder = (a >= 0) ? a : (-a);
    uint32_t divider = (b >= 0) ? b : (-b);

    uint32_t quotient = 0;
    uint32_t bit = 0x10000;

    /* The algorithm requires D >= R */
    while (divider < remainder) {
        divider <<= 1;
        bit <<= 1;
    }

#ifndef FIXMATH_NO_OVERFLOW
    if (!bit)
        return fix16_overflow;
#endif

    if (divider & 0x80000000) {
        // Perform one step manually to avoid overflows later.
        // We know that divider's bottom bit is 0 here.
        if (remainder >= divider) {
            quotient |= bit;
            remainder -= divider;
        }
        divider >>= 1;
        bit >>= 1;
    }

    /* Main division loop */
    while (bit && remainder) {
        if (remainder >= divider) {
            quotient |= bit;
            remainder -= divider;
        }

        remainder <<= 1;
        bit >>= 1;
    }

#ifndef FIXMATH_NO_ROUNDING
    if (remainder >= divider) {
        quotient++;
    }
#endif

    fix16_t result = quotient;

    /* Figure out the sign of result */
    if ((a ^ b) & 0x80000000) {
#ifndef FIXMATH_NO_OVERFLOW
        if (result == fix16_minimum)
            return fix16_overflow;
#endif

        result = -result;
    }

    return result;
}
static const fix16_t THREE_PI_DIV_4 = 0x00025B2F;       /*!< Fix16 value of 3PI/4 */
static const fix16_t PI_DIV_4 = 0x0000C90F;             /*!< Fix16 value of PI/4 */

fix16_t fix16_atan2(fix16_t inY, fix16_t inX) {
    fix16_t abs_inY, mask, angle, r, r_3;

#ifndef FIXMATH_NO_CACHE
    uintptr_t hash = (inX ^ inY);
    hash ^= hash >> 20;
    hash &= 0x0FFF;
    if ((fix16_atan_cache_index[0][hash] == inX) && (fix16_atan_cache_index[1][hash] == inY))
        return fix16_atan_cache_value[hash];
#endif

    /* Absolute inY */
    mask = (inY >> (sizeof(fix16_t) * CHAR_BIT - 1));
    abs_inY = (inY + mask) ^ mask;

    if (inX >= 0) {
        r = fix16_div((inX - abs_inY), (inX + abs_inY));
        r_3 = fix16_mul(fix16_mul(r, r), r);
        angle = fix16_mul(0x00003240, r_3) - fix16_mul(0x0000FB50, r) + PI_DIV_4;
    } else {
        r = fix16_div((inX + abs_inY), (abs_inY - inX));
        r_3 = fix16_mul(fix16_mul(r, r), r);
        angle = fix16_mul(0x00003240, r_3)
            - fix16_mul(0x0000FB50, r)
            + THREE_PI_DIV_4;
    }
    if (inY < 0) {
        angle = -angle;
    }

#ifndef FIXMATH_NO_CACHE
    fix16_atan_cache_index[0][hash] = inX;
    fix16_atan_cache_index[1][hash] = inY;
    fix16_atan_cache_value[hash] = angle;
#endif

    return angle;
}
fix16_t fix16_atan(fix16_t x) {
    return fix16_atan2(x, fix16_one);
}
fix16_t fix16_asin(fix16_t x) {
    if ((x > fix16_one)
        || (x < -fix16_one))
        return 0;

    fix16_t out;
    out = (fix16_one - fix16_mul(x, x));
    out = fix16_div(x, fix16_sqrt(out));
    out = fix16_atan(out);
    return out;
}

static fix16_t scale_value(fix16_t value, int_fast8_t scale) {
    if (scale > 0) {
        fix16_t temp = value << scale;
        if (temp >> scale != value)
            return fix16_overflow;
        else
            return temp;
    } else if (scale < 0) {
        return value >> -scale;
    } else {
        return value;
    }
}

fix16_t fa16_dot(const fix16_t *a, uint_fast8_t a_stride,
                 const fix16_t *b, uint_fast8_t b_stride,
                 uint_fast8_t n) {
    int64_t sum = 0;

    while (n--) {
        if (*a != 0 && *b != 0) {
            sum += (int64_t) (*a) * (*b);
        }

        // Go to next item
        a += a_stride;
        b += b_stride;
    }

    // The upper 17 bits should all be the same (the sign).
    uint32_t upper = sum >> 47;
    if (sum < 0) {
        upper = ~upper;

#ifndef FIXMATH_NO_ROUNDING
        // This adjustment is required in order to round -1/2 correctly
        sum--;
#endif
    }

#ifndef FIXMATH_NO_OVERFLOW
        if (upper)
            return fix16_overflow;
#endif

    fix16_t result = sum >> 16;

#ifndef FIXMATH_NO_ROUNDING
    result += (sum & 0x8000) >> 15;
#endif

    return result;
}

void fa16_unalias(void *dest, void **a, void **b, void *tmp, unsigned size) {
    if (dest == *a) {
        memcpy(tmp, *a, size);
        *a = tmp;

        if (dest == *b)
            *b = tmp;
    } else if (dest == *b) {
        memcpy(tmp, *b, size);
        *b = tmp;
    }
}
fix16_t fa16_norm(const fix16_t *a, uint_fast8_t a_stride, uint_fast8_t n) {
    int64_t sum = 0;

    while (n--) {
        if (*a != 0) {
            sum += (int64_t) (*a) * (*a);
        }

        a += a_stride;
    }

    int_fast8_t scale = 0;
    uint32_t highpart = (uint32_t) (sum >> 32);
    uint32_t lowpart = (uint32_t) sum;
    if (highpart)
        scale = (int_fast8_t) (33 - clz(highpart));
    else if (lowpart & 0x80000000)
        scale = 1;

    if (scale & 1) scale++;

    fix16_t result = fix16_sqrt((uint32_t) (sum >> scale));
    result = scale_value(result, scale / 2 - 8);

    return result;
}

fix16_t v3d_norm(const v3d *a) {
    return fa16_norm(&a->x, &a->y - &a->x, 3);
}

static void mf16_addsub(mf16 *dest, const mf16 *a, const mf16 *b, uint8_t add) {
    int row, column;

    dest->errors = a->errors | b->errors;
    if (a->columns != b->columns || a->rows != b->rows)
        dest->errors |= FIXMATRIX_DIMERR;

    dest->rows = a->rows;
    dest->columns = a->columns;

    for (row = 0; row < dest->rows; row++) {
        for (column = 0; column < dest->columns; column++) {
            fix16_t sum;
            if (add)
                sum = fix16_add(a->data[row][column], b->data[row][column]);
            else
                sum = fix16_sub(a->data[row][column], b->data[row][column]);

            if (sum == fix16_overflow)
                dest->errors |= FIXMATRIX_OVERFLOW;

            dest->data[row][column] = sum;
        }
    }
}

void mf16_sub(mf16 *dest, const mf16 *a, const mf16 *b) {
    mf16_addsub(dest, a, b, 0);
}

void mf16_mul(mf16 *dest, const mf16 *a, const mf16 *b) {
    int row, column;

    // If dest and input matrices alias, we have to use a temp matrix.
    mf16 tmp;
    fa16_unalias(dest, (void **) &a, (void **) &b, &tmp, sizeof(tmp));

    dest->errors = a->errors | b->errors;

    if (a->columns != b->rows)
        dest->errors |= FIXMATRIX_DIMERR;

    dest->rows = a->rows;
    dest->columns = b->columns;

    for (row = 0; row < dest->rows; row++) {
        for (column = 0; column < dest->columns; column++) {
            dest->data[row][column] = fa16_dot(
                &a->data[row][0], 1,
                &b->data[0][column], FIXMATRIX_MAX_SIZE,
                a->columns);

            if (dest->data[row][column] == fix16_overflow)
                dest->errors |= FIXMATRIX_OVERFLOW;
        }
    }
}

void mf16_add(mf16 *dest, const mf16 *a, const mf16 *b) {
    mf16_addsub(dest, a, b, 1);
}

void mf16_mul_bt(mf16 *dest, const mf16 *a, const mf16 *bt) {
    int row, column;

    // If dest and input matrices alias, we have to use a temp matrix.
    mf16 tmp;
    fa16_unalias(dest, (void **) &a, (void **) &bt, &tmp, sizeof(tmp));

    dest->errors = a->errors | bt->errors;

    if (a->columns != bt->columns)
        dest->errors |= FIXMATRIX_DIMERR;

    dest->rows = a->rows;
    dest->columns = bt->rows;

    for (row = 0; row < dest->rows; row++) {
        for (column = 0; column < dest->columns; column++) {
            dest->data[row][column] = fa16_dot(
                &a->data[row][0], 1,
                &bt->data[column][0], 1,
                a->columns);

            if (dest->data[row][column] == fix16_overflow)
                dest->errors |= FIXMATRIX_OVERFLOW;
        }
    }
}

void mf16_mul_abat(mf16 *dest, const mf16 *a, const mf16 *b) {
    mf16_mul(dest, a, b);
    mf16_mul_bt(dest, dest, a);
}

void mf16_cholesky(mf16 *dest, const mf16 *matrix) {
    // This is the Cholesky–Banachiewicz algorithm.
    // Refer to http://en.wikipedia.org/wiki/Cholesky_decomposition#The_Cholesky.E2.80.93Banachiewicz_and_Cholesky.E2.80.93Crout_algorithms

    int row, column, k;
    dest->errors = matrix->errors;

    if (matrix->rows != matrix->columns)
        dest->errors |= FIXMATRIX_DIMERR;

    dest->rows = dest->columns = matrix->rows;

    for (row = 0; row < dest->rows; row++) {
        for (column = 0; column < dest->columns; column++) {
            if (row == column) {
                // Value on the diagonal
                // Ljj = sqrt(Ajj - sum(Ljk^2, k = 1..(j-1))
                fix16_t value = matrix->data[row][column];
                for (k = 0; k < column; k++) {
                    fix16_t Ljk = dest->data[row][k];
                    Ljk = fix16_mul(Ljk, Ljk);
                    value = fix16_sub(value, Ljk);

                    if (value == fix16_overflow || Ljk == fix16_overflow)
                        dest->errors |= FIXMATRIX_OVERFLOW;
                }

                if (value < 0) {
                    if (value < -65)
                        dest->errors |= FIXMATRIX_NEGATIVE;
                    value = 0;
                }

                dest->data[row][column] = fix16_sqrt(value);
            } else if (row < column) {
                // Value above diagonal
                dest->data[row][column] = 0;
            } else {
                // Value below diagonal
                // Lij = 1/Ljj (Aij - sum(Lik Ljk, k = 1..(j-1)))
                fix16_t value = matrix->data[row][column];
                for (k = 0; k < column; k++) {
                    fix16_t Lik = dest->data[row][k];
                    fix16_t Ljk = dest->data[column][k];
                    fix16_t product = fix16_mul(Lik, Ljk);
                    value = fix16_sub(value, product);

                    if (value == fix16_overflow || product == fix16_overflow)
                        dest->errors |= FIXMATRIX_OVERFLOW;
                }
                fix16_t Ljj = dest->data[column][column];
                value = fix16_div(value, Ljj);
                dest->data[row][column] = value;

                if (value == fix16_overflow)
                    dest->errors |= FIXMATRIX_OVERFLOW;
            }
        }
    }
}

void mf16_invert_lt(mf16 *dest, const mf16 *matrix) {
    // This is port of the algorithm as found in the Efficient Java Matrix Library
    // https://code.google.com/p/efficient-java-matrix-library

    int_fast8_t i, j, k;
    const int n = matrix->rows;

    // If dest and input matrices alias, we have to use a temp matrix.
    mf16 tmp;
    fa16_unalias(dest, (void **) &matrix, (void **) &matrix, &tmp, sizeof(tmp));

    dest->errors = dest->errors | matrix->errors;

    // TODO reorder these operations to avoid cache misses

    // inverts the lower triangular system and saves the result
    // in the upper triangle to minimize cache misses
    for (i = 0; i < n; ++i) {
        const fix16_t el_ii = matrix->data[i][i];
        for (j = 0; j <= i; ++j) {
            fix16_t sum = (i == j) ? fix16_one : 0;
            for (k = i - 1; k >= j; --k) {
                sum = fix16_sub(sum, fix16_mul(matrix->data[i][k], dest->data[j][k]));
            }
            dest->data[j][i] = fix16_div(sum, el_ii);
        }
    }
    // solve the system and handle the previous solution being in the upper triangle
    // takes advantage of symmetry
    for (i = n - 1; i >= 0; --i) {
        const fix16_t el_ii = matrix->data[i][i];
        for (j = 0; j <= i; ++j) {
            fix16_t sum = (i < j) ? 0 : dest->data[j][i];
            for (k = i + 1; k < n; ++k) {
                sum = fix16_sub(sum, fix16_mul(matrix->data[k][i], dest->data[j][k]));
            }
            dest->data[i][j] = dest->data[j][i] = fix16_div(sum, el_ii);
        }
    }
}

void mf16_mul_add(mf16 *dest, const mf16 *a, const mf16 *b) {
    int row, column;
    const int
        acolumns = a->columns,
        drows = dest->rows,
        dcolumns = dest->columns;

    // If dest and input matrices alias, we have to use a temp matrix.
    mf16 tmp;
    fa16_unalias(dest, (void **) &a, (void **) &b, &tmp, sizeof(tmp));

    dest->errors = a->errors | b->errors;

    if (a->columns != b->rows)
        dest->errors |= FIXMATRIX_DIMERR;

    dest->rows = a->rows;
    dest->columns = b->columns;

    for (row = drows - 1; row >= 0; --row) {
        const fix16_t *aptr = &a->data[row][0];
        const fix16_t *bptr = &b->data[0][0];
        fix16_t *rowptr = &dest->data[row][0];

        for (column = dcolumns - 1; column >= 0; --column) {
            fix16_t value = fa16_dot(
                aptr, 1,
                bptr + column, FIXMATRIX_MAX_SIZE,
                acolumns);

            // fetch and modify current value
            rowptr[column] = fix16_add(rowptr[column], value);

#ifndef FIXMATH_NO_OVERFLOW
            // test for overflows
            if (rowptr[column] == fix16_overflow)
                dest->errors |= FIXMATRIX_OVERFLOW;
#endif
        }
    }
}

void mf16_mul_sub(mf16 *dest, const mf16 *a, const mf16 *b) {
    int row, column;
    const int
        acolumns = a->columns,
        drows = dest->rows,
        dcolumns = dest->columns;

    // If dest and input matrices alias, we have to use a temp matrix.
    mf16 tmp;
    fa16_unalias(dest, (void **) &a, (void **) &b, &tmp, sizeof(tmp));

    dest->errors = a->errors | b->errors;

    if (a->columns != b->rows)
        dest->errors |= FIXMATRIX_DIMERR;

    dest->rows = a->rows;
    dest->columns = b->columns;

    for (row = drows - 1; row >= 0; --row) {
        const fix16_t *aptr = &a->data[row][0];
        const fix16_t *bptr = &b->data[0][0];
        fix16_t *rowptr = &dest->data[row][0];

        for (column = dcolumns - 1; column >= 0; --column) {
            fix16_t value = fa16_dot(
                aptr, 1,
                bptr + column, FIXMATRIX_MAX_SIZE,
                acolumns);

            // fetch and modify current value
            rowptr[column] = fix16_sub(rowptr[column], value);

#ifndef FIXMATH_NO_OVERFLOW
            // test for overflows
            if (rowptr[column] == fix16_overflow)
                dest->errors |= FIXMATRIX_OVERFLOW;
#endif
        }
    }
}

fix16_t norm3(register const fix16_t a, register const fix16_t b, register const fix16_t c) {
    return fix16_sqrt(fix16_add(fix16_sq(a), fix16_add(fix16_sq(b), fix16_sq(c))));
}

void update_state_matrix_from_state(kalman16_uc_t *const kf, register fix16_t deltaT) {
    mf16 *const A = &kf->A;
    const mf16 *const x = &kf->x;

    fix16_t c1 = x->data[0][0];
    fix16_t c2 = x->data[1][0];
    fix16_t c3 = x->data[2][0];

    //matrix_set(A, 0, 3,   0);
    matrix_set(A, 0, 4, fix16_mul(c3, deltaT));
    matrix_set(A, 0, 5, -fix16_mul(c2, deltaT));

    matrix_set(A, 1, 3, -fix16_mul(c3, deltaT));
    //matrix_set(A, 1, 4,   0);
    matrix_set(A, 1, 5, fix16_mul(c1, deltaT));

    matrix_set(A, 2, 3, fix16_mul(c2, deltaT));
    matrix_set(A, 2, 4, -fix16_mul(c1, deltaT));
    //matrix_set(A, 2, 5,   0);
}

void kalman_predict_P_uc(register kalman16_uc_t *const kf) {
    // matrices and vectors
    const mf16 *A = &kf->A;
    const mf16 *Q = &kf->Q;
    mf16 *P = &kf->P;

    /************************************************************************/
    /* Predict next covariance using system dynamics and input              */
    /* P = A*P*A' + B*Q*B'                                                  */
    /************************************************************************/

    // P = A*P*A'
    mf16_mul_abat(P, A, P);                 // P = A*P*A'
    mf16_add(P, P, Q);                      // P += Q
}

void kalman_correct(kalman16_t *kf, kalman16_observation_t *kfm) {
    mf16 *const x = &kf->x;
    mf16 *const P = &kf->P;
    const mf16 *const H = &kfm->H;

    static mf16 K, S, y,
        temp_HP,    // { H->rows, H->columns };
    temp_PHt;   // { P->rows, H->columns };

    /************************************************************************/
    /* Calculate innovation and residual covariance                         */
    /* y = z - H*x                                                          */
    /* S = H*P*H' + R                                                       */
    /************************************************************************/

    // y = z - H*x
    mf16_mul(&y, H, x);
    mf16_sub(&y, &kfm->z, &y);

    // S = H*P*H' + R
    mf16_mul_abat(&S, H, P);               // temp = H*P*H'
    mf16_add(&S, &kfm->R, &S);                // S += R

    /************************************************************************/
    /* Calculate Kalman gain                                                */
    /* K = P*H' * S^-1                                                      */
    /************************************************************************/

    // K = P*H' * S^-1
    mf16_cholesky(&S, &S);
    mf16_invert_lt(&S, &S);               // Sinv = S^-1
    mf16_mul_bt(&temp_PHt, P, H);         // temp = P*H'
    mf16_mul(&K, &temp_PHt, &S);          // K = temp*Sinv

    /************************************************************************/
    /* Correct state prediction                                             */
    /* x = x + K*y                                                          */
    /************************************************************************/

    // x = x + K*y
    mf16_mul_add(x, &K, &y);

    /************************************************************************/
    /* Correct state covariances                                            */
    /* P = (I-K*H) * P                                                      */
    /*   = P - K*(H*P)                                                      */
    /************************************************************************/

    // P = P - K*(H*P)
    mf16_mul(&temp_HP, H, P);                   // temp_HP = H*P
    mf16_mul_sub(P, &K, &temp_HP);              // P -= K*temp_HP
}

void kalman_correct_uc(kalman16_uc_t *kf, kalman16_observation_t *kfm) {
    // just be careful, kid
    kalman_correct((kalman16_t *) kf, kfm);
}

uint_fast8_t acceleration_detected() {
    register fix16_t
        alpha = fix16_abs(fix16_sub(norm3(m_accelerometer.x, m_accelerometer.y, m_accelerometer.z), F16(1)));
    if (alpha < attitude_threshold) {
        return 0;
    }

    return 1;
}

void fusion_fastpredict_X(kalman16_uc_t *const kf, const register fix16_t deltaT) {
    mf16 *const x = kalman_get_state_vector_uc(kf);

    /*
        Transition matrix layout:

        A_rp = [0 0 0,     0  Cn3 -Cn2;
                0 0 0,  -Cn3    0  Cn1;
                0 0 0,   Cn2 -Cn1    0;

                0 0 0,     0 0 0;
                0 0 0,     0 0 0;
                0 0 0,     0 0 0];
    */

    // fetch estimated DCM components
    register const fix16_t c1 = x->data[0][0];
    register const fix16_t c2 = x->data[1][0];
    register const fix16_t c3 = x->data[2][0];

    // fetch estimated angular velocities
    register const fix16_t gx = x->data[3][0];
    register const fix16_t gy = x->data[4][0];
    register const fix16_t gz = x->data[5][0];

    // solve differential equations
    register const fix16_t
        d_c1 = fix16_sub(fix16_mul(c3, gy), fix16_mul(c2, gz)); //    0*gx  +   c3*gy  + (-c2*gz) = c3*gy - c2*gz
    register const fix16_t
        d_c2 = fix16_sub(fix16_mul(c1, gz), fix16_mul(c3, gx)); // (-c3*gx) +    0*gy  +   c1*gz  = c1*gz - c3*gx
    register const fix16_t
        d_c3 = fix16_sub(fix16_mul(c2, gx), fix16_mul(c1, gy)); //   c2*gx  + (-c1*gy) +    0*gz  = c2*gx - c1*gy

    // integrate
    x->data[0][0] = fix16_add(c1, fix16_mul(d_c1, deltaT));
    x->data[1][0] = fix16_add(c2, fix16_mul(d_c2, deltaT));
    x->data[2][0] = fix16_add(c3, fix16_mul(d_c3, deltaT));

    // keep constant.
    x->data[3][0] = gx;
    x->data[4][0] = gy;
    x->data[5][0] = gz;
}

void fusion_sanitize_state(kalman16_uc_t *const kf) {
    mf16 *const A = &kf->A;
    mf16 *const x = &kf->x;

    // fetch axes
    fix16_t c1 = x->data[0][0];
    fix16_t c2 = x->data[1][0];
    fix16_t c3 = x->data[2][0];

    // calculate vector norm
    fix16_t norm = norm3(c1, c2, c3);

    // normalize vectors
    c1 = fix16_div(c1, norm);
    c2 = fix16_div(c2, norm);
    c3 = fix16_div(c3, norm);

    // re-set to state and state matrix
    x->data[0][0] = c1;
    x->data[1][0] = c2;
    x->data[2][0] = c3;
}

void fusion_update_attitude_gyro(register const fix16_t deltaT) {
    /************************************************************************/
    /* Prepare measurement                                                  */
    /************************************************************************/
    {
        mf16 *const z = &kfm_gyro.z;

        matrix_set(z, 0, 0, m_gyroscope.x);
        matrix_set(z, 1, 0, m_gyroscope.y);
        matrix_set(z, 2, 0, m_gyroscope.z);
    }

    /************************************************************************/
    /* Perform Kalman update                                                */
    /************************************************************************/

    kalman_correct_uc(&kf_attitude, &kfm_gyro);

    /************************************************************************/
    /* Re-orthogonalize and update state matrix                             */
    /************************************************************************/

    fusion_sanitize_state(&kf_attitude);
}

void tune_measurement_noise(kalman16_observation_t *const kfm) {
    mf16 *const R = &kfm->R;

    matrix_set(R, 0, 0, fix16_mul(initial_r_axis, alpha1));
    matrix_set(R, 1, 1, fix16_mul(initial_r_axis, alpha1));
    matrix_set(R, 2, 2, fix16_mul(initial_r_axis, alpha1));

    matrix_set(R, 3, 3, fix16_mul(initial_r_gyro, alpha2));
    matrix_set(R, 4, 4, fix16_mul(initial_r_gyro, alpha2));
    matrix_set(R, 5, 5, fix16_mul(initial_r_gyro, alpha2));
}

void fusion_update_attitude(register const fix16_t deltaT) {
    /************************************************************************/
    /* Perform acceleration detection                                       */
    /************************************************************************/

    if (acceleration_detected()) {
        fusion_update_attitude_gyro(deltaT);
        return;
    }

    /************************************************************************/
    /* Prepare measurement                                                  */
    /************************************************************************/
    {
        mf16 *const z = &kfm_accel.z;

        fix16_t norm = v3d_norm(&m_accelerometer);

        matrix_set(z, 0, 0, fix16_div(m_accelerometer.x, norm));
        matrix_set(z, 1, 0, fix16_div(m_accelerometer.y, norm));
        matrix_set(z, 2, 0, fix16_div(m_accelerometer.z, norm));

        matrix_set(z, 3, 0, m_gyroscope.x);
        matrix_set(z, 4, 0, m_gyroscope.y);
        matrix_set(z, 5, 0, m_gyroscope.z);
    }

    /************************************************************************/
    /* Prepare noise                                                        */
    /************************************************************************/

    tune_measurement_noise(&kfm_accel);

    /************************************************************************/
    /* Perform Kalman update                                                */
    /************************************************************************/

    kalman_correct_uc(&kf_attitude, &kfm_accel);

    /************************************************************************/
    /* Re-orthogonalize and update state matrix                             */
    /************************************************************************/

    fusion_sanitize_state(&kf_attitude);
}

void magnetometer_project(fix16_t *const mx, fix16_t *const my, fix16_t *const mz) {
    const mf16 *const x = kalman_get_state_vector_uc(&kf_attitude);

    register const fix16_t acc_x = x->data[0][0];
    register const fix16_t acc_y = x->data[1][0];
    register const fix16_t acc_z = x->data[2][0];

    /************************************************************************/
    /* Instead of tilt corrected magnetometer, use TRIAD algorithm          */
    /************************************************************************/

    // calculate cross product for C1
    // m = cross([m_magnetometer.x m_magnetometer.y m_magnetometer.z], [m_accelerometer.x m_accelerometer.y m_accelerometer.z])
    // -->
    //      mx = m_magnetometer.y*m_accelerometer.z - m_magnetometer.z*m_accelerometer.y
    //      my = m_magnetometer.z*m_accelerometer.x - m_magnetometer.x*m_accelerometer.z
    //      mz = m_magnetometer.x*m_accelerometer.y - m_magnetometer.y*m_accelerometer.x
    *mx = fix16_sub(fix16_mul(m_magnetometer.y, acc_z), fix16_mul(m_magnetometer.z, acc_y));
    *my = fix16_sub(fix16_mul(m_magnetometer.z, acc_x), fix16_mul(m_magnetometer.x, acc_z));
    *mz = fix16_sub(fix16_mul(m_magnetometer.x, acc_y), fix16_mul(m_magnetometer.y, acc_x));

    // normalize C1
    const register fix16_t norm = norm3(*mx, *my, *mz);
    *mx = fix16_div(*mx, norm);
    *my = fix16_div(*my, norm);
    *mz = fix16_div(*mz, norm);
}

static void fusion_update_orientation_gyro(register const fix16_t deltaT) {
    /************************************************************************/
    /* Prepare measurement                                                  */
    /************************************************************************/
    {
        mf16 *const z = &kfm_gyro.z;

        matrix_set(z, 0, 0, m_gyroscope.x);
        matrix_set(z, 1, 0, m_gyroscope.y);
        matrix_set(z, 2, 0, m_gyroscope.z);
    }

    /************************************************************************/
    /* Perform Kalman update                                                */
    /************************************************************************/

    kalman_correct_uc(&kf_orientation, &kfm_gyro);

    /************************************************************************/
    /* Re-orthogonalize and update state matrix                             */
    /************************************************************************/

    fusion_sanitize_state(&kf_orientation);
}

static void fusion_update_orientation(register const fix16_t deltaT) {
    /************************************************************************/
    /* Calculate metrics required for update                                */
    /************************************************************************/
    fix16_t mx, my, mz;
    magnetometer_project(&mx, &my, &mz);

#if 0
    // check for singularity
    if (cos_pitch < singularity_cos_threshold)
    {
        fusion_update_orientation_gyro(deltaT);
        return;
    }
#endif

    /************************************************************************/
    /* Prepare noise                                                        */
    /************************************************************************/

    tune_measurement_noise(&kfm_magneto);
    {
        mf16 *const R = &kfm_magneto.R;

        // anyway, overwrite covariance of projection
        matrix_set(R, 0, 0, fix16_mul(initial_r_projection, alpha1));
        matrix_set(R, 1, 1, fix16_mul(initial_r_projection, alpha1));
        matrix_set(R, 2, 2, fix16_mul(initial_r_projection, alpha1));
    }

    /************************************************************************/
    /* Prepare measurement                                                  */
    /************************************************************************/
    {
        mf16 *const z = &kfm_magneto.z;

        matrix_set(z, 0, 0, mx);
        matrix_set(z, 1, 0, my);
        matrix_set(z, 2, 0, mz);

#if 1
        matrix_set(z, 3, 0, m_gyroscope.x);
        matrix_set(z, 4, 0, m_gyroscope.y);
        matrix_set(z, 5, 0, m_gyroscope.z);
#else
        matrix_set(z, 3, 0, kf_attitude.x.data[3][0]);
        matrix_set(z, 4, 0, kf_attitude.x.data[4][0]);
        matrix_set(z, 5, 0, kf_attitude.x.data[5][0]);
#endif
    }

    /************************************************************************/
    /* Perform Kalman update                                                */
    /************************************************************************/

    kalman_correct_uc(&kf_orientation, &kfm_magneto);

    /************************************************************************/
    /* Re-orthogonalize and update state matrix                             */
    /************************************************************************/

    fusion_sanitize_state(&kf_orientation);
}

void qf16_div_s(qf16 *dest, const qf16 *q, fix16_t s) {
    dest->a = fix16_div(q->a, s);
    dest->b = fix16_div(q->b, s);
    dest->c = fix16_div(q->c, s);
    dest->d = fix16_div(q->d, s);
}

fix16_t qf16_norm(const qf16 *q) {
    return fa16_norm(&q->a, &q->b - &q->a, 4);
}

void qf16_normalize(qf16 *dest, const qf16 *q) {
    qf16_div_s(dest, q, qf16_norm(q));
}

void calculate_roll_pitch(register fix16_t *const roll, register fix16_t *const pitch) {
    const mf16 *const x = kalman_get_state_vector_uc(&kf_attitude);

    // fetch axes
    fix16_t c31 = x->data[0][0];
    fix16_t c32 = x->data[1][0];
    fix16_t c33 = x->data[2][0];

    // calculate pitch
    *pitch = -fix16_asin(c31);

    // calculate roll
    *roll = -fix16_atan2(c32, -c33);
}

void calculate_yaw(register const fix16_t roll, register const fix16_t pitch, register fix16_t *const yaw) {
    const mf16 *const x2 = kalman_get_state_vector_uc(&kf_orientation);
    const mf16 *const x3 = kalman_get_state_vector_uc(&kf_attitude);

    // fetch axes
    const fix16_t c21 = x2->data[0][0];
    const fix16_t c22 = x2->data[1][0];
    const fix16_t c23 = x2->data[2][0];

    const fix16_t c31 = x3->data[0][0];
    const fix16_t c32 = x3->data[1][0];
    const fix16_t c33 = x3->data[2][0];

    // calculate partial cross product for C11
    // C1  = cross([C21 C22 C23], [C31 C32 C33])
    // -->
    //      C11 = C22*C33 - C23*C32
    //      C12 = C23*C31 - C21*C33
    //      C13 = C21*C32 - C22*C31
    fix16_t c11 = fix16_sub(fix16_mul(c22, c33), fix16_mul(c23, c32));

    // calculate pitch
    *yaw = fix16_atan2(c21, -c11);
}

void fusion_predict(register const fix16_t deltaT) {
    mf16 *const x2 = kalman_get_state_vector_uc(&kf_orientation);
    mf16 *const x3 = kalman_get_state_vector_uc(&kf_attitude);

    mf16 *const P2 = kalman_get_system_covariance_uc(&kf_orientation);
    mf16 *const P3 = kalman_get_system_covariance_uc(&kf_attitude);

    // update state matrix
    update_state_matrix_from_state(&kf_attitude, deltaT);
    update_state_matrix_from_state(&kf_orientation, deltaT);

    // predict state
    fusion_fastpredict_X(&kf_attitude, deltaT);
    fusion_fastpredict_X(&kf_orientation, deltaT);

    // predict covariance
    kalman_predict_P_uc(&kf_attitude);
    kalman_predict_P_uc(&kf_orientation);

    // re-orthogonalize and update state matrix
    fusion_sanitize_state(&kf_attitude);
    fusion_sanitize_state(&kf_orientation);
}

void fusion_update(register const fix16_t deltaT) {
    // perform roll and pitch updates
//    if (true == m_have_accelerometer)
    {
        // bootstrap filter

        fix16_t norm = v3d_norm(&m_accelerometer);

        kf_attitude.x.data[0][0] = fix16_div(m_accelerometer.x, norm);
        kf_attitude.x.data[1][0] = fix16_div(m_accelerometer.y, norm);
        kf_attitude.x.data[2][0] = fix16_div(m_accelerometer.z, norm);

        fusion_update_attitude(deltaT);
    }
//    else
    {
        // perform only rotational update
        fusion_update_attitude_gyro(deltaT);
    }

    // perform yaw updates
//    if (true == m_have_magnetometer)
    {
        // bootstrap filter
        // make sure that the attitude filter was already bootstrapped in order to be able to project the
        // magnetometer readings
//        if ((false == m_orientation_bootstrapped) && (true == m_attitude_bootstrapped))
        {
            fix16_t mx, my, mz;
            magnetometer_project(&mx, &my, &mz);

            kf_orientation.x.data[0][0] = mx;
            kf_orientation.x.data[1][0] = my;
            kf_orientation.x.data[2][0] = mz;

//            m_orientation_bootstrapped = true;
        }

        fusion_update_orientation(deltaT);
    }
//    else
    {
        // perform only rotational update
        fusion_update_orientation_gyro(deltaT);
    }

    // reset information
//    m_have_accelerometer = false;
//    m_have_magnetometer = false;
}

static void fetch_quaternion_opt2(register qf16 *const quat) {
    const register mf16 *const x2 = kalman_get_state_vector_uc(&kf_orientation);
    const register mf16 *const x3 = kalman_get_state_vector_uc(&kf_attitude);

    // m00 = R(1, 1);    m01 = R(1, 2);    m02 = R(1, 3);
    // m10 = R(2, 1);    m11 = R(2, 2);    m12 = R(2, 3);
    // m20 = R(3, 1);    m21 = R(3, 2);    m22 = R(3, 3);

    const fix16_t m10 = x2->data[0][0];
    const fix16_t m11 = x2->data[1][0];
    const fix16_t m12 = x2->data[2][0];

    const fix16_t m20 = -x3->data[0][0];
    const fix16_t m21 = -x3->data[1][0];
    const fix16_t m22 = -x3->data[2][0];

    // calculate cross product for C1
    // m0 = cross([m10 m11 m12], [m20 m21 m22])
    // -->
    //      m00 = m11*m22 - m12*m21
    //      m01 = m12*m20 - m10*m22
    //      m02 = m10*m21 - m11*m20
    fix16_t m00 = fix16_sub(fix16_mul(m11, m22), fix16_mul(m12, m21));
    fix16_t m01 = fix16_sub(fix16_mul(m12, m20), fix16_mul(m10, m22));
    fix16_t m02 = fix16_sub(fix16_mul(m10, m21), fix16_mul(m11, m20));

    // normalize C1
    const register fix16_t norm = norm3(m00, m01, m02);
    m00 = fix16_div(m00, norm);
    m01 = fix16_div(m01, norm);
    m02 = fix16_div(m02, norm);

    // "Angel" code
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

    fix16_t qw, qx, qy, qz;

    // check the matrice's trace
    const register fix16_t trace = fix16_add(m00, fix16_add(m11, m22));
    if (trace > 0) {
        /*
        s = 0.5 / sqrt(trace + 1.0);
        qw = 0.25 / s;
        qx = ( R(3,2) - R(2,3) ) * s;
        qy = ( R(1,3) - R(3,1) ) * s;
        qz = ( R(2,1) - R(1,2) ) * s;
        */

        const fix16_t s = fix16_div(F16(0.5), fix16_sqrt(fix16_add(F16(1.0), trace)));

        qw = fix16_div(F16(0.25), s);
        qx = fix16_mul(fix16_sub(m21, m12), s);
        qy = fix16_mul(fix16_sub(m02, m20), s);
        qz = fix16_mul(fix16_sub(m10, m01), s);
    } else {
        if (m00 > m11 && m00 > m22) {
            /*
            s = 2.0 * sqrt( 1.0 + R(1,1) - R(2,2) - R(3,3));
            qw = (R(3,2) - R(2,3) ) / s;
            qx = 0.25 * s;
            qy = (R(1,2) + R(2,1) ) / s;
            qz = (R(1,3) + R(3,1) ) / s;
            */
            const fix16_t s = fix16_mul(F16(2), fix16_sqrt(fix16_add(F16(1), fix16_sub(m00, fix16_add(m11, m22)))));

            qw = fix16_div(fix16_sub(m21, m12), s);
            qx = fix16_mul(F16(0.25), s);
            qy = fix16_div(fix16_add(m01, m10), s);
            qz = fix16_div(fix16_add(m02, m20), s);
        } else if (m11 > m22) {
            /*
            s = 2.0 * sqrt( 1.0 + R(2,2) - R(1,1) - R(3,3));
            qw = (R(1,3) - R(3,1) ) / s;
            qx = (R(1,2) + R(2,1) ) / s;
            qy = 0.25 * s;
            qz = (R(2,3) + R(3,2) ) / s;
            */
            const fix16_t s = fix16_mul(F16(2), fix16_sqrt(fix16_add(F16(1), fix16_sub(m11, fix16_add(m00, m22)))));

            qw = fix16_div(fix16_sub(m02, m20), s);
            qx = fix16_div(fix16_add(m01, m10), s);
            qy = fix16_mul(F16(0.25), s);
            qz = fix16_div(fix16_add(m12, m21), s);
        } else {
            /*
            s = 2.0 * sqrt( 1.0 + R(3,3) - R(1,1) - R(2,2) );
            qw = (R(2,1) - R(1,2) ) / s;
            qx = (R(1,3) + R(3,1) ) / s;
            qy = (R(2,3) + R(3,2) ) / s;
            qz = 0.25 * s;
            */
            const fix16_t s = fix16_mul(F16(2), fix16_sqrt(fix16_add(F16(1), fix16_sub(m22, fix16_add(m00, m11)))));

            qw = fix16_div(fix16_sub(m10, m01), s);
            qx = fix16_div(fix16_add(m02, m20), s);
            qy = fix16_div(fix16_add(m12, m21), s);
            qz = fix16_mul(F16(0.25), s);
        }
    }

    // compose quaternion
    quat->a = qw;
    quat->b = qx;
    quat->c = qy;
    quat->d = qz;

    // normalizify
    qf16_normalize(quat, quat);
}

void fusion_fetch_quaternion(register qf16 *const quat) {
    fetch_quaternion_opt2(quat);
}

void fusion_fetch_angles(register fix16_t *const roll, register fix16_t *const pitch, register fix16_t *const yaw) {
    // calculate roll and pitch
    calculate_roll_pitch(roll, pitch);

    // calculate yaw
    calculate_yaw(*roll, *pitch, yaw);
}

void get_sensors() {
    m_magnetometer.x = magnetometer.x;
    m_magnetometer.y = magnetometer.y;
    m_magnetometer.z = magnetometer.z;
    m_accelerometer.x = imu.x_acll;
    m_accelerometer.y = imu.y_acll;
    m_accelerometer.z = imu.z_acll;
    m_gyroscope.x = imu.x_gyro;
    m_gyroscope.y = imu.y_gyro;
    m_gyroscope.z = imu.z_gyro;
}

/**
 * @brief Returns the current system time in milliseconds
 * @return The time.
 */
//static inline uint32_t systemTime() {
//    return SystemMilliseconds;
//}
