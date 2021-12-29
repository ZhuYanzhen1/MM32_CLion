//
// Created by 16625 on 2021-12-24.
//

#include "gps_parser.h"
#include "string.h"

#define STRING_TO_NUM(x, y, num)    posx = nmea_comma_position(p, num);\
                                    if (posx != 0XFF) \
                                        x = nmea_str2num(p + posx, &y)

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
    if (m == 0) {
        return 0;
    }
    int result = 1;
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
long nmea_str2num(char *buffer, char *decimal_places) {
    char *p = buffer;
    int integer_data = 0, decimal_data = 0;
    char integer_length = 0, decimal_length = 0;
    char mask = 0;
    long data = 0;

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
        if (mask & 0X01) decimal_length++;
        else integer_length++;
        p++;
    }

    /* Remove negative sign */
    if (mask & 0X02) buffer++;

    /* Get the integer part of the data */
    for (int i = 0; i < integer_length; i++)
        integer_data += nmea_pow(10, integer_length - 1 - i) * (buffer[i] - '0');

    /* Maximum 4 decimal places */
    if (decimal_length > 4) decimal_length = 4;
    *decimal_places = decimal_length;

    /* Get the decimal part of the data */
    for (int i = 0; i < decimal_length; i++)
        decimal_data += nmea_pow(10, decimal_length - 1 - i) * (buffer[integer_length + 1 + i] - '0');
    data = integer_data * nmea_pow(10, decimal_length) + decimal_data;
    if (mask & 0X02) data = -data;
    return data;
}

/*!
    \brief      get checksum
    \param[in]  buffer: Digital storage area
    \retval     checksum
*/
int nmea_get_checksum(char *buffer) {
    int checksum = 0;
    while (*buffer++ != '*');
    checksum = (buffer[0] - (buffer[0] > 58 ? '7' : '0')) * 16 + (buffer[1] - (buffer[1] > 58 ? '7' : '0'));
    return checksum;
}

/*!
    \brief      Analyze GPGGA information
    \param[in]  buffer: Digital storage area
    \param[in]  gps_gga: Receiver time, location and positioning related data
    \retval     0：Receive gps information error
                1：Receive gps information without error
*/
char nmea_gpgga_analysis(nmea_gga *gps_gga, char *buffer) {
    char *p;
    char posx;

    /* Variables a and decimal_places are not used */
    char a = 1;
    char *decimal_places = &a;

    /* check_sum is calculated, checksum is read from gps */
    int check_sum = 0;

    /* strstr determines whether $GPGGA is a substring of the p array, and if so, returns the address of the first occurrence in $GPGGA */
    p = (char *) strstr((const char *) buffer, "$GNGGA");

    /* If the number of "," is not enough, it means receiving error */
    posx = nmea_comma_position(p, 15);
    if (posx != 0XFF) gps_gga->checksum = nmea_get_checksum(buffer);
    else return 0;

    posx = nmea_comma_position(p, 1);
    if (posx != 0XFF)
        gps_gga->positioning_time.uct_time = nmea_str2num(
            p + posx, &gps_gga->positioning_time.decimal_places_time);

//    posx = nmea_comma_position(p, 2);
//    if (posx != 0XFF) gps_gga->latitude = nmea_str2num(p + posx, &gps_gga->decimal_places_latitude);
//
    STRING_TO_NUM(gps_gga->latitude, gps_gga->decimal_places_latitude, 2);

    posx = nmea_comma_position(p, 3);
    if (posx != 0XFF) gps_gga->latitude_direction = *(p + posx);
    posx = nmea_comma_position(p, 4);
    if (posx != 0XFF) gps_gga->longitude = nmea_str2num(p + posx, &gps_gga->decimal_places_longitude);
    posx = nmea_comma_position(p, 5);

    if (posx != 0XFF) gps_gga->longitude_direction = *(p + posx);
    posx = nmea_comma_position(p, 6);
    if (posx != 0XFF) gps_gga->positioning_quality = (int) nmea_str2num(p + posx, decimal_places);
    posx = nmea_comma_position(p, 7);
    if (posx != 0XFF) gps_gga->positioning_satellites_num = (int) nmea_str2num(p + posx, decimal_places);
    posx = nmea_comma_position(p, 8);

    if (posx != 0XFF) gps_gga->horizontal_accuracy_factor = nmea_str2num(p + posx, &gps_gga->decimal_places_accuracy);
    posx = nmea_comma_position(p, 9);
    if (posx != 0XFF) gps_gga->altitude = nmea_str2num(p + posx, &gps_gga->decimal_places_altitude);
    posx = nmea_comma_position(p, 10);
    if (posx != 0XFF) gps_gga->height_unit_altitude = *(p + posx);
    posx = nmea_comma_position(p, 11);
    if (posx != 0XFF) gps_gga->distance_reference_ellipsoid_geoid = nmea_str2num(p + posx, decimal_places);
    posx = nmea_comma_position(p, 12);
    if (posx != 0XFF) gps_gga->height_unit_distance = *(p + posx);
    posx = nmea_comma_position(p, 13);
    if (posx != 0XFF) gps_gga->differentially_corrected_data_age = (int) nmea_str2num(p + posx, decimal_places);
    posx = nmea_comma_position(p, 14);
    if (posx != 0XFF) gps_gga->differential_reference_stations_id = (char) nmea_str2num(p + posx, decimal_places);


    /* Verify that the checksum are correct */
    for (char i = 1; i < 14; i++) {
        char position = 0;
        char Decimal_places = 0;
        int num = 0;
        position = nmea_comma_position(p, i);
        num = nmea_str2num(p + position, &Decimal_places);
        check_sum = check_sum ^ num;
    }
    if (check_sum != gps_gga->checksum) return 0;
    else return 1;
}

/*!
    \brief      Analyze GPANT information
    \param[in]  buffer: Digital storage area
    \param[in]  gps_ant: antenna status structure
    \retval     0：Receive gps information error
                1：Receive gps information without error
*/
char nema_gpant_analysis(nmea_ant *gps_ant, char *buffer) {
    char posx;
    char i = 0;
    char ant_message[14] = {0};
//
// TODO: 将字符串改为复制的传递方式
//
    gps_ant->text_message = ant_message;

    /* check_sum is calculated, checksum is read from gps */
    int check_sum = 0;

    /* Variables a and decimal_places are not used */
    unsigned char a = 1;
    char *decimal_places = (char *) &a;

    char *p = (char *) strstr((const char *) buffer, "$GNTXT");

    /* If the number of "," is not enough, it means receiving error */
    posx = nmea_comma_position(p, 5);
    if (posx != 0XFF) gps_ant->checksum = nmea_get_checksum(buffer);
    else return 0;

    posx = nmea_comma_position(p, 1);
    if (posx != 0XFF) gps_ant->xx = nmea_str2num(p + posx, decimal_places);
    posx = nmea_comma_position(p, 2);
    if (posx != 0XFF) gps_ant->yy = nmea_str2num(p + posx, decimal_places);
    posx = nmea_comma_position(p, 3);
    if (posx != 0XFF) gps_ant->zz = nmea_str2num(p + posx, decimal_places);
    posx = nmea_comma_position(p, 4);
    if (posx != 0XFF) {
        while (*(p + posx + i) != '*') {
            ant_message[i] = *(p + posx + i);
            i++;
        }
    }

    /* Verify that the checksum are correct */
    for (i = 1; i < 4; i++) {
        char position = 0, Decimal_places = 0;
        int num = 0;
        position = nmea_comma_position(p, i);
        num = nmea_str2num(p + position, &Decimal_places);
        check_sum = check_sum ^ num;
    }
    if (check_sum != gps_ant->checksum) return 0;
    else return 1;
}
