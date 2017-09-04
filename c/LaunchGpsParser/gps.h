#ifndef GPS_H
#define GPS_H

struct location{
    double latitude;
    double longitude;
    double speed;
    double altitude;
    double course;
};

typedef struct location loc_t;

void gps_init(void);
void gps_on(void);
void gps_location(loc_t *);

void gps_close(void);

void gps_convert_deg_to_dec(double *, char, double *, char);
double gps_deg_dec(double);

#endif // GPS_H
