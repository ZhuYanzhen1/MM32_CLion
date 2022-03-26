/**************************************************************************/ /**
    \file       sensor_decode.h
    \brief      sensor_decode function Header File
    \author     ZGL
    \version    V1.3.2
    \date       09. March 2022
******************************************************************************/

#ifndef MM32F3277_DEVICE_SENSOR_DECODE_H_
#define MM32F3277_DEVICE_SENSOR_DECODE_H_

#define READ_MCU_AMOUNT         158
#define PROC_MCU_SEND_AMOUNT    16
#define SHORT_SPLIT_CHAR(x, y)  packets[x] = ((buffer[y] & 0x0000ff00) >> 8);\
                                packets[(x)+1] = (buffer[y] & 0x000000ff);
#define INT_SPLIT_CHAR(x, y)    packets[x] = ((buffer[y] & 0x00ff0000) >> 16);\
                                packets[(x)+1] = ((buffer[y] & 0x0000ff00) >> 8);\
                                packets[(x)+2] = (buffer[y] & 0x000000ff);
#define FLOAT_SPLIT_CHAR(x, y)  packets[x] = ((buffer[y] & 0xff000000) >> 24);\
                                packets[(x)+1] = ((buffer[y] & 0x00ff0000) >> 16);\
                                packets[(x)+2] = ((buffer[y] & 0x0000ff00) >> 8);\
                                packets[(x)+3] = (buffer[y] & 0x000000ff);
#define DECODE_TO_SHORT(x, y)   (x) = (int)(((int) packets[y] <<8) | \
                                (packets[y+1])) ;
#define DECODE_TO_INT(x, y)     (x) = (int) (((int) packets[y] <<24) | \
                                ((int)packets[y+1] <<16) |\
                                ((int)packets[y+2])<<8 |\
                                ((int)packets[y+3]));
#define DECODE_TO_FLOAT(x, y)   (temp) = (unsigned int)( ((int)packets[y] <<24) |\
                                ((int) packets[y+1] <<16) |\
                                ((int) packets[y+2] <<8) |\
                                ((int) packets[y+3]));\
                                (x) = *((float *) (&temp));
/* MSB
 * high:4bit
 * low:8bit
 * xy：x占高四位；y占低四位
 * */
typedef struct {
    float north;
    float chebyshev_north;
    short ax;
    short ay;
    short az;
    short checksum;
} decode_fixed;

typedef struct {
    int mag_x;
    int mag_y;
    int mag_z;
    int offset_x;
    int offset_y;
    int offset_z;
    int diag_x;
    int diag_y;
    int diag_z;
    int offdiag_x;
    int offdiag_y;
    int offdiag_z;
    int residual;
    int step_length;    // 步长
    int num;    // 收到第几个数据
    int checksum;
} decode_debug;

typedef struct {
    float distance_north; // 先发北再发东
    float distance_east;
    float north_angle;
} decode_proc;

extern decode_fixed small_packets;
extern decode_debug debug_data;
extern decode_proc proc_data;

void precossing_proc_to_control(unsigned int packets[PROC_MCU_SEND_AMOUNT], const unsigned int *buffer);
void unpacking_fixed_length_data(unsigned int packets[10]);
void unpacking_variable_length_data(unsigned int *packets);
void unpacking_proc_to_control(unsigned int packets[PROC_MCU_SEND_AMOUNT - 2]);
void deal_dma_proc(const unsigned int *p);

#endif // MM32F3277_DEVICE_SENSOR_DECODE_H_
