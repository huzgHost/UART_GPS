#include <stdio.h>
#include "serial.h"
#include "gps.h"

int main()
{
    gps_init();
    loc_t data;

    while(1) {
        gps_location(&data);

        printf("%lf %lf\n", data.latitude, data.longitude);
    }

    return 0;
}
