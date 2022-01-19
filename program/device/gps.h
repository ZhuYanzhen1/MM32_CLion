/**************************************************************************/ /**
    \file       gps.c
    \brief      Contains gps initialization and
                display of received gps related information on the screen
    \author     ZGL
    \version    V1.0.2
    \date       19. January 2022
******************************************************************************/

#ifndef MAIN_C_DEVICE_GPS_H_
#define MAIN_C_DEVICE_GPS_H_

void gps_config();
void gui_show_gnrmc_information(nmea_rmc *gps_rmc);

#endif  // MAIN_C_DEVICE_GPS_H_
