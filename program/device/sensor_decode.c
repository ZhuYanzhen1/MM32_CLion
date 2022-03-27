/**************************************************************************/ /**
    \file       sensor_decode.c
    \brief      sensor_decode function Source File
    \author     ZGL
    \version    V1.3.2
    \date       09. March 2022
******************************************************************************/

#include "sensor_decode.h"
#include "verification.h"

decode_fixed small_packets = {0};
decode_debug debug_data = {0};
decode_proc proc_data = {0};

/*!
    \brief      Packets with fixed data length
    \param[out] packets: Processes packets as long as they are sent through the serial port
    \param[in]  buffer: Data to be sent,includes 5 short type variables and a check digit
*/
void precossing_proc_to_control(unsigned int packets[PROC_MCU_SEND_AMOUNT], const unsigned int *buffer) {
    // 包头
    packets[0] = 0xff;
    packets[13] = 0x00;  // 调整位

    FLOAT_SPLIT_CHAR(1, 0);
    FLOAT_SPLIT_CHAR(5, 1);
    FLOAT_SPLIT_CHAR(9, 2);

    // 调整位，从高到低，每一位与一个字节的数据对应，如果为1，那就代表相应的数据为0xff
    for (unsigned char i = 1; i < 13; ++i) {
        if (packets[i] == 0xff) {
            packets[i] = 0x00;
            packets[13] |= 0x80 >> (i - 1);
        }
    }

    packets[14] = verification_crc8(&packets[1], 13);
    // 包尾
    packets[15] = 0xff;
}

/*!
    \brief      Solving packets with fixed data length
    \param[in]  packets: Packets to be solved
    \note       The package to be solved does not contain the head and tail parts of the package
*/
void unpacking_proc_to_control(unsigned int packets[PROC_MCU_SEND_AMOUNT - 2]) {
    unsigned int temp;
    short checksum = verification_crc8((unsigned int *) packets, 13);
    small_packets.checksum = (short) packets[13];
    if (checksum != small_packets.checksum) return;
    // 调整位恢复原始数据，和校验的顺序不能换，因为封包的时候是先算校验，再计算调整位
    for (unsigned char i = 0; i < 12; ++i)
        if (packets[12] & (0x80 >> i))
            packets[i] = 0xff;

    DECODE_TO_FLOAT(proc_data.distance_north, 0)    // 如果发送的是float类型，得先转换成unsigned int的地址传进来
    DECODE_TO_FLOAT(proc_data.distance_east, 4)
    DECODE_TO_FLOAT(proc_data.north_angle, 8)
//    unsigned int tmp_float_int = *((unsigned int *) (&a));
//    float a = *((float *) (&tmp_float_int));
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

void deal_dma_proc(const unsigned int *p) {
    for (unsigned char counter = 0; counter < PROC_MCU_SEND_AMOUNT; ++counter) {
        switch (status) {
            case 0:if (p[counter] == 0xff) status = 1;
                break;
            case 1:
                if (p[counter] == 0xff && package_counter != (PROC_MCU_SEND_AMOUNT - 2))
                    status = 1;
                else if (p[counter] == 0xff && package_counter == (PROC_MCU_SEND_AMOUNT - 2))
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

#endif

