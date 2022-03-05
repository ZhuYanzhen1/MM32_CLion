/**************************************************************************/ /**
    \file       gps_parser.c
    \brief      gps unpacking algorithm,
                algorithm for processing gps packets truncated by DMA
    \author     ZGL
    \version    V1.2.2
    \date       19. January 2022
******************************************************************************/

#include "gps_parser.h"

#define STRING_TO_NUM(x, y, num)    if(comma_position[(num)-1]!=0) \
                                        (x) = nmea_str2num(p + comma_position[(num)-1] +1, &(y));
#define STRING_TO_STR(x, num)       if (comma_position[(num)-1]!=0) \
                                        (x) = *(p + comma_position[(num)-1]+1);

nmea_rmc gps_rmc = {0};

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
    \brief      num*(10^n)
    \param[in]  num
    \param[in]  n (n<=3 && n>=0)
    \retval     num*(10^n)
*/
int num_times_nth_power_of_10(int num, int n) {
    switch (n) {
        case 0:num = num;
            break;
        case 1:num = (num << 3) + (num << 1);
            break;
        case 2:num = (num << 6) + (num << 5) + (num << 2);
            break;
        case 3:num = (num << 10) - (num << 4) - (num << 3);
            break;
        default:
            while (n > 0) {
                num *= 10;
                n--;
            }
            break;
    }
    return num;
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
        integer_data = num_times_nth_power_of_10(integer_data, 1) + (buffer[i] - '0');

    /* Maximum 4 decimal places */
    if (decimal_length > 4)
        decimal_length = 4;
    *decimal_places = decimal_length;

    /* Get the decimal part of the data */

    for (unsigned char i = 0; i < decimal_length; i++)
        decimal_data = num_times_nth_power_of_10(decimal_data, 1) + (buffer[integer_length + 1 + i] - '0');

    data = num_times_nth_power_of_10(integer_data, decimal_length) + decimal_data;
    if (mask & 0X02) data = -data;
    return data;

}

/*!
    \brief      get checksum
    \param[in]  buffer: Digital storage area
    \retval     checksum
*/
unsigned char nmea_get_checksum(char *buffer) {
    while (*buffer++ != '*');
    int checksum = ((buffer[0] - (buffer[0] > 58 ? '7' : '0')) << 4) + (buffer[1] - (buffer[1] > 58 ? '7' : '0'));
    return checksum;
}

/*!
    \brief      Convert the latitude and longitude read by gps to int format of degree,
                The highest two to three digits are integer parts, the rest are fractional parts
    \param[in]  degree: Longitude or Latitude
    \param[in]  decimal_places: Fractional digits of latitude and longitude
    \retval     Decimal places
*/
unsigned char change_latitude_longitude_format(unsigned int *degree, char decimal_places) {
    unsigned int fractional_part = nmea_pow(10, decimal_places + 2);
    unsigned int decimal_latitude = (*degree) % fractional_part / 60;
    unsigned int temp_decimal = decimal_latitude;
    unsigned char n = 0;
    while (temp_decimal > 0) {
        temp_decimal /= 10;
        n++;
    }
//    *degree = (*degree / fractional_part) * nmea_pow(10, decimal_places) + decimal_latitude;
    unsigned int int_degree = *degree / fractional_part;
    *degree = num_times_nth_power_of_10((int) int_degree, n) + decimal_latitude;
    return n;
}

/*!
    \brief      Analyze GPRMC information
    \param[in]  buffer: Digital storage area
    \param[in]  gps_rmc: Recommended minimum positioning information
                time, status, latitude, longitude, speed, direction, positioning mode
*/
void nmea_gnrmc_analysis(char *buffer) {
    char i = 0, decimal_places = 1, check_sum = 0, comma_position[13] = {0};
    char *p = buffer;

    nmea_all_comma_position(buffer, comma_position, 13);
    if (comma_position[12 != 0])
        gps_rmc.checksum = nmea_get_checksum(buffer);
    else
        return;
    while (p[i] != '*')
        check_sum ^= p[i++];
    if (check_sum != gps_rmc.checksum)
        return;

    STRING_TO_NUM(gps_rmc.positioning_time.uct_time, gps_rmc.positioning_time.decimal_places_time, 1)
    STRING_TO_STR(gps_rmc.status, 2)
    STRING_TO_NUM(gps_rmc.latitude, gps_rmc.decimal_places_latitude, 3)
    STRING_TO_STR(gps_rmc.latitude_direction, 4)
    STRING_TO_NUM(gps_rmc.longitude, gps_rmc.decimal_places_longitude, 5)
    STRING_TO_STR(gps_rmc.longitude_direction, 6)
    STRING_TO_NUM(gps_rmc.speed_to_ground_section, gps_rmc.decimal_places_speed, 7)
    STRING_TO_NUM(gps_rmc.direction_of_ground_truth, gps_rmc.decimal_places_direction, 8)
    STRING_TO_NUM(gps_rmc.date, decimal_places, 9)
    STRING_TO_STR(gps_rmc.mode, 12)

}

static unsigned char status = 0;
static unsigned char package_buffer[80];
static unsigned char package_counter = 0;

#ifndef RUNNING_UNIT_TEST
void deal_dma_gnrmc(const unsigned int *p) {
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
            case 2:nmea_gnrmc_analysis((char *) package_buffer);
                package_counter = 0;
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
            default:break;
        }
    }
}
#endif  // RUNNING_UNIT_TEST


