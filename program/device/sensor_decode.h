//
// Created by 16625 on 2022-03-09.
//

#ifndef MM32F3277_DEVICE_SENSOR_DECODE_H_
#define MM32F3277_DEVICE_SENSOR_DECODE_H_

#define SHORT_SPLIT_CHAR(x, y)  packets[x] = (char) ((buffer[y] & 0x0000ff00) >> 8);\
                                packets[(x)+1] = (char) (buffer[y] & 0x000000ff);
#define INT_SPLIT_CHAR(x, y)    packets[x] = (char) ((buffer[y] & 0x00ff0000) >> 16);\
                                packets[(x)+1] = (char) ((buffer[y] & 0x0000ff00) >> 8);\
                                packets[(x)+2] = (char) (buffer[y] & 0x000000ff);
#define FLOAT_SPLIT_CHAR(x, y)  packets[x] = (char) ((buffer[y] & 0xff000000) >> 24);\
                                packets[(x)+1] = (char) ((buffer[y] & 0x00ff0000) >> 16);\
                                packets[(x)+2] = (char) ((buffer[y] & 0x0000ff00) >> 8);\
                                packets[(x)+3] = (char) (buffer[y] & 0x000000ff);
#define DECODE_TO_SHORT(x, y)   (x) = (int)(((int) packets[y] <<8) | \
                                (packets[y+1])) ;
#define DECODE_TO_INT(x, y)     (x) = (int) (((int) packets[y] <<16) | \
                                ((int)packets[y+1] <<8) |\
                                ((int)packets[y+2]));
#define DECODE_TO_float(x, y)   (x) = (int) ( ((int)packets[y] <<24) |\
                                ((int) packets[y+1] <<16) |\
                                ((int) packets[y+2] <<8) |\
                                ((int) packets[y+3]));
/* MSB
 * high:4bit
 * low:8bit
 * xy：x占高四位；y占低四位
 * */
typedef struct {
    short pitch;
    short yaw;
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
    int bias_x;
    int bias_y;
    int bias_z;
    int offbias_x;
    int offbias_y;
    int offbias_z;
    int residual;
    int step_length;
    int num;    // 收到第几个数据
    int checksum;
} decode_debug;

void precossing_fixed_length_data(unsigned char packets[12], short *buffer);
void unpacking_fixed_length_data(unsigned char packets[10]);
void precossing_variable_length_data(unsigned char *packets, unsigned char length, const int *buffer);
void unpacking_variable_length_data(unsigned char *packets);

#endif //MAIN_C_DEVICE_SENSOR_DECODE_H_
