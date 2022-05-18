/**************************************************************************/ /**
    \file       sensor_decode.c
    \brief      sensor_decode function Source File
    \author     ZGL
    \version    V1.3.2
    \date       09. March 2022
******************************************************************************/

#include "sensor_decode.h"
#include "verification.h"

decode_fixed_sum small_packets_sum = {0};
decode_fixed small_packets = {0};
decode_debug debug_data = {0};
decode_proc proc_data = {0};
volatile unsigned char lqr_flag = 0;

/*!
    \brief      Packets with fixed data length
    \param[out] packets: Processes packets as long as they are sent through the serial port
    \param[in]  buffer: Data to be sent,includes 5 short type variables and a check digit
*/
void precossing_proc_to_control(unsigned int packets[PROC_MCU_SEND_AMOUNT], const unsigned int *buffer) {
    // 包头
    packets[0] = 0xff;
    packets[PROC_MCU_SEND_AMOUNT - 5] = 0x00;  // 前8个信息字节的调整位
    packets[PROC_MCU_SEND_AMOUNT - 4] = 0x00;  // 后8个信息字节的调整位

    FLOAT_SPLIT_CHAR(1, 0);
    FLOAT_SPLIT_CHAR(5, 1);
    FLOAT_SPLIT_CHAR(9, 2);
    FLOAT_SPLIT_CHAR(13, 3);

    // 调整位，从高到低，每一位与一个字节的数据对应，如果为1，那就代表相应的数据为0xff
    for (unsigned char i = 1; i < 9; ++i) {
        if (packets[i] == 0xff) {
            packets[i] = 0x00;
            packets[PROC_MCU_SEND_AMOUNT - 5] |= 0x80 >> (i - 1);
        }
    }
    for (unsigned char i = 9; i < 18; ++i) {
        if (packets[i] == 0xff) {
            packets[i] = 0x00;
            packets[PROC_MCU_SEND_AMOUNT - 4] |= 0x80 >> (i - 9);
        }
    }

    // 校验位
    packets[PROC_MCU_SEND_AMOUNT - 2] = verification_crc8(&packets[1], PROC_MCU_SEND_AMOUNT - 3);
    // 包尾
    packets[PROC_MCU_SEND_AMOUNT - 1] = 0xff;
}

/*!
    \brief      Solving packets with fixed data length
    \param[in]  packets: Packets to be solved. Excluding the head and tail of the package
    \note       The package to be solved does not contain the head and tail parts of the package
*/
void unpacking_proc_to_control(unsigned int packets[PROC_MCU_SEND_AMOUNT - 2]) {
    unsigned int temp;
    volatile short checksum = verification_crc8((unsigned int *) packets, PROC_MCU_SEND_AMOUNT - 3);
    proc_data.checksum = (short) packets[PROC_MCU_SEND_AMOUNT - 3];
    if (checksum != proc_data.checksum) return;
    // 调整位恢复原始数据，和校验的顺序不能换，因为封包的时候是先算校验，再计算调整位
    for (unsigned char i = 0; i < 8; ++i)
        if (packets[PROC_MCU_SEND_AMOUNT - 5] & (0x80 >> i))
            packets[i] = 0xff;
    for (unsigned char i = 8; i < 16; ++i)
        if (packets[PROC_MCU_SEND_AMOUNT - 4] & (0x80 >> i))
            packets[i] = 0xff;

    DECODE_TO_FLOAT(proc_data.distance_north, 0)    // 如果发送的是float类型，得先转换成unsigned int的地址传进来
    DECODE_TO_FLOAT(proc_data.distance_east, 4)
    DECODE_TO_FLOAT(proc_data.north_angle, 8)
    DECODE_TO_FLOAT(proc_data.v, 12)
//    unsigned int tmp_float_int = *((unsigned int *) (&a));
//    float a = *((float *) (&tmp_float_int));
    lqr_flag = 1;
}

/*!
    \brief      Solving packets with fixed data length
    \param[in]  packets: Packets to be solved
    \note       The package to be solved does not contain the head and tail parts of the package
*/
void unpacking_fixed_length_data(unsigned int packets[10]) {
    short checksum = verification_crc8((unsigned int *) packets, 9);
    small_packets.checksum = (short) packets[9];
    if (checksum != small_packets.checksum) return;
    // 调整位恢复原始数据，和校验的顺序不能换，因为封包的时候是先算校验，再计算调整位
    for (unsigned char i = 0; i < 8; ++i)
        if (packets[8] & (0x80 >> i))
            packets[i] = 0xff;

    small_packets.north = (float) ((short) (packets[0] | (((short) packets[5] & 0xf0) << 4))) / 10.0f;
    small_packets.chebyshev_north = (float) ((short) (packets[1] | (((short) packets[5] & 0x0f) << 8))) / 10.0f;
    small_packets.ax = (short) (packets[2] | (((short) packets[6] & 0xf0) << 4));
    small_packets.ay = (short) (packets[3] | ((short) (packets[6] & 0x0f) << 8));
    small_packets.az = (short) (packets[4] | ((short) (packets[7] & 0xf0) << 4));

    //先解出来并拿到东西，再去把传感器3轴信息-2048
    small_packets.ax -= 2048;
    small_packets.ay -= 2048;
    small_packets.az -= 2048;

    // small_packets.chebyshev_north 实际上是指向南，要将其修正为北
    small_packets.chebyshev_north += 180;
    small_packets.chebyshev_north = (small_packets.chebyshev_north > 360.0f) ? (small_packets.chebyshev_north - 360)
                                                                             : small_packets.chebyshev_north;

//    small_packets_sum.ax += small_packets.ax;  没用到
    small_packets_sum.ay += (float) small_packets.ay;
//    small_packets_sum.az += small_packets.az;  没用到

    small_packets_sum.num++;
}

/*!
    \brief      Packets with variable data length
    \param[out] packets: Processes packets as long as they are sent through the serial port
    \note
*/
void unpacking_variable_length_data(unsigned int *packets) {
//    debug_data.ax = (int) ((int) (packets[0] & 0x00ff0000) | (int) (packets[1] & 0x0000ff00) |
//        (packets[2] & 0x000000ff));

    unsigned short checksum = verification_crc16((unsigned int *) packets, 64);
    DECODE_TO_SHORT(debug_data.checksum, 64)    //
    if (checksum != debug_data.checksum) return;

    DECODE_TO_INT(debug_data.mag_x, 0)
    DECODE_TO_INT(debug_data.mag_y, 4)
    DECODE_TO_INT(debug_data.mag_z, 8)
    DECODE_TO_INT(debug_data.offset_x, 12)
    DECODE_TO_INT(debug_data.offset_y, 16)
    DECODE_TO_INT(debug_data.offset_z, 20)
    DECODE_TO_INT(debug_data.diag_x, 24)
    DECODE_TO_INT(debug_data.diag_y, 28)
    DECODE_TO_INT(debug_data.diag_z, 32)
    DECODE_TO_INT(debug_data.offdiag_x, 36)
    DECODE_TO_INT(debug_data.offdiag_y, 40)
    DECODE_TO_INT(debug_data.offdiag_z, 44)
    DECODE_TO_INT(debug_data.residual, 48)
    DECODE_TO_INT(debug_data.step_length, 52)
    DECODE_TO_SHORT(debug_data.num, 56)
    DECODE_TO_SHORT(debug_data.ax, 58)
    DECODE_TO_SHORT(debug_data.ay, 60)
    DECODE_TO_SHORT(debug_data.az, 62)
}

#ifdef IS_CONTROL_MCU
static unsigned char status = 0;
static unsigned int package_buffer[80];
static unsigned char package_counter = 0;
//static unsigned char package_counter_1 = 0;

void deal_uart6_dma_proc(const unsigned int *p) {
    for (unsigned char counter = 0; counter < CTRL_MCU_RECEIVE_AMOUNT; ++counter) {
        switch (status) {
            case 0:if (p[counter] == 0xff) status = 1;
                package_counter = 0;
                break;
            case 1:
                if (p[counter] == 0xff && package_counter != (PROC_MCU_SEND_AMOUNT - 2)) {
                    status = 1;
                    package_counter = 0;    // 原本没有的
                } else if (p[counter] == 0xff && package_counter == (PROC_MCU_SEND_AMOUNT - 2))
                    status = 2;
                else {
                    package_buffer[package_counter] = p[counter];
                    package_counter++;
                }
                break;
            case 2:unpacking_proc_to_control(package_buffer);
                package_counter = 0;
                status = 1;
                break;
            default:break;
        }
    }
}

#else
static unsigned char status = 0;
static unsigned int package_buffer[80] = {0};
static unsigned char package_counter = 0;
static unsigned char debug_buffer_counter = 0;
unsigned char uart2_dma_buffer_size = 16;

void deal_dma_read_mcu(const unsigned int *p) {
    for (unsigned char counter = 0; counter < uart2_dma_buffer_size; ++counter) {
        switch (status) {
            case 0:
                if (p[counter] == 0xff && uart2_dma_buffer_size == 16) status = 1;
                else if (p[counter] == 0xa5 && uart2_dma_buffer_size == 96) status = 3;
                package_counter = 0;
                break;
            case 1:
                if (p[counter] == 0xff && package_counter != 10) {
                    status = 1;
                    package_counter = 0;
                } else if (p[counter] == 0xff && package_counter == 10)
                    status = 2;
                else if (package_counter > 10)
                    status = 0;
                else {
                    package_buffer[package_counter] = p[counter];
                    package_counter++;
                }
                break;
            case 2:unpacking_fixed_length_data((unsigned int *) package_buffer);
                package_counter = 0;
                status = 0;
                break;
            case 3:
                if (p[counter] == 0x5a)
                    status = 4;
                else
                    status = 0;
                break;
            case 4:debug_buffer_counter = p[counter];
                status = 5;
                break;
            case 5:
                if (debug_buffer_counter == 3)   //
                    status = 6;
                else {
                    package_buffer[package_counter] = p[counter];
                    package_counter++;
                    debug_buffer_counter--;
                }
                break;
            case 6:unpacking_variable_length_data((unsigned int *) package_buffer);
                package_counter = 0;
                status = 0;
                break;
            default:status = 0;
                break;
        }
    }
}
#endif


