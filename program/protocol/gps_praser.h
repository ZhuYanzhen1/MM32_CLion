//
// Created by 16625 on 2021-12-24.
//

#ifndef MAIN_C_PROTOCOL_GPS_PRASER_H_
#define MAIN_C_PROTOCOL_GPS_PRASER_H_

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
    char a;
    float uct_time;
} nmea_utc_time;

typedef struct {
    nmea_utc_time positioning_time;
    float latitude;
    float longitude;
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
    short differential_reference_stations_id;
    float altitude;
    float horizontal_accuracy_factor;

    /* "-" indicates that the geoid is lower than the reference ellipsoid */
    float distance_reference_ellipsoid_geoid;

    /* This field is empty when DGPS is not used */
    int differentially_corrected_data_age;
    int checksum;

} nmea_gga;

char nmea_comma_position(char *buffer, char n);
int nmea_pow(char m, char n);
int nmea_str2num(char *buffer);
void nmea_gpgga_analysis(nmea_gga *gpsx, char *buffer);

#endif //MAIN_C_PROTOCOL_GPS_PRASER_H_
