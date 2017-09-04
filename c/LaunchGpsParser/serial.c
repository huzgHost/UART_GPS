#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include "serial.h"

int serialFile = -1;
void serial_init() {
    serialFile = open(PORTNAME, O_RDWR | O_NOCTTY | O_NDELAY);
    //serialFile = open("/home/huzg/demo/x2.txt", O_RDWR);

    if(-1 == serialFile) {
        //open faied
    }
}

void serial_config() {
    struct termios serialConfig;
    tcgetattr(serialFile, &serialConfig);
    serialConfig.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    serialConfig.c_iflag = IGNPAR;
    serialConfig.c_oflag = 0;
    serialConfig.c_lflag = 0;
    tcflush(serialFile, TCIFLUSH);
    tcsetattr(serialFile, TCSANOW, &serialConfig);
}

void serial_readln(char *buffer, int len) {
    char c;
    char *b = buffer;
    int rx_length = -1;
    while(1) {
        rx_length = read(serialFile, &c, 1);
        if(rx_length <= 0) {
            sleep(1);
        } else {
            if(c == '\n') {
                *b++ = '\0';
                break;
            }

            *b++ = c;
        }
    }
}

void serial_close() {
    close(serialFile);
}
