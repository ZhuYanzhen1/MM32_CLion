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
void coordinate_system_transformation_neu(float delta) {
    int temp_acll_ax, temp_acll_ay;
    float temp_delta = GEO_ANGLE(delta);

    float temp_latitude = unit_to_degree(gps_rmc.latitude, 4);
    float temp_lonitude = unit_to_degree(gps_rmc.longitude, 4);
    neu.north_distance = get_distance(QRIGIN_LAT, temp_lonitude, temp_latitude, temp_lonitude);
    neu.east_distance = get_distance(temp_latitude, QRIGIN_LON, temp_latitude, temp_lonitude);
//    float temp_latitude = unit_to_degree(gps_rmc.latitude, 4);
//    neu.north_distance = get_distance_m_lat(temp_latitude - QRIGIN_LAT);
//    float temp_lonitude = unit_to_degree(gps_rmc.longitude, 4);
//    neu.east_distance = get_distance_m_lon(temp_lonitude - QRIGIN_LON);

    temp_acll_ax = small_packets.ax;
    temp_acll_ay = small_packets.ay;
    neu.north_acceleration = MG_TO_M_S_2
    ((float) temp_acll_ax * FACTOR_ALLC * qfp_fcos(temp_delta)
         + (float) temp_acll_ay * FACTOR_ALLC * qfp_fcos(temp_delta + PI / 2));
    neu.east_acceleration = MG_TO_M_S_2
    ((float) temp_acll_ax * FACTOR_ALLC * qfp_fsin(temp_delta)
         + (float) temp_acll_ay * FACTOR_ALLC * qfp_fsin(temp_delta + PI / 2));

    /* Use GPS to get the velocity, convert the units,
     * and then convert the coordinate system to the NEU */
    float temp_v = (float) gps_rmc.speed_to_ground_section;
    int v_decimal = num_times_nth_power_of_10(1, gps_rmc.decimal_places_speed);
    temp_v = KNOT_TO_M_S(temp_v / v_decimal);
    neu.north_v = temp_v * qfp_fcos(temp_delta);
    neu.east_v = temp_v * qfp_fsin(temp_delta);
}
