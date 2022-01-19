//
// Created by 16625 on 2021-12-24.
//

#ifndef MAIN_C_PROTOCOL_GPS_PARSER_H_
#define MAIN_C_PROTOCOL_GPS_PARSER_H_

typedef enum {
    autonomous_mode = 'A',
    voyage_projection = 'E',
    invalid_data = 'N',
    mifferential_mode = 'D',
    Unlocated = 'M'
} positioning_mode;

typedef enum {
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
    int latitude;
    int longitude;
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

void nmea_all_comma_position(char *buffer, char *comma, char n);
unsigned char nmea_get_checksum(char *buffer);
int num_times_nth_power_of_10(int num, int n);
int nmea_pow(char m, char n);
int nmea_str2num(char *buffer, char *decimal_places);
void nmea_gnrmc_analysis(nmea_rmc *gps_rmc, char *buffer);
void change_latitude_longitude_format(int *degree, char decimal_places);

void deal_dma_gnrmc(const unsigned int *p);

#endif //MAIN_C_PROTOCOL_GPS_PARSER_H_
