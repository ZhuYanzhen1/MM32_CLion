//
// Created by 16625 on 2021-12-24.
//

#include "gps_praser.h"
#include "main.h"

/*!
    \brief      Get the position of the nth comma from inside buffer
    \param[in]  buffer: Digital storage area
    \retval     0~0XFE,represents the offset of the comma location.
                0XFF means there is no nth comma
*/
char nmea_comma_position(char *buffer, char n) {
    char *p = buffer;
    while (n) {

        /* If '*' or illegal character is encountered, there is no nth comma */
        if (*buffer == '*' || *buffer < ' ' || *buffer > 'z') return 0XFF;
        if (*buffer == ',') n--;
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
    while (n--) {
        result *= m;
    }
    while (n < 0) {
        result /= m;
        n++;
    }
    return result;
}

/*!
    \brief      Character to number conversion,end with ',' or '*'
    \param[in]  buffer: Digital storage area
    \param[in]  decimal_places: Number of decimal places,return to the calling function
    \retval     Converted values()
*/
int nmea_str2num(char *buffer) {
    char *p = buffer;
    int integer_data = 0, decimal_data = 0;
    char integer_length = 0, decimal_length = 0, i;
    char mask = 0;
    int data;

    /* Get the length of integers and decimals */
    while (1) {
        if (*p == '-') { /* Negative numbers */
            mask |= 0X02;
            p++;
        }
        if (*p == ',' || (*p == '*'))break;
        if (*p == '.') { /* Decimal */
            mask |= 0X01;
            p++;
        } else if (*p > '9' || (*p < '0')) { /* There are illegal characters */
            integer_length = 0;
            decimal_length = 0;
            break;
        }
        if (mask == 0X01) decimal_length++;
        else integer_length++;
        p++;
    }

    /* Remove negative sign */
    if (mask == 0X02) buffer++;

    /* Get the integer part of the data */
    for (i = 0; i < integer_length; i++) {
        integer_data += nmea_pow(10, integer_length - 1 - i) * (buffer[i] - '0');
    }

    /* Maximum 5 decimal places */
    if (decimal_length > 5) decimal_length = 5;

    /* Get the decimal part of the data */
    for (i = 0; i < decimal_length; i++) {
        decimal_data += nmea_pow(10, decimal_length - 1 - i) * (buffer[integer_length + 1 + i] - '0');
    }
    data = integer_data + decimal_data * nmea_pow(10, -decimal_length);
    if (mask == 0X02) data = -data;
    return data;
}

//分析GPGGA信息
//gpsx:nmea信息结构体
//buf:接收到的GPS数据缓冲区首地址
void NMEA_GPGGA_Analysis(nmea_gga *gpsx, char *buf) {
    char *p1;
    char posx;

    /* strstr determines whether $GPGGA is a substring of the p array, and if so, returns the address of the first occurrence in $GPGGA */
    p1 = (char *) strstr((const char *) buf, "$GPGGA");
    posx = nmea_comma_position(p1, 6);                              //得到GPS状态
    if (posx != 0XFF)gpsx->positioning_quality = nmea_str2num(p1 + posx);
    posx = nmea_comma_position(p1, 7);                              //得到用于定位的卫星数
    if (posx != 0XFF)gpsx->positioning_satellites_num = nmea_str2num(p1 + posx);
    posx = nmea_comma_position(p1, 9);                              //得到海拔高度
    if (posx != 0XFF)gpsx->altitude = nmea_str2num(p1 + posx);
}
