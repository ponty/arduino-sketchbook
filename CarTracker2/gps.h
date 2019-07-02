#pragma once

#include "config_hw.h"

#ifdef EXT_GPS
#ifdef __MBED__

#include "TinyGPS.h"

Serial ser_gps(NC, PIN_GPS_RX);
TinyGPS tinygps;

#else

#include <MicroNMEA.h>

SoftwareSerial gps(PIN_GPS_RX, PIN_GPS_TX); // RX, TX
char nmeaBuffer[85];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

#endif
#endif

// gps
double latitude = 0;
double longitude = 0;
double speed = 0;
double hdop = 0;
double altitude = 0;
double heading = 0;

void gps_init()
{
#ifdef EXT_GPS
#ifdef __MBED__
    // GPS
    ser_gps.baud(9600);
#else
    gps.begin(9600);
#endif
#endif

}

void read_gps()
{
    INFLN("waiting for GPS position ");

    int year;
    byte month;
    byte day;
    byte hour;
    byte minute;
    byte second;

#ifdef EXT_GPS

#ifdef __MBED__
    tinygps.reset_ready();
    while (1)
    {
        if (ser_gps.readable())
        {
            char c = ser_gps.getc();
            tinygps.encode(c);
            pc.putc(c);
        }
        if (tinygps.gga_ready() && tinygps.rmc_ready())
        {
            break;
        }
    }
    hdop = tinygps.f_hdop();
    tinygps.f_get_position(&latitude, &longitude);
    altitude = tinygps.f_altitude();
    speed = tinygps.f_speed_kmph();
    tinygps.crack_datetime(&year, &month, &day, &hour, &minute, &second);
#else

    nmea.clear();
    gps.listen();
    while (1)
    {
        if (gps.available())
        {
            char c = gps.read();
            INFC(c);
            nmea.process(c);
        }
        if (nmea.isValid())
        {
            if (nmea.getHDOP() > 0 && nmea.getHDOP() < 255 )
                if (nmea.getSpeed() != LONG_MIN)
                    break;
        }
    }
    INFLN("");

    if (nmea.isValid())
    {
        INFLN("GPS valid");

        hdop = nmea.getHDOP()/10.0;
        long latitude_mdeg = nmea.getLatitude();
        latitude = latitude_mdeg / 1000000.0;
        long longitude_mdeg = nmea.getLongitude();
        longitude = longitude_mdeg / 1000000.0;

        long alt;
        if (nmea.getAltitude(alt))
        {
            altitude = alt / 1000.0;
        }
        else
        {
            altitude = 0;
            INFLN("altitude not available");
        }

        speed = nmea.getSpeed();
//        INF("Speed (mKnots): ");
//        INFVAR(speed);
//        INFLN("");

        if (speed == LONG_MIN)
        {
            speed = -1;
        }
        else
        {
            // 1 Knots -> 1.85 km/h
            // 1000 / 1.85 = 541
            speed /= 541;
        }

        year = nmea.getYear();
        month = nmea.getMonth();
        day = nmea.getDay();

        hour = nmea.getHour();
        minute = nmea.getMinute();
        second = nmea.getSecond();
    }
    else
    {
        INFLN("GPS invalid");
        return;
    }

#endif

#else
    while (1)
    {
        bool gps_ok = fona.getGPS(&latitude, &longitude, &speed, &heading, &altitude);
        if (gps_ok)
        {
            break;
        }
        wait(2);
    }
#endif

    INF("HDOP: ");
    INFVAR(hdop);
    INFLN("");

    INF("Latitude (deg): ");
    INFVAR(latitude);
    INFLN("");

    INF("Longitude (deg): ");
    INFVAR(longitude);
    INFLN("");

    INF("altitude: ");
    INFVAR(altitude);
    INFLN("");

    INF("Speed: ");
    INFVAR(speed);
    INFLN("");

    //    INFLN("sat_count: %f ", tinygps.sat_count());

    INF("=== TIME: ");
    INFVAR(year);
    INF(".");
    INFVAR(month);
    INF(".");
    INFVAR(day);
    INF(" ");
    INFVAR(hour);
    INF(":");
    INFVAR(minute);
    INF(":");
    INFVAR(second);
    INFLN(" ===");
}
