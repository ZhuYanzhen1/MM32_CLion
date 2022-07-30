/**************************************************************************/ /**
    \file       sensor_decode.h
    \brief      sensor_decode function Header File
    \author     ZGL
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/

#ifndef MM32F3277_DEVICE_SENSOR_DECODE_H_
#define MM32F3277_DEVICE_SENSOR_DECODE_H_

/* The number of bytes sent by read_mcu to proc_mcu in a packet */
#define READ_MCU_AMOUNT         96
/* Number of bytes sent by proc_mcu to ctrl_mcu in a packet */
#define PROC_MCU_SEND_AMOUNT    21
/* Number of bytes in a packet received by ctrl_mcu from proc_mcu */
#define CTRL_MCU_RECEIVE_AMOUNT 21

/* Splitting a short type into two char types */
#define SHORT_SPLIT_CHAR(x, y)  packets[x] = ((buffer[y] & 0x0000ff00) >> 8);\
                                packets[(x)+1] = (buffer[y] & 0x000000ff);
/* Splitting an int data into four char data */
#define INT_SPLIT_CHAR(x, y)    packets[x] = ((buffer[y] & 0x00ff0000) >> 16);\
                                packets[(x)+1] = ((buffer[y] & 0x0000ff00) >> 8);\
                                packets[(x)+2] = (buffer[y] & 0x000000ff);
/* Divide a float data into four char type data */
#define FLOAT_SPLIT_CHAR(x, y)  packets[x] = ((buffer[y] & 0xff000000) >> 24);\
                                packets[(x)+1] = ((buffer[y] & 0x00ff0000) >> 16);\
                                packets[(x)+2] = ((buffer[y] & 0x0000ff00) >> 8);\
                                packets[(x)+3] = (buffer[y] & 0x000000ff);

/* Merge two char data into one short data type */
#define DECODE_TO_SHORT(x, y)   (x) = (int)(((int) packets[y] <<8) | \
                                (packets[y+1])) ;
/* Merge four char types into one int type */
#define DECODE_TO_INT(x, y)     (x) = (int) (((int) packets[y] <<24) | \
                                ((int)packets[y+1] <<16) |\
                                ((int)packets[y+2])<<8 |\
                                ((int)packets[y+3]));
/* Merge four char types into one float type */
#define DECODE_TO_FLOAT(x, y)   (temp) = (unsigned int)( ((int)packets[y] <<24) |\
                                ((int) packets[y+1] <<16) |\
                                ((int) packets[y+2] <<8) |\
                                ((int) packets[y+3]));\
                                (x) = *((float *) (&temp));

/* Processed sensor information */
typedef struct {
    float north;
    float chebyshev_north;
    short ax;
    short ay;
    short az;
    short checksum;
} decode_fixed;

/* Integral amount of processed sensors */
typedef struct {
    float north;
    float chebyshev_north;
    float ax;
    float ay;
    float az;
    short checksum;
    short num;  // 接收的数量
} decode_fixed_sum;

/* Sensor raw data and matrix associated with geomagnetic correction */
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
    int num;    // 收到第几个数
    short ax;
    short ay;
    short az;
    int checksum;
} decode_debug;

/* The amount of state of the car */
typedef struct {
    float distance_north;
    float distance_east;
    float north_angle;
    float v;
    short checksum;
} decode_proc;

extern decode_fixed_sum small_packets_sum;
extern decode_fixed small_packets;
extern decode_debug debug_data;
extern volatile decode_proc proc_data;
extern unsigned char uart2_dma_buffer_size;

void precossing_proc_to_control(unsigned int packets[PROC_MCU_SEND_AMOUNT], const unsigned int *buffer);
void unpacking_fixed_length_data(unsigned int packets[10]);
void unpacking_variable_length_data(unsigned int *packets);
void unpacking_proc_to_control(unsigned int packets[PROC_MCU_SEND_AMOUNT - 2]);
void deal_uart6_dma_proc(const unsigned int *p);
void deal_dma_read_mcu(const unsigned int *p);

#endif // MM32F3277_DEVICE_SENSOR_DECODE_H_
