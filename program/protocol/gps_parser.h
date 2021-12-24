//
// Created by 16625 on 2021-12-24.
//

#ifndef MAIN_C_PROTOCOL_GPS_PARSER_H_
#define MAIN_C_PROTOCOL_GPS_PARSER_H_

typedef enum {
    error_message = 0,
    warning_message = 1,
    notification_message = 2,
    user_message = 7
} text_identifiers;

typedef struct {
    int satellite_azimuth;
    char satellite_number;
    char satellite_elevation_angle;
    char signal_to_noise_ratio;
} nmea_satellite_information;

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
    nmea_utc_time positioning_time;
    long latitude;
    long longitude;
    char latitude_direction;
    char longitude_direction;

    /* 0:Positioning is unavailable or invalid */
    /* 1:SPS positioning mode, positioning effective */
    /* 6:Estimation mode (heading projection) is only valid for NMEA 2.3 and above */
    char positioning_quality;

    /* 0~24 */
    char positioning_satellites_num;

    char height_unit_altitude;
    char height_unit_distance;
    char differential_reference_stations_id;
    char decimal_places_distance;

    long altitude;
    long horizontal_accuracy_factor;

    /* "-" indicates that the geoid is lower than the reference ellipsoid */
    long distance_reference_ellipsoid_geoid;

    /* This field is empty when DGPS is not used */
    int differentially_corrected_data_age;
    int checksum;
    char decimal_places_latitude;
    char decimal_places_longitude;
    char decimal_places_altitude;
    char decimal_places_accuracy;

} nmea_gga;

typedef struct {
    char xx;
    char yy;
    char *text_message;
    char checksum;
    text_identifiers zz;
} nmea_ant;

int nmea_comma_position(char *buffer, char n);
int nmea_pow(char m, char n);
long nmea_str2num(char *buffer, char *decimal_places);
void nmea_gpgga_analysis(nmea_gga *gpsx, char *buffer);
void nema_gpant_analysis(nmea_ant *gps_ant, char *buffer);

#endif //MAIN_C_PROTOCOL_GPS_PARSER_H_
