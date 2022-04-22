/**************************************************************************/ /**
    \file       gps_parser.h
    \brief      DMA function Header File
    \author     ZGL
    \version    V1.3.2
    \date       06. March 2022
******************************************************************************/

#ifndef MM32F3277_PROTOCOL_GPS_PARSER_H_
#define MM32F3277_PROTOCOL_GPS_PARSER_H_

typedef enum {
    autonomous_mode = 'A',
    voyage_projection = 'E',
    invalid_data = 'N',
    mifferential_mode = 'D',
    Unlocated = 'M'
} positioning_mode;

typedef volatile enum {
    invalid = 'V',
    valid = 'A'
} location_valid_flag;

typedef struct {
    short year;
    char month;
    char date;
    char hour;
    char minute;
    char second;
    char decimal_places_time;
    long uct_time;
} nmea_utc_time;

typedef struct {
    unsigned int latitude;
    unsigned int longitude;
    char decimal_places_latitude;
    char decimal_places_longitude;
    char latitude_direction;
    char longitude_direction;
    int speed_to_ground_section;
    int direction_of_ground_truth;
    int date;
    positioning_mode mode;
    char checksum;
    char decimal_places_speed;
    char decimal_places_direction;
    location_valid_flag status;
    nmea_utc_time positioning_time;
} nmea_rmc;

extern nmea_rmc gps_rmc;

void nmea_gnrmc_analysis(char *buffer);
void deal_dma_gnrmc(const unsigned int *p);
int nmea_pow(char m, char n);
int num_times_nth_power_of_10(int num, int n);
float unit_to_degree(unsigned int degree, char decimal_places);

#endif // MM32F3277_PROTOCOL_GPS_PARSER_H_
