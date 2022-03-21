/**************************************************************************/ /**
    \file       data_conversion.c
    \brief      Coordinate system transformation
                Data Transformation
    \author     ZGL
    \version    V1.3.2
    \date       06. March 2022
******************************************************************************/

#include "data_conversion.h"
#include "qfplib.h"
#include "gps_parser.h"
#include "sensor_decode.h"
#include "fast_math.h"
#include "kalman.h"

extern kalman_data_t kalman_data;

neu_infomation neu = {0};

/*!
    \brief      Get the distance based on the latitude and longitude of the starting point
                and the latitude and longitude of the current state
    \param[in]  lat_1(°):Starting latitude
    \param[in]  lon_1(°):Starting longitude
    \param[in]  lat_2(°):Latitude of Present State
    \param[in]  lon_2(°):longitude of Present State
    \retval     Distance between two points on the earth
    \note       When we keep the longitude the same, we can calculate the distance to the north
                When we keep the latitude the same, we can calculate the distance to the east
*/
float get_distance(float lat_2, float lon_2, float lat_1, float lon_1) {
    float temp =
        my_pow(qfp_fsin(GEO_ANGLE((lat_1 - lat_2) * 0.5f)), 2) +
            qfp_fcos(GEO_ANGLE(lat_1)) * qfp_fcos(GEO_ANGLE(lat_2))
                * my_pow(qfp_fsin(GEO_ANGLE((lon_1 - lon_2) * 0.5f)), 2);
    float distance = 2 * EARTH_RADIUS * my_asin(qfp_fsqrt(temp));
    return distance;
}

/*!
    \brief      Convert latitude to displacement in ENU coordinate system
    \param[in]  latitude(°)
    \retval     Converted data
*/
float get_distance_m_lat(float lat) {
    float distance = GEO_ANGLE(lat);
    return EARTH_RADIUS * distance;
}

/*!
    \brief      Convert longitude to displacement in ENU coordinate system
    \param[in]  longitude(°)
    \retval     Converted data
*/
float get_distance_m_lon(float lon) {
    float distance = GEO_ANGLE(lon);
    return EARTH_RADIUS * distance;
}

/*!
    \brief      Preceding steps of Kalman Fusion
                Convert the acceleration of imu and velocity of gps to NEU coordinate system
                Convert the data units read from the sensor to m/s^2, m/s, m
    \param[in]  True North Angle
*/
void sensor_unit_conversion() {
    float temp_latitude = unit_to_degree(gps_rmc.latitude, 4);
    float temp_lonitude = unit_to_degree(gps_rmc.longitude, 4);
    neu.north_distance = get_distance(QRIGIN_LAT, temp_lonitude, temp_latitude, temp_lonitude);
    neu.east_distance = get_distance(temp_latitude, QRIGIN_LON, temp_latitude, temp_lonitude);

    neu.acceleration = MG_TO_M_S_2(small_packets.ax * FACTOR_ALLC);

    float temp_v = (float) gps_rmc.speed_to_ground_section;
    int v_decimal = num_times_nth_power_of_10(1, gps_rmc.decimal_places_speed);
    neu.v = KNOT_TO_M_S(temp_v / v_decimal);
}

void coordinate_system_transformation_kalman_v(float delta) {
    float temp_delta = GEO_ANGLE(delta);
    neu.north_v = kalman_data.v * qfp_fcos(temp_delta);
    neu.east_v = kalman_data.v * qfp_fsin(temp_delta);
}
