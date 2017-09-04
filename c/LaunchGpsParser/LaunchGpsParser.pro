TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    serial.c \
    gps.c \
    nmea.c

HEADERS += \
    serial.h \
    gps.h \
    nmea.h
