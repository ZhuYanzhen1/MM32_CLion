//
// Created by 16625 on 2022-02-25.
//

#include "ahrs.h"
#include "adis16470.h"
#include "hmc5883l.h"
#include "qfplib.h"
#include "kalman.h"

//---------------------------------------------------------------------------------------------------
// Definitions



#define SAMPLE_FREQ 512.0f        // sample frequency in Hz
#define BETA_DEF    0.1f        // 2 * proportional gain

volatile float beta = BETA_DEF;                                // 2 * proportional gain (Kp)
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;    // quaternion of sensor frame relative to auxiliary frame

extern hmc5883l magnetometer;
extern adis16470_t imu;

float roll, yaw, pitch;

// AHRS algorithm update

void MadgwickAHRSupdate_optimization() {
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float hx, hy;
    float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1,
        q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
    float gx = (float) imu.x_gyro, gy = (float) imu.y_gyro, gz = (float) imu.z_gyro;
    float ax = (float) imu.x_acll, ay = (float) imu.y_acll, az = (float) imu.z_acll;
    float mx = (float) magnetometer.x, my = (float) magnetometer.y, mz = (float) magnetometer.x;

    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
    if ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
        MadgwickAHRSupdateIMU();
        return;
    }

    // Rate of change of quaternion from gyroscope
    qDot1 = qfp_fmul(0.5f, qfp_fadd(
        qfp_fadd(qfp_fmul(-q1, gx), qfp_fmul(-q2, gy)),
        qfp_fmul(-q3, gz)));
    qDot2 = qfp_fmul(0.5f, qfp_fadd(
        qfp_fadd(qfp_fmul(q0, gx), qfp_fmul(q2, gz)),
        qfp_fmul(-q3, gy)));
    qDot3 = qfp_fmul(0.5f, qfp_fadd(
        qfp_fadd(qfp_fmul(q0, gy), qfp_fmul(-q1, gz)),
        qfp_fmul(q3, gx)));
    qDot4 = qfp_fmul(0.5f, qfp_fadd(
        qfp_fadd(qfp_fmul(q0, gz), qfp_fmul(q1, gy)),
        qfp_fmul(-q2, gx)));
//    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
//    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
//    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
//    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((imu.x_acll == 0) && (imu.y_acll == 0) && (imu.z_acll == 0))) {

        // Normalise accelerometer measurement
        recipNorm = invSqrt(qfp_fadd(
            qfp_fmul(ax, ax), qfp_fadd(qfp_fmul(ay, ay), qfp_fmul(az, az))));
        ax = qfp_fmul(ax, recipNorm);
        ay = qfp_fmul(ay, recipNorm);
        az = qfp_fmul(az, recipNorm);
//        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
//        ax *= recipNorm;
//        ay *= recipNorm;
//        az *= recipNorm;

        // Normalise magnetometer measurement
        recipNorm = invSqrt(qfp_fadd(
            qfp_fmul(mx, mx), qfp_fadd(qfp_fmul(my, my), qfp_fmul(mz, mz))));
        mx = qfp_fmul(mx, recipNorm);
        my = qfp_fmul(my, recipNorm);
        mz = qfp_fmul(mz, recipNorm);
//        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
//        mx *= recipNorm;
//        my *= recipNorm;
//        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = qfp_fmul(2.0f, qfp_fmul(q0, mx));
        _2q0my = qfp_fmul(2.0f, qfp_fmul(q0, my));
        _2q0mz = qfp_fmul(2.0f, qfp_fmul(q0, mz));
        _2q1mx = qfp_fmul(2.0f, qfp_fmul(q1, mx));
        _2q0 = qfp_fmul(2.0f, q0);
        _2q1 = qfp_fmul(2.0f, q1);
        _2q2 = qfp_fmul(2.0f, q2);
        _2q3 = qfp_fmul(2.0f, q3);
        _2q0q2 = qfp_fmul(2.0f, qfp_fmul(q0, q2));
        _2q2q3 = qfp_fmul(2.0f, qfp_fmul(q2, q3));
        q0q0 = qfp_fmul(q0, q0);
        q0q1 = qfp_fmul(q0, q1);
        q0q2 = qfp_fmul(q0, q2);
        q0q3 = qfp_fmul(q0, q3);
        q1q1 = qfp_fmul(q1, q1);
        q1q2 = qfp_fmul(q1, q2);
        q1q3 = qfp_fmul(q1, q3);
        q2q2 = qfp_fmul(q2, q2);
        q2q3 = qfp_fmul(q2, q3);
        q3q3 = qfp_fmul(q3, q3);


        // Reference direction of Earth's magnetic field
        hx = qfp_fadd(qfp_fadd(
                          qfp_fadd(qfp_fmul(mx, q0q0), qfp_fmul(-_2q0my, q3)),
                          qfp_fadd(qfp_fmul(_2q0mz, q2), qfp_fmul(mx, q1q1))),
                      qfp_fadd(
                          qfp_fadd(qfp_fmul(qfp_fmul(_2q1, my), q2), qfp_fmul(qfp_fmul(_2q1, mz), q3)),
                          qfp_fadd(qfp_fmul(-mx, q2q2), qfp_fmul(-mx, q3q3))));
//      hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
        hy = qfp_fadd(qfp_fadd(
                          qfp_fadd(qfp_fmul(_2q0mx, q3), qfp_fmul(my, q0q0)),
                          qfp_fadd(qfp_fmul(-_2q0mz, q1), qfp_fmul(_2q1mx, q2))),
                      qfp_fadd(
                          qfp_fadd((qfp_fmul(-my, q1q1)), qfp_fmul(my, q2q2)),
                          qfp_fadd(qfp_fmul(qfp_fmul(_2q2, mz), q3), qfp_fmul(-my, q3q3))));
//        hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
        _2bx = qfp_fsqrt(qfp_fadd(qfp_fmul(hx, hx), qfp_fmul(hy, hy)));
        _2bz = qfp_fadd(qfp_fadd(
                            qfp_fadd(qfp_fmul(-_2q0mx, q2), qfp_fmul(_2q0my, q1)),
                            qfp_fadd(qfp_fmul(mz, q0q0), qfp_fmul(_2q1mx, q3))),
                        qfp_fadd(
                            qfp_fadd((qfp_fmul(-mz, q1q1)), qfp_fmul(-mz, q2q2)),
                            qfp_fadd(qfp_fmul(qfp_fmul(_2q2, my), q3), qfp_fmul(mz, q3q3))));
//       _2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 - mz * q2q2 + _2q2 * my * q3 + mz * q3q3;
        _4bx = qfp_fmul(2.0f, _2bx);
        _4bz = qfp_fmul(2.0f, _2bz);
//        _4bx = 2.0f * _2bx;
//        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        s0 = qfp_fadd(
            qfp_fadd(
                qfp_fadd(
                    qfp_fmul(-_2q2, qfp_fadd(qfp_fadd(qfp_fmul(2.0f, q1q3), -_2q0q2), -ax)),
                    qfp_fmul(_2q1, qfp_fadd(qfp_fadd(qfp_fmul(2.0f, q0q1), _2q2q3), -ay))),
                qfp_fadd(
                    qfp_fmul(-_2bz, qfp_fmul(
                        q2,
                        qfp_fmul(_2bx, qfp_fadd(qfp_fadd(0.5f, -q2q2), -q3q3)))),
                    qfp_fadd(qfp_fmul(_2bz, qfp_fadd(q1q3, -q0q2)), -mx))),
            qfp_fadd(
                qfp_fadd(
                    qfp_fmul(qfp_fadd(
                                 qfp_fmul(-_2bx, q3), qfp_fmul(_2bz, q1)),
                             qfp_fmul(_2bx, qfp_fadd(q1q2, -q0q3))),
                    qfp_fmul(_2bz, qfp_fadd(qfp_fadd(q0q1, q2q3), -my))),
                qfp_fmul(_2bx, qfp_fmul(
                    q2, qfp_fadd(
                        qfp_fmul(_2bx, qfp_fadd(q0q2, q1q3)),
                        qfp_fadd(qfp_fmul(_2bz, qfp_fadd(0.5f, qfp_fadd(-q1q1, -q2q2))), -mz)))))
        );
//        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay)
//            - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
//            + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
//            + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = qfp_fadd(
            qfp_fadd(
                qfp_fadd(qfp_fadd(
                             qfp_fmul(_2q3, qfp_fadd(qfp_fadd(qfp_fmul(2.0f, q1q3), -_2q0q2), -ax)),
                             qfp_fmul(_2q0, qfp_fadd(qfp_fadd(qfp_fmul(2.0f, q0q1), _2q2q3), -ay))),
                         qfp_fmul(-4.0f, qfp_fmul(q1,
                                                  qfp_fadd(qfp_fadd(qfp_fadd(1, qfp_fmul(-2.0f, q1q3)),
                                                                    qfp_fmul(-2.0f, q2q2)),
                                                           -az)))),
                qfp_fadd(qfp_fmul(_2bz, qfp_fmul(
                             q3,
                             qfp_fadd(
                                 qfp_fmul(_2bx, qfp_fadd(0.5f, qfp_fadd(-q2q2, -q3q3))),
                                 qfp_fadd(qfp_fmul(_2bz, qfp_fadd(q1q3, -q0q2)), -mx)))),
                         qfp_fmul(qfp_fadd(qfp_fmul(_2bx, q2), qfp_fmul(_2bz, q0)),
                                  qfp_fmul(_2bx,
                                           qfp_fadd(qfp_fadd(
                                                        qfp_fmul(_2bx, qfp_fadd(q1q2, -q0q3)),
                                                        qfp_fmul(_2bz, qfp_fadd(q0q1, -q2q3))),
                                                    -my))))),
            qfp_fmul(qfp_fadd(qfp_fmul(_2bx, q3), qfp_fmul(-_4bz, q1)),
                     qfp_fmul(_2bx,
                              qfp_fadd(qfp_fadd(
                                           qfp_fmul(_2bx, qfp_fadd(q0q2, q1q3)),
                                           qfp_fmul(_2bz, qfp_fadd(qfp_fadd(0.5f, -q1q1), -q2q2))),
                                       -mz))));

//        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay)
//            - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az)
//            + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
//            + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
//            + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);

        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay)
            - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az)
            + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
            + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
            + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);

        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay)
            + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
            + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
            + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude

        s0 = qfp_fmul(s0, recipNorm);
        s1 = qfp_fmul(s1, recipNorm);
        s2 = qfp_fmul(s2, recipNorm);
        s3 = qfp_fmul(s3, recipNorm);
//        s0 *= recipNorm;
//        s1 *= recipNorm;
//        s2 *= recipNorm;
//        s3 *= recipNorm;

        // Apply feedback step
        qDot1 = qfp_fadd(qDot1, qfp_fmul(beta, s0));
        qDot2 = qfp_fadd(qDot2, qfp_fmul(beta, s0));
        qDot3 = qfp_fadd(qDot3, qfp_fmul(beta, s0));
        qDot4 = qfp_fadd(qDot4, qfp_fmul(beta, s0));
//        qDot1 -= beta * s0;
//        qDot2 -= beta * s1;
//        qDot3 -= beta * s2;
//        qDot4 -= beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    q0 = qfp_fadd(q0, qfp_fmul(qDot1, qfp_fdiv(1.0f, SAMPLE_FREQ)));
    q1 = qfp_fadd(q1, qfp_fmul(qDot2, qfp_fdiv(1.0f, SAMPLE_FREQ)));
    q2 = qfp_fadd(q2, qfp_fmul(qDot3, qfp_fdiv(1.0f, SAMPLE_FREQ)));
    q3 = qfp_fadd(q3, qfp_fmul(qDot4, qfp_fdiv(1.0f, SAMPLE_FREQ)));
//    q0 += qDot1 * (1.0f / SAMPLE_FREQ);
//    q1 += qDot2 * (1.0f / SAMPLE_FREQ);
//    q2 += qDot3 * (1.0f / SAMPLE_FREQ);
//    q3 += qDot4 * (1.0f / SAMPLE_FREQ);

    // Normalise quaternion
    recipNorm = invSqrt(qfp_fadd(qfp_fadd(qfp_fmul(q0, q0), qfp_fmul(q1, q1)),
                                 qfp_fadd(qfp_fmul(q2, q2), qfp_fmul(q3, q3))));
    q0 = qfp_fmul(q0, recipNorm);
    q1 = qfp_fmul(q1, recipNorm);
    q2 = qfp_fmul(q2, recipNorm);
    q3 = qfp_fmul(q3, recipNorm);
//    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
//    q0 *= recipNorm;
//    q1 *= recipNorm;
//    q2 *= recipNorm;
//    q3 *= recipNorm;

}

void MadgwickAHRSupdate() {
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float hx, hy;
    float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1,
        q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
    float gx = (float) imu.x_gyro * FACTOR_GYRO, gy = (float) imu.y_gyro * FACTOR_GYRO,
        gz = (float) imu.z_gyro * FACTOR_GYRO;
    float ax = (float) imu.x_acll * FACTOR_ALLC, ay = (float) imu.y_acll * FACTOR_ALLC,
        az = (float) imu.z_acll * FACTOR_ALLC;
    float mx = (float) magnetometer.x * FACTOR_MAGNETOMETER_MGS, my = (float) magnetometer.y * FACTOR_MAGNETOMETER_MGS,
        mz = (float) magnetometer.x * FACTOR_MAGNETOMETER_MGS;


    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
    if ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
        MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az);
        return;
    }

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Normalise magnetometer measurement
        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0f * q0 * mx;
        _2q0my = 2.0f * q0 * my;
        _2q0mz = 2.0f * q0 * mz;
        _2q1mx = 2.0f * q1 * mx;
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _2q0q2 = 2.0f * q0 * q2;
        _2q2q3 = 2.0f * q2 * q3;
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        // Reference direction of Earth's magnetic field
        hx =
            mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
        hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
        _2bx = qfp_fsqrt(hx * hx + hy * hy);
        _2bz =
            -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay)
            - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
            + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
            + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay)
            - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az)
            + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
            + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
            + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax)
            + _2q3 * (2.0f * q0q1 + _2q2q3 - ay)
            - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az)
            + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
            + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my)
            + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay)
            + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx)
            + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3)
                + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    q0 += qDot1 * (1.0f / SAMPLE_FREQ);
    q1 += qDot2 * (1.0f / SAMPLE_FREQ);
    q2 += qDot3 * (1.0f / SAMPLE_FREQ);
    q3 += qDot4 * (1.0f / SAMPLE_FREQ);

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    pitch = my_asinf(2 * (q1 * q2 - q1 * q3));
    roll = qfp_fatan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
    yaw = qfp_fatan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3));

    pitch *= ARC_TO_ANGLE;
    roll *= ARC_TO_ANGLE;
    yaw *= ARC_TO_ANGLE;

}

//---------------------------------------------------------------------------------------------------
// IMU algorithm update

void MadgwickAHRSupdateIMU() {
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1, q2q2, q3q3;
    float gx = (float) imu.x_gyro * FACTOR_GYRO, gy = (float) imu.y_gyro * FACTOR_GYRO,
        gz = (float) imu.z_gyro * FACTOR_GYRO;
    float ax = (float) imu.x_acll * FACTOR_ALLC, ay = (float) imu.y_acll * FACTOR_ALLC,
        az = (float) imu.z_acll * FACTOR_ALLC;

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _4q0 = 4.0f * q0;
        _4q1 = 4.0f * q1;
        _4q2 = 4.0f * q2;
        _8q1 = 8.0f * q1;
        _8q2 = 8.0f * q2;
        q0q0 = q0 * q0;
        q1q1 = q1 * q1;
        q2q2 = q2 * q2;
        q3q3 = q3 * q3;

        // Gradient decent algorithm corrective step
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    q0 += qDot1 * (1.0f / SAMPLE_FREQ);
    q1 += qDot2 * (1.0f / SAMPLE_FREQ);
    q2 += qDot3 * (1.0f / SAMPLE_FREQ);
    q3 += qDot4 * (1.0f / SAMPLE_FREQ);

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    pitch = my_asinf(2 * (q1 * q2 - q1 * q3));
    roll = qfp_fatan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
    yaw = qfp_fatan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3));

    pitch *= ARC_TO_ANGLE;
    roll *= ARC_TO_ANGLE;
    yaw *= ARC_TO_ANGLE;

    (void) pitch;
    (void) roll;
    (void) yaw;
}

int instability_fix = 1;

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x) {
    if (instability_fix == 0) {
        /* original code */
        float halfx = 0.5f * x;
        float y = x;
        long i = *(long *) &y;
        i = 0x5f3759df - (i >> 1);
        y = *(float *) &i;
        y = y * (1.5f - (halfx * y * y));
        return y;
    } else if (instability_fix == 1) {
        /* close-to-optimal  method with low cost from http://pizer.wordpress.com/2008/10/12/fast-inverse-square-root */
        unsigned int i = 0x5F1F1412 - (*(unsigned int *) &x >> 1);
        float tmp = *(float *) &i;
        return tmp * (1.69000231f - 0.714158168f * x * tmp * tmp);
    } else {
        /* optimal but expensive method: */
        return 1.0f / qfp_fsqrt(x);
    }
}

float my_asinf(float x) {
    return qfp_fatan2(x, qfp_fsqrt(1.0f - x * x));
}//求x的反正弦值。
