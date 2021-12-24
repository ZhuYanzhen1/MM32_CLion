//
// Created by 16625 on 2021-12-24.
//

#ifndef MAIN_C_PROTOCOL_GPS_PRASER_H_
#define MAIN_C_PROTOCOL_GPS_PRASER_H_

//GPS NMEA-0183协议重要参数结构体定义
//卫星信息
typedef struct {
    int satellite_azimuth;          //卫星方位角
    char satellite_number;          //卫星编号
    char satellite_elevation_angle; //卫星仰角
    char signal_to_noise_ratio;     //信噪比
} nmea_satellite_information;

//UTC时间信息
typedef struct {
    short year;
    char month;
    char date;
    char hour;
    char minute;
    char second;
} nmea_utc_time;

//NMEA 0183 协议解析后数据存放结构体
typedef struct {
    char visible_satellites_num;                   //可见卫星数
    nmea_satellite_information slmsg[32];       //最多12颗卫星

    char id[5];
    nmea_utc_time positioning_time;          //用于定位的时间
    int latitude;               //纬度 分扩大100000倍,实际要除以100000
    char latitude_direction;                  //北纬/南纬,N:北纬;S:南纬
    int longitude;              //经度 分扩大100000倍,实际要除以100000
    char longitude_direction;                  //东经/西经,E:东经;W:西经
    char positioning_quality;//定位质量
    char positioning_satellites_num;                //用于定位的卫星数,0~12.
    float location_longitude_factor;                   //位置精度因子 0~500,对应实际值0~50.0
    float altitude;               //海拔高度,放大了10倍,实际除以10.单位:0.1m
    char height_unit_altitude;           //高度单位
    int distance_reference_ellipsoid_geoid;//参考椭球面与大地水准面之间的距离，“-”表示大地水准面低于参考椭球面
    char height_unit_distance;
    int differentially_corrected_data_age;//差分修正的数据龄期，未使用DGPS时该域为空
    int differential_reference_stations_id;
    int checksum;

    short speed;                  //地面速率,放大了1000倍,实际除以10.单位:0.001公里/小时
} nmea_gga;

char nmea_comma_position(char *buffer, char n);
int nmea_pow(char m, char n);
int nmea_str2num(char *buffer, char *decimal_places);

#endif //MAIN_C_PROTOCOL_GPS_PRASER_H_
