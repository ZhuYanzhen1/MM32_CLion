//
// Created by 16625 on 2022-03-09.
//

#include "sensor_decode.h"
#include "verification.h"

decode_fixed small_packets = {0};
decode_debug debug_data = {0};

/*!
    \brief      Packets with fixed data length
    \param[out] packets: Processes packets as long as they are sent through the serial port
    \param[in]  buffer: Data to be sent,includes 5 short type variables and a check digit
*/
void precossing_fixed_length_data(unsigned char packets[12], short *buffer) {
    // 包头
    packets[0] = 0xff;

    // 对加速度的值进行处理，防止负值造成影响，然后在接收的时候再加2048
    buffer[2] += 2048;
    buffer[3] += 2048;
    buffer[4] += 2048;

    // 处理5个数据的低8位
    packets[1] = (unsigned char) (buffer[0] & 0x00ff);
    packets[2] = (unsigned char) (buffer[1] & 0x00ff);
    packets[3] = (unsigned char) (buffer[2] & 0x00ff);
    packets[4] = (unsigned char) (buffer[3] & 0x00ff);
    packets[5] = (unsigned char) (buffer[4] & 0x00ff);

    // 处理5个数据的高4位
    packets[6] = (unsigned char) (((buffer[0] & 0x0f00) >> 4) | ((buffer[1] & 0x0F00) >> 8));
    packets[7] = (unsigned char) (((buffer[2] & 0x0f00) >> 4) | ((buffer[3] & 0x0F00) >> 8));
    packets[8] = (unsigned char) ((buffer[4] & 0x0f00) >> 4);

    // 调整位，从高到低，每一位与一个字节的数据对应，如果为1，那就代表相应的数据为0xff
    for (unsigned char i = 1; i < 9; ++i) {
        if (packets[i] == 0xff) {
            packets[i] = 0;
            packets[9] = 0x80 >> (i - 1);
        }
    }

    packets[10] = verification_crc8(packets + 1, 9);

    // 包尾
    packets[11] = 0xff;
}

/*!
    \brief      Solving packets with fixed data length
    \param[in]  packets: Packets to be solved
*/
void unpacking_fixed_length_data(unsigned char packets[10]) {
    short checksum = verification_crc8(packets, 9);
    small_packets.checksum = (short) packets[9];
    if (checksum != small_packets.checksum) return;
    // 调整位恢复原始数据，和校验的顺序不能换，因为封包的时候是先算校验，再计算调整位
    for (unsigned char i = 0; i < 8; ++i)
        if (packets[8] & (0x80 >> i))
            packets[i] = 0xff;

    small_packets.pitch = (short) (packets[0] | (((short) packets[5] & 0xf0) << 4));
    small_packets.yaw = (short) (packets[1] | (((short) packets[5] & 0x0f) << 8));
    small_packets.mx = (short) (packets[2] | (((short) packets[6] & 0xf0) << 4));
    small_packets.my = (short) (packets[3] | ((short) (packets[6] & 0x0f) << 8));
    small_packets.mz = (short) (packets[4] | ((short) (packets[7] & 0xf0) << 4));

    //先解出来并拿到东西，再去把传感器3轴信息-2048
    small_packets.mx -= 2048;
    small_packets.my -= 2048;
    small_packets.mz -= 2048;

}

/*!
    \brief      Packets with variable data length
    \param[out] packets: Processes packets as long as they are sent through the serial port
    \param[in]  length: Packet data length
    \param[in]  buffer: Data to be sent,includes 5 short type variables and a check digit
    \note
*/
void precossing_variable_length_data(unsigned char *packets, unsigned char length, const int *buffer) {
    // 包头
    packets[0] = 0xa5;
    packets[1] = 0xa5;

    // 数据长度
    packets[2] = length;

    // 地磁计24位，一共3个8位，高位在前，地位在后，顺序xyz
    INT_SPLIT_CHAR(3, 0);
    INT_SPLIT_CHAR(6, 1);
    INT_SPLIT_CHAR(9, 2);

    // 椭球矫正的零偏offset xyz
    FLOAT_SPLIT_CHAR(12, 3)
    FLOAT_SPLIT_CHAR(16, 4)
    FLOAT_SPLIT_CHAR(20, 5)

    // 椭球矫正的拉伸矩阵diag xyz
    FLOAT_SPLIT_CHAR(24, 6)
    FLOAT_SPLIT_CHAR(28, 7)
    FLOAT_SPLIT_CHAR(32, 8)
    // 椭球矫正的拉伸矩阵两边offdiag
    FLOAT_SPLIT_CHAR(36, 9)
    FLOAT_SPLIT_CHAR(40, 10)
    FLOAT_SPLIT_CHAR(44, 11)

    // 橢球校正残差、步长
    FLOAT_SPLIT_CHAR(48, 12)
    FLOAT_SPLIT_CHAR(52, 13)

    // 橢球校正采集数据个数
    SHORT_SPLIT_CHAR(56, 14)

    // 最后一位放校验,先取高位再取低位
    unsigned short checksum = verification_crc16(packets + 3, length - 4);
    packets[length - 1] = (unsigned short) (checksum & 0xff00) >> 8;
    packets[length] = (unsigned short) checksum & 0x00ff;
}

/*!
    \brief      Packets with variable data length
    \param[out] packets: Processes packets as long as they are sent through the serial port
    \param[in]  length: Packet data length
    \param[in]  buffer: Data to be sent,includes 5 short type variables and a check digit
    \note
*/
void unpacking_variable_length_data(unsigned char *packets) {
//    debug_data.mag_x = (int) ((int) (packets[0] & 0x00ff0000) | (int) (packets[1] & 0x0000ff00) |
//        (packets[2] & 0x000000ff));

    unsigned short checksum = verification_crc16(packets, 55);
    DECODE_TO_SHORT(debug_data.checksum, 55)
    if (checksum != debug_data.checksum) return;

    DECODE_TO_INT(debug_data.mag_x, 0)
    DECODE_TO_INT(debug_data.mag_y, 3)
    DECODE_TO_INT(debug_data.mag_z, 6)
    DECODE_TO_float(debug_data.offset_x, 9)
    DECODE_TO_float(debug_data.offset_y, 13)
    DECODE_TO_float(debug_data.offset_z, 17)
    DECODE_TO_float(debug_data.bias_x, 21)
    DECODE_TO_float(debug_data.bias_y, 25)
    DECODE_TO_float(debug_data.bias_z, 29)
    DECODE_TO_float(debug_data.offbias_x, 33)
    DECODE_TO_float(debug_data.offbias_y, 37)
    DECODE_TO_float(debug_data.offbias_z, 41)
    DECODE_TO_float(debug_data.residual, 45)
    DECODE_TO_float(debug_data.step_length, 49)
    DECODE_TO_SHORT(debug_data.num, 53)

}
