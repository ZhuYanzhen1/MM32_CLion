/**************************************************************************/ /**
    \file     hmc5883l.c
    \brief    hmc5883l function Source File
    \author   ZGL
    \version  V1.3.2
    \date     06. March 2021
******************************************************************************/

#include "hmc5883l.h"
#include "iic.h"
#include "delay.h"
#include "qfplib.h"
#include "data_conversion.h"

hmc5883l magnetometer = {0};
hmc_correction magnetometer_correction = {0};

void hmc5883l_config() {
    iic_start();

    /* Write command, address, function */
    iic_send_byte(0x3c);
    iic_send_byte(0x00);
    iic_send_byte(0x14);

    delayms(2);
    iic_start();
    iic_send_byte(0x3c);
    iic_send_byte(0x01);
    iic_send_byte(0x00);

    delayms(2);
    iic_start();
    iic_send_byte(0x3c);
    iic_send_byte(0x02);
    iic_send_byte(0x00);
    iic_stop();

    iic_read_hmc5883l_verification();
}

void iic_read_hmc5883l() {
    short xyz_data[6] = {0};
    iic_start();

    /*  0x3c is the write signal; ox3d is the read signal */
    iic_send_byte(0x3c);

    /* Address bits */
    iic_send_byte(0x03);
    delayus(100);

    iic_start();
    iic_send_byte(0x3d);
    for (unsigned char i = 0; i < 6; i++) {
        xyz_data[i] = iic_read_byte();
        if (i == 5) {
            iic_not_ack();
        } else {
            iic_ack();
        }

    }
    iic_stop();

    /* The read order is xzy */
    magnetometer.x = (short) ((xyz_data[0] << 8) | xyz_data[1]);
    magnetometer.y = (short) ((xyz_data[4] << 8) | xyz_data[5]);
    magnetometer.z = (short) ((xyz_data[2] << 8) | xyz_data[3]);
}

unsigned char iic_read_hmc5883l_verification() {
    unsigned char num_a, num_b, num_c;

    iic_start();
    iic_send_byte(0x3c);
    iic_send_byte(0x0a);

    delayms(1);

    iic_start();
    iic_send_byte(0x3d);
    num_a = iic_read_byte();
    iic_ack();
    num_b = iic_read_byte();
    iic_ack();
    num_c = iic_read_byte();
    iic_not_ack();
    iic_stop();

    if (num_a == 'H' && num_b == '4' && num_c == '3')
        return 0;
    else
        return 1;
}

/*!
    \brief  Ellipsoidal correction and zero bias correction of the data obtained from hmc5883l
    \note   This function uses the floating point optimization in qfplib.h,
            which may not work properly if ported to other microcontrollers,
            The commented out content is the code before optimization
*/
void hmc5883l_correction() {
    /* Zero-Deflection Correction */
//    magnetometer_correction.x = (float) magnetometer.x * FACTOR_MAGNETOMETER_MGS + 29.5794438f;
//    magnetometer_correction.y = (float) magnetometer.y * FACTOR_MAGNETOMETER_MGS + 29.5794438f;
//    magnetometer_correction.z = (float) magnetometer.z * FACTOR_MAGNETOMETER_MGS + 29.5794438f;

    magnetometer_correction.x = qfp_fadd(qfp_fmul((float) magnetometer.x, FACTOR_MAGNETOMETER_MGS), 29.5794438f);
    magnetometer_correction.y = qfp_fadd(qfp_fmul((float) magnetometer.y, FACTOR_MAGNETOMETER_MGS), 116.3653953f);
    magnetometer_correction.z = qfp_fadd(qfp_fmul((float) magnetometer.z, FACTOR_MAGNETOMETER_MGS), -12.0118083f);

    /* Ellipsoid correction */ // 右乘
    float stretch_matrix[3][3] = {0.9990217f, 0.0157003f, 0.0176384f,
                                  0.0157003f, 0.9833404f, 0.0071299f,
                                  0.0176384f, 0.0071299f, 1.0185523f};
    magnetometer_correction.x = qfp_fadd(qfp_fadd(
                                             qfp_fmul(magnetometer_correction.x, stretch_matrix[0][0]),
                                             qfp_fmul(magnetometer_correction.y, stretch_matrix[1][0])),
                                         qfp_fmul(magnetometer_correction.z, stretch_matrix[2][0]));
    magnetometer_correction.y = qfp_fadd(qfp_fadd(
                                             qfp_fmul(magnetometer_correction.x, stretch_matrix[0][1]),
                                             qfp_fmul(magnetometer_correction.y, stretch_matrix[1][1])),
                                         qfp_fmul(magnetometer_correction.z, stretch_matrix[2][1]));
    magnetometer_correction.z = qfp_fadd(qfp_fadd(
                                             qfp_fmul(magnetometer_correction.x, stretch_matrix[0][2]),
                                             qfp_fmul(magnetometer_correction.y, stretch_matrix[1][2])),
                                         qfp_fmul(magnetometer_correction.z, stretch_matrix[2][2]));

//    magnetometer_correction.x =
//        magnetometer_correction.x * stretch_matrix[0][0] + magnetometer_correction.y * stretch_matrix[1][0] +
//            magnetometer_correction.z * stretch_matrix[2][0];
//    magnetometer_correction.y =
//        magnetometer_correction.x * stretch_matrix[0][1] + magnetometer_correction.y * stretch_matrix[1][1] +
//            magnetometer_correction.z * stretch_matrix[2][1];
//    magnetometer_correction.z =
//        magnetometer_correction.x * stretch_matrix[0][2] + magnetometer_correction.y * stretch_matrix[1][2] +
//            magnetometer_correction.z * stretch_matrix[2][2];

}

/*!
    \brief  At power-up, the approximate position of the true north angle
            is calculated using a geomagnetometer to reduce the number of iterations of Kalman fusion
    \retval True North Angle
    \note   The commented out content is the pre-optimized floating point operation
*/
float initial_angle_begin() {
    float initial_angle;
    for (unsigned char i = 0; i < AVERAGE_NUM_TURE_NORTH; i++) {
        iic_read_hmc5883l();
        hmc5883l_correction();
        initial_angle = qfp_fadd(initial_angle, qfp_fadd(
            qfp_fmul(qfp_fatan2(magnetometer_correction.y, magnetometer_correction.x),
                     qfp_fdiv(180, PI)), 180));
//         initial_angle += (atan2(magnetometer_correction.y,agnetometer_correction.x)*(180 / pi)+180);
    }
    initial_angle = qfp_fdiv(initial_angle, AVERAGE_NUM_TURE_NORTH);
    return initial_angle;
}
