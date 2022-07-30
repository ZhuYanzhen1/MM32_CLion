/**************************************************************************/ /**
    \file       data_conversion.h
    \brief      Coordinate system transformation
                Data Transformation
    \author     ZGL
    \version    V1.6.1
    \date       24. July 2022
******************************************************************************/
#ifndef MM32F3277_MATH_DATA_CONVERSION_H_
#define MM32F3277_MATH_DATA_CONVERSION_H_

/*! \brief The starting point is set as(23°N,113°E) */
#define QRIGIN_LAT      23
#define QRIGIN_LON      113

#define EARTH_RADIUS    6371000

/*! \brief Acceleration conversion factor */
#define FACTOR_ALLC_X   1.778f
#define FACTOR_ALLC_Y   1.811f
#define FACTOR_ALLC_Z   1.899f

/*! \brief Convert the units of the velocity obtained from GPS from knot to m/s */
#define GEO_ANGLE(x)    ((x) * 0.0174533f)  // PI / 180 = 0.0174533f

/*! \brief Convert the units of the velocity obtained from GPS from knot to m/s */
#define KNOT_TO_M_S(x)  ((x) * 0.514444f)

/*! \brief Change the units of the accelerometer data from mg to m/s^2 */
#define MG_TO_M_S_2(x)  ((x) / 102.1627f)

/*! \brief Status information in the neu coordinates */
typedef volatile struct {
    float north_v;
    float east_v;
    float north_distance;
    float east_distance;
    float v;
    float acceleration;
} neu_infomation;

extern neu_infomation neu;

float get_distance(float lat_2, float lon_2, float lat_1, float lon_1);
void sensor_unit_conversion();
void coordinate_system_transformation_kalman_v(float delta);

#endif  // MM32F3277_MATH_DATA_CONVERSION_H_
