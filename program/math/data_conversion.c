//
// Created by 16625 on 2022-03-06.
//

#include "data_conversion.h"
#include "qfplib.h"
#include "adis16470.h"
#include "gps_parser.h"

neu_infomation neu = {0};

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
    float temp_delta = GEO_ANGLE(delta);

    float temp_latitude = unit_to_degree(gps_rmc.latitude, 4);
    neu.north_distance = get_distance_m_lat(temp_latitude - QRIGIN_LAT);
    float temp_lonitude = unit_to_degree(gps_rmc.longitude, 4);
    neu.east_distance = get_distance_m_lon(temp_lonitude - QRIGIN_LON);

    neu.north_acceleration = MG_TO_M_S_2
    ((float) imu.x_acll * FACTOR_ALLC * qfp_fcos(temp_delta)
         + (float) imu.y_acll * FACTOR_ALLC * qfp_fcos(temp_delta + PI / 2));
    neu.east_acceleration = MG_TO_M_S_2
    ((float) imu.x_acll * FACTOR_ALLC * qfp_fsin(temp_delta)
         + (float) imu.y_acll * FACTOR_ALLC * qfp_fsin(temp_delta + PI / 2));

    /* Use GPS to get the velocity, convert the units,
     * and then convert the coordinate system to the NEU */
    float temp_v = (float) gps_rmc.speed_to_ground_section;
    int v_decimal = num_times_nth_power_of_10(1, gps_rmc.decimal_places_speed);
    temp_v = KNOT_TO_M_S(temp_v / v_decimal);
    neu.north_v = temp_v * qfp_fcos(temp_delta);
    neu.east_v = temp_v * qfp_fsin(temp_delta);
}

