//
// Created by 16625 on 2022-03-06.
//

#ifndef MM32F3277_MATH_DATA_CONVERSION_H_
#define MM32F3277_MATH_DATA_CONVERSION_H_

// TODO 当到了其他地方比赛之前，得先测试当地的经纬度，并更改此处的宏，否则会导致浮点运算精度丢失的问题
#define QRIGIN_LAT      20  /* The starting point is set as(20°N,110°E) */
#define QRIGIN_LON      110
#define PI              3.1415926f
#define EARTH_RADIUS    6371000
#define FACTOR_ALLC     1.25f
#define GEO_ANGLE(x)    ((x) * PI / 180.0)
#define KNOT_TO_M_S(x)  ((x)* 0.514444)
#define MG_TO_M_S_2(x)  ((x)/102.1627f)

typedef volatile struct {
    float north_acceleration;
    float east_acceleration;
    float north_v;
    float east_v;
    float north_distance;
    float east_distance;
} neu_infomation;

extern neu_infomation neu;

#endif //MAIN_C_MATH_DATA_CONVERSION_H_
