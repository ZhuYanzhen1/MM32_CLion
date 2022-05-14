/**************************************************************************/ /**
    \file       data_conversion.h
    \brief      Coordinate system transformation
                Data Transformation
    \author     ZGL
    \version    V1.3.3
    \date       22. March 2022
******************************************************************************/
#ifndef MM32F3277_MATH_DATA_CONVERSION_H_
#define MM32F3277_MATH_DATA_CONVERSION_H_

// TODO 当到了其他地方比赛之前，得先测试当地的经纬度，并更改此处的宏，否则会导致浮点运算精度丢失的问题
#define QRIGIN_LAT      23 /* The starting point is set as(20°N,110°E) */
#define QRIGIN_LON      113  //113.4f
#define EARTH_RADIUS    6371000
#define FACTOR_ALLC_X   1.778f
#define FACTOR_ALLC_Y   1.811f
#define FACTOR_ALLC_Z   1.899f
#define GEO_ANGLE(x)    ((x) * 0.0174533f)  // PI / 180 = 0.0174533f
#define KNOT_TO_M_S(x)  ((x) * 0.514444f)
#define MG_TO_M_S_2(x)  ((x) / 102.1627f)

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
