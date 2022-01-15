//
// Created by 16625 on 2021-12-24.
//

#include "gps_parser.h"
#include "string.h"
#include "qfplib.h"

#define STRING_TO_NUM(x, y, num)    posx = nmea_comma_position(p, num);\
                                    if (posx != 0XFF) \
                                        x = nmea_str2num(p + posx, &y);
#define STRING_TO_STR(x, num)       posx = nmea_comma_position(p, num);\
                                    if (posx != 0XFF) \
                                        x = *(p + posx);
#define STRING_TO_NUM_CHAR(x, num)  posx = nmea_comma_position(p, num);\
                                    if (posx != 0XFF) \
                                        x = (char)nmea_str2num(p + posx, &decimal_places);
/*
 * To be optimized: wait until all the unit tests are finished,
 * and change the floating-point operations to the four operations in qfplib.h
 */

/*!
    \brief      Get the position of the nth comma from inside buffer
    \param[in]  buffer: Digital storage area
    \retval     0~0XFE,represents the offset of the comma location.
                0XFF means there is no nth comma
    \note       The return value type cannot be char, because when the desired target is not found,
                the return 0xff is greater than the range of char
*/
int nmea_comma_position(char *buffer, char n) {
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
    if (m == 0)
        return 0;
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
    for (int i = 0; i < integer_length; i++)
        integer_data += nmea_pow(10, integer_length - 1 - i) * (buffer[i] - '0');

    /* Maximum 4 decimal places */
    if (decimal_length > 4)
        decimal_length = 4;
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
    *degree = (*degree / fractional_part) * 1000000 + second_latitude;
}

/*!
    \brief      Analyze GPRMC information
    \param[in]  buffer: Digital storage area
    \param[in]  gps_rmc: Recommended minimum positioning information
                time, status, latitude, longitude, speed, direction, positioning mode
*/
void nmea_gnrmc_analysis(nmea_rmc *gps_rmc, char *buffer) {
    char *p, i = 0, decimal_places = 1, posx = 0, check_sum = 0;

    /* strstr determines whether $GPGGA is a substring of the p array,
     * and if so, returns the address of the first occurrence in $GPGGA */
    p = (char *) strstr((const char *) buffer, "$GNRMC");

    /* If the number of "," is not enough, it means receiving error */
    posx = nmea_comma_position(p, 13);
    if (posx != 0XFF)
        gps_rmc->checksum = nmea_get_checksum(buffer);
    else
        return;

    /* Verify that the checksum are correct */
    while (p[++i] != '*')
        check_sum ^= p[i];
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

/*!
    \brief      Analyze GPGGA information
    \param[in]  buffer: Digital storage area
    \param[in]  gps_gga: Receiver time, location and positioning related data
*/
void nmea_gngga_analysis(nmea_gga *gps_gga, unsigned char *buffer) {
    char *p, i = 0, decimal_places = 1, posx = 0, check_sum = 0;

    /* strstr determines whether $GPGGA is a substring of the p array,
     * and if so, returns the address of the first occurrence in $GPGGA */
    p = (char *) strstr((const char *) buffer, "$GNGGA");

    /* If the number of "," is not enough, it means receiving error */
    posx = nmea_comma_position(p, 15);
    if (posx != 0XFF)
        gps_gga->checksum = nmea_get_checksum(buffer);
    else
        return;

    /* Verify that the checksum are correct */
    while (p[++i] != '*')
        check_sum ^= p[i];
    if (check_sum != gps_gga->checksum)
        return;

    STRING_TO_NUM(gps_gga->positioning_time.uct_time, gps_gga->positioning_time.decimal_places_time, 1)
    STRING_TO_NUM(gps_gga->latitude, gps_gga->decimal_places_latitude, 2)
    STRING_TO_STR(gps_gga->latitude_direction, 3)
    STRING_TO_NUM(gps_gga->longitude, gps_gga->decimal_places_longitude, 4)
    STRING_TO_STR(gps_gga->longitude_direction, 5)
    STRING_TO_NUM_CHAR(gps_gga->positioning_quality, 6)
    STRING_TO_NUM_CHAR(gps_gga->positioning_satellites_num, 7)
    STRING_TO_NUM(gps_gga->horizontal_accuracy_factor, gps_gga->decimal_places_accuracy, 8)
    STRING_TO_NUM(gps_gga->altitude, gps_gga->decimal_places_altitude, 9)
    STRING_TO_STR(gps_gga->height_unit_altitude, 10)
    STRING_TO_NUM_CHAR(gps_gga->distance_reference_ellipsoid_geoid, 11)
    STRING_TO_STR(gps_gga->height_unit_distance, 12)
    STRING_TO_NUM_CHAR(gps_gga->differentially_corrected_data_age, 13)
    STRING_TO_NUM_CHAR(gps_gga->differential_reference_stations_id, 14)

}

/*!
    \brief      Analyze GPANT information
    \param[in]  buffer: Digital storage area
    \param[in]  gps_ant: antenna status structure
*/
void nmea_gnant_analysis(nmea_ant *gps_ant, unsigned char *buffer) {
    char posx, i = 0, *p, check_sum = 0, decimal_places = 1;

    p = (char *) strstr((const char *) buffer, "$GNTXT");

    /* If the number of "," is not enough, it means receiving error */
    posx = nmea_comma_position(p, 5);
    if (posx != 0XFF)
        gps_ant->checksum = nmea_get_checksum(buffer);
    else
        return;

    /* Verify that the checksum are correct */
    while (p[++i] != '*') {
        check_sum ^= p[i];
    }
    if (check_sum != gps_ant->checksum)
        return;

    STRING_TO_NUM_CHAR(gps_ant->xx, 1)
    STRING_TO_NUM_CHAR(gps_ant->yy, 2)
    STRING_TO_NUM_CHAR(gps_ant->zz, 3)
    posx = nmea_comma_position(p, 4);
    if (posx != 0XFF) {
        i = 0;
        while (*(p + posx + i) != '*') {
            gps_ant->text_message[i] = *(p + posx + i);
            i++;
        }
    }
}

/*!
    \brief      Analyze GPANT information
    \param[in]  buffer: Digital storage area
    \param[in]  gps_vtg: Ground speed and ground heading information.
*/
void nmea_gnvtg_analysis(nmea_vtg *gps_vtg, unsigned char *buffer) {
    char posx = 0, i = 0, check_sum = 0, decimal_places = 1;
    char *p = (char *) strstr((const char *) buffer, "$GNVTG");

    /* If the number of "," is not enough, it means receiving error */
    posx = nmea_comma_position(p, 10);
    if (posx != 0XFF)
        gps_vtg->checksum = nmea_get_checksum(buffer);
    else
        return;

    /* Verify that the checksum are correct */
    while (p[++i] != '*') {
        check_sum ^= p[i];
    }
    if (check_sum != gps_vtg->checksum)
        return;

    STRING_TO_NUM(gps_vtg->true_north_direction, gps_vtg->decimal_point_direction, 1)
    STRING_TO_NUM_CHAR(gps_vtg->true_north_indication, 2)
    STRING_TO_NUM_CHAR(gps_vtg->to_geomagnetic_north_direction, 3)
    STRING_TO_NUM_CHAR(gps_vtg->magnetic_north_indication, 4)
    STRING_TO_NUM(gps_vtg->speed_to_ground_section, gps_vtg->decimal_point_speed_section, 5)
    STRING_TO_NUM_CHAR(gps_vtg->speed_unit_knots, 6)
    STRING_TO_NUM(gps_vtg->speed_to_ground_kmh, gps_vtg->decimal_point_speed_section, 7)
    STRING_TO_NUM_CHAR(gps_vtg->speed_unit, 8)
    STRING_TO_NUM_CHAR(gps_vtg->positioning_mode_flag, 9)
}
