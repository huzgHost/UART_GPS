#ifndef SERIAL_H
#define SERIAL_H

#ifndef PORTNAME
#define PORTNAME '/dev/ttyUSB0'
#endif

void serial_init(void);
void serial_config(void);
void serial_readln(char *, int);
void serial_close(void);

#endif // SERIAL_H
