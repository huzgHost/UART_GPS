#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gps.h"
#include "serial.h"
#include "nmea.h"

void gps_init()
{
    serial_init();
    serial_config();
}

void gps_on()
{

}

void gps_location(loc_t *coord)
{
    uint8_t status = _EMPTY;
    //同时解析GPGGA，GPRMC，保证GPS数据的正确性
    while(status != _COMPLETED) {
        gpgga_t gpgga;
        gprmc_t gprmc;
        char buffer[256];

        serial_readln(buffer, 256);
        switch(nmea_get_message_type(buffer)) {
        case NMEA_GPGGA:
            nmea_parse_gpgga(buffer, &gpgga);

//            gps_convert_deg_to_dec(&(gpgga.latitude), gpgga.lat, &(gpgga.longitude), gpgga.lon);
            coord->latitude = gpgga.latitude;
            coord->longitude = gpgga.longitude;
            coord->altitude = gpgga.altitude;

            status |= NMEA_GPGGA;
            break;
        case NMEA_GPRMC:
            nmea_parse_gprmc(buffer, &gprmc);

            coord->speed = gprmc.speed;
            coord->course = gprmc.course;

            status |= NMEA_GPRMC;
            break;
        }
    }
}

void gps_close()
{
    serial_close();
}

void gps_convert_deg_to_dec(double *latitude, char ns, double *longitude, char we)
{
    //$GPGGA,184353.07,1929.045,S,02410.506,E,1,04,2.6,100.00,M,-33.9,M,,0000*6D
    double lat = (ns == 'N') ? *latitude : -1 * (*latitude);
    double lon = (we == 'E') ? *longitude : -1 * (*longitude);

    *latitude = gps_deg_dec(lat);
    *longitude = gps_deg_dec(lon);
}

double gps_deg_dec(double deg_point)
{
    double ddeg;
    double sec = modf(deg_point, &ddeg) * 60;//得到整数部分:ddeg; 小数部分:sec
    int deg = (int)(ddeg/100);//整数部分/100
    int min = (int)(deg_point-(deg*100));//

    double absdlat = round(deg * 1000000.);//度
    double absmlat = round(min * 1000000.);//分
    double absslat = round(sec * 1000000.);//秒

    return round(absdlat + (absmlat/60) + (absslat/3600)) /1000000;
}
