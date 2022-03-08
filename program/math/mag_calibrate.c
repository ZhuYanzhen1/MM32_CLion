#include "mag_calibrate.h"
#include "fast_math.h"

#ifdef RUNNING_UNIT_TEST

#include "stdio.h"

#else
#include "printf.h"
#include "delay.h"
#endif

#define OUTPUT_DEBUG_INFO   1

//校正参数，包含hard iron 和soft iron
calpara_t params;

static float sample_buffer[3][COMPASS_CAL_NUM_SAMPLES];
volatile unsigned int samples_collected;

void Initial_CompassCal(calpara_t *calibrate_param) {
    for (int i = 0; i < 3; i++) {
        calibrate_param->offset[i] = 0;
        calibrate_param->diag[i] = 1.0f;
        calibrate_param->offdiag[i] = 0;
    }
    /************需要根据磁强计实际输出核对************/
    calibrate_param->radius = 200;
    samples_collected = 0;
}

//static volatile unava_data_counter = 0;

char accept_sample(float sample[3], float mag_sphere_radius) {
    float min_sample_dist, dist;
    float maxsize = COMPASS_CAL_NUM_SAMPLES;
    float dx, dy, dz;
    int i;

    if (samples_collected == 0) {
        return 1;
    }

    min_sample_dist = fast_absf(4.0f * mag_sphere_radius / fast_sqrt(maxsize)) / 3.0f;
    for (i = 0; i < samples_collected; i++) {
        dx = sample[0] - sample_buffer[0][i];
        dy = sample[1] - sample_buffer[1][i];
        dz = sample[2] - sample_buffer[2][i];

        dist = fast_sqrt(dx * dx + dy * dy + dz * dz);

        if (dist < min_sample_dist) {
//            unava_data_counter++;
//            printf("unava_data_counter:%d\r\n", unava_data_counter);
            return 0;
        }
    }

    return 1;
}

void calc_initial_para(calpara_t *calibrate_param) {
    float avr_value, avr_radius;
    int i, j;

    //旋转一圈各轴采样数据和应为0
    for (i = 0; i < 3; i++) {
        avr_value = 0;
        for (j = 0; j < samples_collected; j++) {
            avr_value -= sample_buffer[i][j];
        }
        avr_value /= (float) samples_collected;
        calibrate_param->offset[i] = avr_value;
    }

    //若环境干扰误差较大，此处可根据理想磁场大小设置固定值
    avr_radius = 0;
    for (i = 0; i < samples_collected; i++) {
        avr_radius += fast_sqrt(sample_buffer[0][i] * sample_buffer[0][i] +
            sample_buffer[1][i] * sample_buffer[1][i] +
            sample_buffer[2][i] * sample_buffer[2][i]);
    }
    avr_radius /= (float) samples_collected;
    calibrate_param->radius = avr_radius;
}

float calc_mean_residual(const float x[], const float y[], const float z[], unsigned int size,
                         float offset_x, float offset_y, float offset_z,
                         float sphere_radius, float diag_x, float diag_y, float diag_z,
                         float offdiag_x, float offdiag_y, float offdiag_z) {
    int k;
    float A, B, C;
    float fitvalue, length, residual;

    fitvalue = 0;
    for (k = 0; k < size; k++) {
        //Calculate Jacobian
        A = (diag_x * (x[k] + offset_x)) + (offdiag_x * (y[k] + offset_y)) + (offdiag_y * (z[k] + offset_z));
        B = (offdiag_x * (x[k] + offset_x)) + (diag_y * (y[k] + offset_y)) + (offdiag_z * (z[k] + offset_z));
        C = (offdiag_y * (x[k] + offset_x)) + (offdiag_z * (y[k] + offset_y)) + (diag_z * (z[k] + offset_z));
        length = fast_sqrt(A * A + B * B + C * C);
        residual = sphere_radius - length;//sphere_radius 为初值

        fitvalue += residual * residual;
    }
    fitvalue = fast_sqrt(fitvalue / (float) size);

    return fitvalue;
}

char InverseMatrix(int dimension, float matrix[]) {
    int column_swap[20];
    int l, k, m;
    float pivot;
    float determinate;
    float swap;

    determinate = 1.0f;

    for (l = 0; l < dimension; l++) {
        column_swap[l] = l;
    }

    for (l = 0; l < dimension; l++) {
        pivot = 0;
        m = l;

        /* Find the element in this row with the largest absolute value -
           the pivot. Pivoting helps avoid division by small quantities. */
        for (k = l; k < dimension; k++) {
            if (fast_absf(pivot) < fast_absf(matrix[dimension * l + k])) {
                m = k;
                pivot = matrix[dimension * l + k];
            }
        }

        /* Swap columns if necessary to make the pivot be the leading
           nonzero element of the row. */
        if (l != m) {
            k = column_swap[m];
            column_swap[m] = column_swap[l];
            column_swap[l] = k;
            for (k = 0; k < dimension; k++) {
                swap = matrix[dimension * k + l];
                matrix[dimension * k + l] = matrix[dimension * k + m];
                matrix[dimension * k + m] = swap;
            }
        }

        /* Divide the row by the pivot, making the leading element
           1.0 and multiplying the determinant by the pivot. */
        matrix[dimension * l + l] = 1.0f;
        determinate = determinate * pivot;   /* Determinant of the matrix. */
        if (fast_absf(determinate) < 1.0E-6) {
            return 0;      /* Pivot = 0 therefore singular matrix. */
        }
        for (m = 0; m < dimension; m++) {
            matrix[dimension * l + m] = matrix[dimension * l + m] / pivot;
        }

        /* Gauss-Jordan elimination.  Subtract the appropriate multiple
           of the current row from all subsequent rows to get the matrix
           into row echelon form. */

        for (m = 0; m < dimension; m++) {
            if (l == m) {
                continue;
            }
            pivot = matrix[dimension * m + l];

            matrix[dimension * m + l] = 0;
            for (k = 0; k < dimension; k++) {
                matrix[dimension * m + k] = matrix[dimension * m + k] - (pivot * matrix[dimension * l + k]);
            }
        }
    }

    /* Swap the columns back into their correct places. */
    for (l = 0; l < dimension; l++) {
        if (column_swap[l] == l) {
            continue;
        }

        /* Find out where the column wound up after column swapping. */
        for (m = l + 1; m < dimension; m++) {
            if (column_swap[m] == l) {
                break;
            }
        }

        column_swap[m] = column_swap[l];
        for (k = 0; k < dimension; k++) {
            pivot = matrix[dimension * l + k];
            matrix[dimension * l + k] = matrix[dimension * m + k];
            matrix[dimension * m + k] = pivot;
        }
        column_swap[l] = l;
    }
    return 1;
}

char run_gn_sphere_fit(const float x[], const float y[], const float z[], unsigned int size,
                       float *offset_x, float *offset_y, float *offset_z,
                       float *sphere_radius, float *diag_x, float *diag_y, float *diag_z,
                       float *offdiag_x, float *offdiag_y, float *offdiag_z) {
    float sphere_jacob[4];
    float JTJ[16], JTFI[4];
    float residual;
    float A, B, C, length;
    float deltax[4], deltavalue;
    int i, j, k, row, col;

    //Initial zeros
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++)
            JTJ[i * 4 + j] = 0;
        JTFI[i] = 0;
    }

    // Gauss Newton迭代
    for (k = 0; k < size; k++) {
        //Calculate Jacobian
        A = (*diag_x * (x[k] + *offset_x)) + (*offdiag_x * (y[k] + *offset_y)) + (*offdiag_y * (z[k] + *offset_z));
        B = (*offdiag_x * (x[k] + *offset_x)) + (*diag_y * (y[k] + *offset_y)) + (*offdiag_z * (z[k] + *offset_z));
        C = (*offdiag_y * (x[k] + *offset_x)) + (*offdiag_z * (y[k] + *offset_y)) + (*diag_z * (z[k] + *offset_z));
        length = fast_sqrt(A * A + B * B + C * C);

        // 0: 对半径导数
        sphere_jacob[0] = 1.0f;
        // 1-3: 对bias x、y、z求导
        sphere_jacob[1] = -1.0f * (((*diag_x * A) + (*offdiag_x * B) + (*offdiag_y * C)) / length);
        sphere_jacob[2] = -1.0f * (((*offdiag_x * A) + (*diag_y * B) + (*offdiag_z * C)) / length);
        sphere_jacob[3] = -1.0f * (((*offdiag_y * A) + (*offdiag_z * B) + (*diag_z * C)) / length);
        residual = *sphere_radius - length;//sphere_radius 为初值

        for (i = 0; i < 4; i++) {
            // compute JTJ
            for (j = 0; j < 4; j++)
                JTJ[i * 4 + j] += sphere_jacob[i] * sphere_jacob[j];
            JTFI[i] += sphere_jacob[i] * residual;
        }
    }

    float fit_params[4] = {*sphere_radius, *offset_x, *offset_y, *offset_z};

    if (!InverseMatrix(4, JTJ)) {
//        printf("Mat can not inverse!!");
        return 0;
    }

    deltavalue = 0;
    for (row = 0; row < 4; row++) {
        deltax[row] = 0;
        for (col = 0; col < 4; col++) {
            deltax[row] -= JTFI[col] * JTJ[row * 4 + col];
        }
        deltavalue += deltax[row] * deltax[row];

        fit_params[row] += deltax[row];
    }

    deltavalue = fast_sqrt(deltavalue);

    calc_mean_residual(x, y, z, size, *offset_x, *offset_y, *offset_z, *sphere_radius,
                       *diag_x, *diag_y, *diag_z, *offdiag_x, *offdiag_y, *offdiag_z);

#if OUTPUT_DEBUG_INFO == 1
    printf("Residual = %lf, Increment = %lf\r\n", residual, deltavalue);
#endif

    *sphere_radius = fit_params[0];
    *offset_x = fit_params[1];
    *offset_y = fit_params[2];
    *offset_z = fit_params[3];

    if (deltavalue < 1e-4)
        return 1;
    else
        return 0;
}

char run_gn_ellipsoid_fit(const float x[], const float y[], const float z[], unsigned int size,
                          float *offset_x, float *offset_y, float *offset_z,
                          float *sphere_radius, float *diag_x, float *diag_y, float *diag_z,
                          float *offdiag_x, float *offdiag_y, float *offdiag_z) {
    float JTJ[81], JTFI[9];
    float ellipsoid_jacob[9];
    float A, B, C, length, residual;
    float deltax[9], deltavalue;
    int i, j, k, row, col;

    //Initial zeros
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++)
            JTJ[i * 9 + j] = 0;
        JTFI[i] = 0;
    }

    // Gauss Newton迭代
    for (k = 0; k < size; k++) {

        //Calculate Jacobian
        A = (*diag_x * (x[k] + *offset_x)) + (*offdiag_x * (y[k] + *offset_y)) + (*offdiag_y * (z[k] + *offset_z));
        B = (*offdiag_x * (x[k] + *offset_x)) + (*diag_y * (y[k] + *offset_y)) + (*offdiag_z * (z[k] + *offset_z));
        C = (*offdiag_y * (x[k] + *offset_x)) + (*offdiag_z * (y[k] + *offset_y)) + (*diag_z * (z[k] + *offset_z));
        length = fast_sqrt(A * A + B * B + C * C);
        residual = *sphere_radius - length;

        // 0-2: 对bias x、y、z求导
        ellipsoid_jacob[0] = -1.0f * (((*diag_x * A) + (*offdiag_x * B) + (*offdiag_y * C)) / length);
        ellipsoid_jacob[1] = -1.0f * (((*offdiag_x * A) + (*diag_y * B) + (*offdiag_z * C)) / length);
        ellipsoid_jacob[2] = -1.0f * (((*offdiag_y * A) + (*offdiag_z * B) + (*diag_z * C)) / length);
        // 3-5: 对diag x、y、z求导
        ellipsoid_jacob[3] = -1.0f * ((x[k] + *offset_x) * A) / length;
        ellipsoid_jacob[4] = -1.0f * ((y[k] + *offset_y) * B) / length;
        ellipsoid_jacob[5] = -1.0f * ((z[k] + *offset_z) * C) / length;
        // 6-8: 对offdiag x、y、z求导
        ellipsoid_jacob[6] = -1.0f * (((y[k] + *offset_y) * A) + ((x[k] + *offset_x) * B)) / length;
        ellipsoid_jacob[7] = -1.0f * (((z[k] + *offset_z) * A) + ((x[k] + *offset_x) * C)) / length;
        ellipsoid_jacob[8] = -1.0f * (((z[k] + *offset_z) * B) + ((y[k] + *offset_y) * C)) / length;

        for (i = 0; i < 9; i++) {
            // compute JTJ
            for (j = 0; j < 9; j++)
                JTJ[i * 9 + j] += ellipsoid_jacob[i] * ellipsoid_jacob[j];
            JTFI[i] += ellipsoid_jacob[i] * residual;
        }
    }

    float fit_params[9] = {*offset_x, *offset_y, *offset_z,
                           *diag_x, *diag_y, *diag_z,
                           *offdiag_x, *offdiag_y, *offdiag_z};

    if (!InverseMatrix(9, JTJ))
        return 0;

    deltavalue = 0;
    for (row = 0; row < 9; row++) {
        deltax[row] = 0;
        for (col = 0; col < 9; col++)
            deltax[row] -= JTFI[col] * JTJ[row * 9 + col];
        deltavalue += deltax[row] * deltax[row];
        fit_params[row] += deltax[row];
    }
    deltavalue = fast_sqrt(deltavalue);

    calc_mean_residual(x, y, z, size, *offset_x, *offset_y, *offset_z, *sphere_radius,
                       *diag_x, *diag_y, *diag_z, *offdiag_x, *offdiag_y, *offdiag_z);
#if OUTPUT_DEBUG_INFO == 1
    printf("Residual = %lf, Increment = %lf\r\n", residual, deltavalue);
#endif

    *offset_x = fit_params[0];
    *offset_y = fit_params[1];
    *offset_z = fit_params[2];
    *diag_x = fit_params[3];
    *diag_y = fit_params[4];
    *diag_z = fit_params[5];
    *offdiag_x = fit_params[6];
    *offdiag_y = fit_params[7];
    *offdiag_z = fit_params[8];

    if (deltavalue < 1e-5)
        return 1;
    else
        return 0;
}

void ellipsoid_fit_least_squares(float x[],
                                 float y[],
                                 float z[],
                                 unsigned int size,
                                 calpara_t *calibrate_param,
                                 float *finalfitness) {
    int max_iterations = 100;
    float offset_x = calibrate_param->offset[0], offset_y = calibrate_param->offset[1],
        offset_z = calibrate_param->offset[2];
    float diag_x = calibrate_param->diag[0], diag_y = calibrate_param->diag[1], diag_z = calibrate_param->diag[2];
    float offdiag_x = calibrate_param->offdiag[0], offdiag_y = calibrate_param->offdiag[1],
        offdiag_z = calibrate_param->offdiag[2];
    float sphere_radius = calibrate_param->radius;
    char stopflag;

    //首先对半径、bias进行优化
#if OUTPUT_DEBUG_INFO == 1
    printf("/************run_gn_sphere_fit***********/\r\n");
#ifndef RUNNING_UNIT_TEST
    delayms(100);
#endif
#endif
    for (int i = 0; i < max_iterations; i++) {
        stopflag = run_gn_sphere_fit(x, y, z, size, &offset_x, &offset_y, &offset_z,
                                     &sphere_radius, &diag_x, &diag_y, &diag_z, &offdiag_x, &offdiag_y, &offdiag_z);
#ifndef RUNNING_UNIT_TEST
        delayms(100);
#endif
        if (stopflag == 1)
            break;
    }

#if OUTPUT_DEBUG_INFO == 1
    //然后对除半径外的所有9个参数进行优化
    printf("/***************run_gn_ellipsoid_fit************/\r\n");
#ifndef RUNNING_UNIT_TEST
    delayms(100);
#endif
#endif
    for (int i = 0; i < max_iterations; i++) {
        stopflag = run_gn_ellipsoid_fit(x, y, z, size, &offset_x, &offset_y, &offset_z,
                                        &sphere_radius, &diag_x, &diag_y, &diag_z,
                                        &offdiag_x, &offdiag_y, &offdiag_z);
#ifndef RUNNING_UNIT_TEST
        delayms(100);
#endif
        if (stopflag == 1)
            break;
    }

#if OUTPUT_DEBUG_INFO == 1
    printf("/******************fit end******************/\r\n");
#ifndef RUNNING_UNIT_TEST
    delayms(100);
#endif
#endif
    *finalfitness = calc_mean_residual(x, y, z, size, offset_x, offset_y, offset_z, sphere_radius,
                                       diag_x, diag_y, diag_z, offdiag_x, offdiag_y, offdiag_z);
#if OUTPUT_DEBUG_INFO == 1
#ifndef RUNNING_UNIT_TEST
    delayms(100);
#endif
    printf("Final fitness = %f\r\n", *finalfitness);
#endif

    //置换回去
    calibrate_param->offset[0] = offset_x;
    calibrate_param->offset[1] = offset_y;
    calibrate_param->offset[2] = offset_z;
    calibrate_param->diag[0] = diag_x;
    calibrate_param->diag[1] = diag_y;
    calibrate_param->diag[2] = diag_z;
    calibrate_param->offdiag[0] = offdiag_x;
    calibrate_param->offdiag[1] = offdiag_y;
    calibrate_param->offdiag[2] = offdiag_z;
    calibrate_param->radius = sphere_radius;
}

char check_calibration_result(calpara_t calibrate_param, float fitness) {
    float tolerance = 15.0f;
    //The maximum measurement range is ~1.9 Ga, the earth field is ~0.6 Ga, so an offset larger than ~1.3 Ga means the mag will saturate in some directions.
    float offset_max = 1300;    //mG
    if ((fitness <= tolerance) &&
        (calibrate_param.radius > 150) && (calibrate_param.radius < 950) &&
        //Earth's magnetic field strength range: 250-850mG
        (fast_absf(calibrate_param.offset[0]) < offset_max) &&
        (fast_absf(calibrate_param.offset[1]) < offset_max) &&
        (fast_absf(calibrate_param.offset[2]) < offset_max) &&
        (calibrate_param.diag[0] > 0.2) && (calibrate_param.diag[0] < 5.0) &&
        (calibrate_param.diag[1] > 0.2) && (calibrate_param.diag[1] < 5.0) &&
        (calibrate_param.diag[2] > 0.2) && (calibrate_param.diag[2] < 5.0) &&
        (fast_absf(calibrate_param.offdiag[0]) < 1.0) &&      //absolute of sine/cosine output cannot be greater than 1
        (fast_absf(calibrate_param.offdiag[1]) < 1.0) &&
        (fast_absf(calibrate_param.offdiag[2]) < 1.0))
        return 1;
    else
        return 2;
}

//磁强计校正主函数
//输入：sample   水平旋转过程中的某个采样数据
//结束标志：0-校正中；1-校正结束,成功；2-校正结束,失败
char CompassCal(float sample[3]) {
    static char cal_state = 0;
    static float mag_sphere_radius = 0;

    char new_sample, calresult = 0;
    float fitness;
    int i;

    //初始化
    if (cal_state == 0) {
        Initial_CompassCal(&params);
        mag_sphere_radius = fast_sqrt(sample[0] * sample[0] + sample[1] * sample[1] + sample[2] * sample[2]);
        cal_state++;
    }

    //采数
    if (cal_state == 1) {
        new_sample = accept_sample(sample, mag_sphere_radius);
        if (new_sample == 1) {
            for (i = 0; i < 3; i++)
                sample_buffer[i][samples_collected] = sample[i];
            samples_collected++;
#if OUTPUT_DEBUG_INFO == 1
            printf("samples_collected = %d\r\n", samples_collected);
#endif
            if (samples_collected == COMPASS_CAL_NUM_SAMPLES)
                cal_state++;
        }
    }

    //拟合优化
    if (cal_state == 2) {
        //优化初始值
        calc_initial_para(&params);
        //优化
        ellipsoid_fit_least_squares(sample_buffer[0], sample_buffer[1], sample_buffer[2],
                                    samples_collected, &params, &fitness);
        //检测结果
        calresult = check_calibration_result(params, fitness);
    }

    return calresult;
}