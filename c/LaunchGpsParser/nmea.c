#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

#include "nmea.h"

uint8_t nmea_get_message_type(const char *message)
{
    uint8_t checksum = 0;
    if((checksum = nmea_valid_checksum(message)) != _EMPTY) {
        return checksum;
    }

    if(strstr(message, NMEA_GPGGA_STR) != NULL) {
        return NMEA_GPGGA;
    }

    if(strstr(message, NMEA_GPRMC_STR) != NULL) {
        return NMEA_GPRMC;
    }

    return NMEA_UNKNOWN;
}

uint8_t nmea_valid_checksum(const char *message)
{
    uint8_t checksum= strtol(strchr(message, '*') + 1, NULL, 16);//转换成16进制
    char p;
    uint8_t sum = 0;
    ++message;//过滤掉$
    while((p = *message++) != '*') {
        sum ^= p;
    }

    if(sum != checksum) {
        return NMEA_CHECKSUM_ERR;
    }

    return _EMPTY;
}

void nmea_parse_gpgga(char *nmea, gpgga_t *loc)
{
    //$GPGGA,184353.07,1929.045,S,02410.506,E,1,04,2.6,100.00,M,-33.9,M,,0000*6D
    char *p = nmea;
    p = strchr(p, ',') + 1;

    p = strchr(p, ',') + 1;
    loc->latitude = atof(p);

    p = strchr(p, ',') + 1;
    switch (p[0]) {
    case 'N':
        loc->lat = 'N';
        break;
    case 'S':
        loc->lat = 'S';
    case ',':
        loc->lat = '\0';
        break;
    }

    p = strchr(p, ',') + 1;
    loc->longitude = atof(p);

    p = strchr(p, ',') + 1;
    switch (p[0]) {
    case 'W':
        loc->lon = 'W';
        break;
    case 'E':
        loc->lon = 'E';
    case ',':
        loc->lon = '\0';
        break;
    }

    p = strchr(p, ',') + 1;
    loc->quality = (uint8_t)atoi(p);

    p = strchr(p, ',') + 1;
    loc->satellites = (uint8_t)atoi(p);

    p = strchr(p, ',') + 1;

    p = strchr(p, ',') + 1;
    loc->altitude = atof(p);

}

void nmea_parse_gprmc(char *nmea, gprmc_t *loc)
{
    //GNRMC,084852.000,A,2236.9453,N,11408.4790,E,0.53,292.44,141216,,,A*75
    char *p = nmea;
    p = strchr(p, ',') + 1;
    p = strchr(p, ',') + 1;

    p = strchr(p, ',') + 1;
    loc->latitude = atof(p);

    p = strchr(p, ',') + 1;
    switch (p[0]) {
    case 'N':
        loc->lat = 'N';
        break;
    case 'S':
        loc->lat = 'S';
        break;
    case ',':
        loc->lat = '\0';
        break;
    }

    p = strchr(p, ',') + 1;
    loc->longitude = atof(p);

    p = strchr(p, ',') + 1;
    switch(p[0]) {
    case 'W':
        loc->lon = 'W';
        break;
    case 'E':
        loc->lon = 'E';
        break;
    case ',':
        loc->lon = '\0';
        break;
    }

    p = strchr(p, ',') + 1;
    loc->speed = atof(p);

    p = strchr(p, ',') + 1;
    loc->course = atof(p);
}
