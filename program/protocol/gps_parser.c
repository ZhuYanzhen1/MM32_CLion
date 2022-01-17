//
// Created by 16625 on 2021-12-24.
//

#include "gps_parser.h"

#define STRING_TO_NUM(x, y, num)    posx = nmea_comma_position(p, num);\
                                    if (posx != 0XFF) \
                                        (x) = nmea_str2num(p + posx, &(y));
#define STRING_TO_STR(x, num)       posx = nmea_comma_position(p, num);\
                                    if (posx != 0XFF) \
                                        (x) = *(p + posx);
#define STRING_TO_NUM_CHAR(x, num)  posx = nmea_comma_position(p, num);\
                                    if (posx != 0XFF) \
                                        (x) = (char)nmea_str2num(p + posx, &decimal_places);

//
//TODO 优化思路：1、找逗号的算法，可以先一次找齐所有逗号，记下位置；2、乘法和加法运算看看能不能改成位运算；3、高效的字符串转数字算法；
//

/*!
    \brief      Get the location of all commas in the gps packet at once
    \param[in]  buffer: Digital storage area
    \param[in]  comma: Store the array at the location of the comma
    \param[in]  n: Number of commas to find
    \retval     0~0XFE,represents the offset of the comma location.
                0XFF means there is no nth comma
    \note       The return value type cannot be char, because when the desired target is not found,
                the return 0xff is greater than the range of char
*/
void nmea_all_comma_position(char *buffer, char *comma, char n) {
    char *p = buffer;
    unsigned char i = 0;
    while (n) {
        if (*buffer == '*' || *buffer < ' ' || *buffer > 'z')
            return;
        if (*buffer == ',') {
            comma[i++] = (buffer - p);
            n--;
        }
        buffer++;
    }
}

/*!
    \brief      Get the position of the nth comma from inside buffer
    \param[in]  buffer: Digital storage area
    \retval     0~0XFE,represents the offset of the comma location.
                0XFF means there is no nth comma
    \note       The return value type cannot be char, because when the desired target is not found,
                the return 0xff is greater than the range of char
*/
unsigned char nmea_comma_position(char *buffer, char n) {
    char *p = buffer;
    while (n) {
        /* If '*' or illegal character is encountered, there is no nth comma */
        if (*buffer == '*' || *buffer < ' ' || *buffer > 'z')
            return 0XFF;
        if (*buffer == ',')
            n--;
        buffer++;
    }

    /* return difference */
    return (buffer - p);
}

/*!
    \brief      m^n
    \param[in]  m
    \param[in]  n
    \retval     m^n
*/
int nmea_pow(char m, char n) {
    int result = 1;
    if (m == 0)
        return 0;
    while (n > 0) {
        result *= m;
        n--;
    }
    return result;
}

/*!
    \brief      Character to number conversion,end with ',' or '*'
    \param[in]  buffer: Digital storage area
    \param[in]  decimal_places: Number of decimal places,return to the calling function
    \retval     Converted values
*/
int nmea_str2num(char *buffer, char *decimal_places) {
    char *p = buffer;
    int integer_data = 0, decimal_data = 0, data;
    char integer_length = 0, decimal_length = 0, mask = 0;

    /* Get the length of integers and decimals */
    while (1) {
        if (*p == '-') { /* Negative numbers */
            mask |= 0X02;
            p++;
        }
        if (*p == ',' || (*p == '*'))
            break;
        if (*p == '.') { /* Decimal */
            mask |= 0X01;
            p++;
        } else if (*p > '9' || (*p < '0')) { /* There are illegal characters */
            integer_length = 0;
            decimal_length = 0;
            break;
        }
        if (mask & 0X01)
            decimal_length++;
        else
            integer_length++;
        p++;
    }

    /* Remove negative sign */
    if (mask & 0X02)
        buffer++;

    /* Get the integer part of the data */

    for (unsigned char i = 0; i < integer_length; i++)
        integer_data = integer_data * 10 + (buffer[i] - '0');

    /* Maximum 4 decimal places */
    if (decimal_length > 4)
        decimal_length = 4;
    *decimal_places = decimal_length;

    /* Get the decimal part of the data */

    for (unsigned char i = 0; i < decimal_length; i++)
        decimal_data = decimal_data * 10 + (buffer[integer_length + 1 + i] - '0');

    data = integer_data * nmea_pow(10, decimal_length) + decimal_data;
    if (mask & 0X02) data = -data;
    return data;

//    while(*p>='0'&&*p<='9')
//    {num=num*10+(*p-'0');
//        p++;
//    }
}

/*!
    \brief      get checksum
    \param[in]  buffer: Digital storage area
    \retval     checksum
*/
unsigned char nmea_get_checksum(char *buffer) {
    while (*buffer++ != '*');
    int checksum = (buffer[0] - (buffer[0] > 58 ? '7' : '0')) * 16 + (buffer[1] - (buffer[1] > 58 ? '7' : '0'));
    return checksum;
}

/*!
    \brief      Convert the latitude and longitude read by gps to int format of hours, minutes and seconds
    \param[in]  degree: Longitude or Latitude
    \param[in]  decimal_places: Fractional digits of latitude and longitude
*/
void change_latitude_longitude_format(int *degree, char decimal_places) {
    int fractional_part = nmea_pow(10, decimal_places);
    int second_latitude = (*degree % fractional_part) * 60;
    *degree = (*degree / fractional_part) * 100000 + second_latitude;
}

/*!
    \brief      Analyze GPRMC information
    \param[in]  buffer: Digital storage area
    \param[in]  gps_rmc: Recommended minimum positioning information
                time, status, latitude, longitude, speed, direction, positioning mode
*/
void nmea_gnrmc_analysis(nmea_rmc *gps_rmc, char *buffer) {
    char i = 0, decimal_places = 1, posx = 0, check_sum = 0;
    char *p = buffer;

    /* If the number of "," is not enough, it means receiving error */
    posx = nmea_comma_position(p, 13);
    if (posx != 0XFF)
        gps_rmc->checksum = nmea_get_checksum(buffer);
    else
        return;

    /* Verify that the checksum are correct */
    while (p[i] != '*')
        check_sum ^= p[i++];
    if (check_sum != gps_rmc->checksum)
        return;

    STRING_TO_NUM(gps_rmc->positioning_time.uct_time, gps_rmc->positioning_time.decimal_places_time, 1)
    STRING_TO_STR(gps_rmc->status, 2)
    STRING_TO_NUM(gps_rmc->latitude, gps_rmc->decimal_places_latitude, 3)
    STRING_TO_STR(gps_rmc->latitude_direction, 4)
    STRING_TO_NUM(gps_rmc->longitude, gps_rmc->decimal_places_longitude, 5)
    STRING_TO_STR(gps_rmc->longitude_direction, 6)
    STRING_TO_NUM(gps_rmc->speed_to_ground_section, gps_rmc->decimal_places_speed, 7)
    STRING_TO_NUM(gps_rmc->direction_of_ground_truth, gps_rmc->decimal_places_direction, 8)
    STRING_TO_NUM(gps_rmc->date, decimal_places, 9)
    STRING_TO_STR(gps_rmc->mode, 12)

    /* Processing Dimension */
    change_latitude_longitude_format(&gps_rmc->latitude, gps_rmc->decimal_places_latitude);
    change_latitude_longitude_format(&gps_rmc->longitude, gps_rmc->decimal_places_longitude);

}

static unsigned char status = 0;
static unsigned char package_buffer[80];
static unsigned char package_counter = 0;
nmea_rmc gps_rmc = {0};

void deal_dma_gnrmc() {
    unsigned char *p = choose_buffer();
    for (unsigned char counter = 0; counter < 74; ++counter) {
        switch (status) {
            case 0:if (p[counter] == '$') status = 1;
                break;
            case 1:
                if (p[counter] == '\r') status = 2;
                else if (counter == 73) {
                    status = 3;
                    goto copy_data_to_package_buffer;
                } else {
                        copy_data_to_package_buffer:
                    package_buffer[package_counter] = p[counter];
                    package_counter++;
                }
                break;
            case 2:nmea_gnrmc_analysis(&gps_rmc, (char *) package_buffer);
                package_counter = 0;
                for (unsigned char p_counter = 0; p_counter < 80; ++p_counter)
                    package_buffer[package_counter] = 0x00;
                status = 0;
                break;
            case 3:
                if (p[counter] == '\r') status = 2;
                else if (counter == 73) {
                    status = 5;
                    goto copy_data_to_package_buffer;
                } else goto copy_data_to_package_buffer;
                break;
            case 5:
                if (p[counter] == '\r') status = 2;
                else goto copy_data_to_package_buffer;
                break;
        }
    }
}

//
//TODO 1:消除速度的静态误差，速度保留到小数点后两位；2：用DMA代替串口中断（接收）
//
