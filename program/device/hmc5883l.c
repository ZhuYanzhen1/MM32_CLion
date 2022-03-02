/**************************************************************************/ /**
    \file     hmc5883l.c
    \brief    hmc5883l function Source File
    \author   ZGL
    \version  V1.0.1
    \date     22. February 2022
******************************************************************************/

#include "hmc5883l.h"
#include "iic.h"
#include "delay.h"
#include "qfplib.h"
#include "kalman.h"

hmc5883l magnetometer = {0};
hmc_correction magnetometer_correction = {0};

void hmc5883l_config() {
    iic_start();
    iic_send_byte(0x3c);
    iic_send_byte(0x00);
    iic_send_byte(0x14);  //输出速率75hz,MAX 0x78

    delayms(2);
    iic_start();
    iic_send_byte(0x3c); //写指令
    iic_send_byte(0x01);
    iic_send_byte(0x00); //测量范围

    delayms(2);
    iic_start();
    iic_send_byte(0x3c); //写指令
    iic_send_byte(0x02);
    iic_send_byte(0x00); //连续测量模式
    iic_stop();

    iic_read_hmc5883l_verification();
}

void iic_read_hmc5883l() {
    short xyz_data[6] = {0};

    iic_start();
    iic_send_byte(0x3c);// 0x3c是写信号 ；ox3d是读信号
    iic_send_byte(0x03);// 地址位
//    iic_stop();

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
    iic_send_byte(0x3c);// 0x3c是写信号 ；ox3d是读信号
    iic_send_byte(0x0a);// 地址位

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

// 单位：毫高斯
void hmc5883l_correction() {
    /* Zero-Deflection Correction */
    magnetometer_correction.x = (float) magnetometer.x * FACTOR_MAGNETOMETER_MGS + 29.5794438f;
    magnetometer_correction.y = (float) magnetometer.y * FACTOR_MAGNETOMETER_MGS + 29.5794438f;
    magnetometer_correction.z = (float) magnetometer.z * FACTOR_MAGNETOMETER_MGS + 29.5794438f;

//    magnetometer_correction.x = qfp_fadd(qfp_fmul((float) magnetometer.x, FACTOR_MAGNETOMETER_MGS), 29.5794438f);
//    magnetometer_correction.y = qfp_fadd(qfp_fmul((float) magnetometer.y, FACTOR_MAGNETOMETER_MGS), 116.3653953f);
//    magnetometer_correction.z = qfp_fadd(qfp_fmul((float) magnetometer.z, FACTOR_MAGNETOMETER_MGS), -12.0118083f);

    /* Ellipsoid correction */ // 右乘
    float stretch_matrix[3][3] = {0.9990217f, 0.0157003f, 0.0176384f,
                                  0.0157003f, 0.9833404f, 0.0071299f,
                                  0.0176384f, 0.0071299f, 1.0185523f};
//    magnetometer_correction.x = qfp_fadd(qfp_fadd(
//                                             qfp_fmul(magnetometer_correction.x, stretch_matrix[0][0]),
//                                             qfp_fmul(magnetometer_correction.y, stretch_matrix[1][0])),
//                                         qfp_fmul(magnetometer_correction.z, stretch_matrix[2][0]));
//    magnetometer_correction.y = qfp_fadd(qfp_fadd(
//                                             qfp_fmul(magnetometer_correction.x, stretch_matrix[0][1]),
//                                             qfp_fmul(magnetometer_correction.y, stretch_matrix[1][1])),
//                                         qfp_fmul(magnetometer_correction.z, stretch_matrix[2][1]));
//    magnetometer_correction.z = qfp_fadd(qfp_fadd(
//                                             qfp_fmul(magnetometer_correction.x, stretch_matrix[0][2]),
//                                             qfp_fmul(magnetometer_correction.y, stretch_matrix[1][2])),
//                                         qfp_fmul(magnetometer_correction.z, stretch_matrix[2][2]));

    magnetometer_correction.x =
        magnetometer_correction.x * stretch_matrix[0][0] + magnetometer_correction.y * stretch_matrix[1][0] +
            magnetometer_correction.z * stretch_matrix[2][0];
    magnetometer_correction.y =
        magnetometer_correction.x * stretch_matrix[0][1] + magnetometer_correction.y * stretch_matrix[1][1] +
            magnetometer_correction.z * stretch_matrix[2][1];
    magnetometer_correction.z =
        magnetometer_correction.x * stretch_matrix[0][2] + magnetometer_correction.y * stretch_matrix[1][2] +
            magnetometer_correction.z * stretch_matrix[2][2];

}

/* b:机体坐标系；n：东北天 */
float ture_north_begin() {
    float initial_ture_north;
    for (unsigned char i = 0; i < AVERAGE_NUM_TURE_NORTH; i++) {
        iic_read_hmc5883l();
        hmc5883l_correction();
        initial_ture_north = qfp_fadd(initial_ture_north,
                                      qfp_fadd(qfp_fmul(qfp_fatan2(magnetometer_correction.y,
                                                                   magnetometer_correction.x),
                                                        qfp_fdiv(180, PI)), 180));
    }
    initial_ture_north = qfp_fdiv(initial_ture_north, AVERAGE_NUM_TURE_NORTH);
    return initial_ture_north;
}
//        angle = (atan2(Mag[1],Mag[0])*(180 / pi)+180);

