#pragma once

#include "FonaLight.h"

#ifdef __MBED__
Adafruit_FONA fona = FonaLight(PIN_FONA_TX, PIN_FONA_RX, PIN_FONA_RST, PIN_FONA_RI);
#else
SoftwareSerial fonaSS = SoftwareSerial(PIN_FONA_TX, PIN_FONA_RX);
//SoftwareSerial *fonaSerial = &fonaSS;
//Adafruit_FONA fona = Adafruit_FONA(PIN_FONA_RST);
FonaLight fona(PIN_FONA_RST);
#endif

double gsmlat = 0;
double gsmlon = 0;
uint16_t vbat;
uint8_t rssi;

void print_network_status(uint8_t n)
{
    INF("Network status:");
    INFVAR(n);
    INFLN("");

    if (n == 0)
        INFLN("Not registered");
    if (n == 1)
        INFLN("Registered (home)");
    if (n == 2)
        INFLN("Not registered (searching)");
    if (n == 3)
        INFLN("Denied");
    if (n == 4)
        INFLN("Unknown");
    if (n == 5)
        INFLN("Registered roaming");
}

bool sim_available()
{
    return fona.
#ifdef __MBED__
            readable()
#else
            available()
#endif
    ;
}

char sim_read()
{
    return fona.
#ifdef __MBED__
            getc()
#else
            read()
#endif
    ;
}

void sim_init()
{
    /////////////////////////////////////////////////////////
    // GSM
    /////////////////////////////////////////////////////////
    INFLN("FONA basic test");
    INFLN("Initializing....(May take 3 seconds)");

    bool ok;
#ifdef __MBED__
    ok = fona.begin(9600));
#else
    fonaSS.begin(9600);
    fonaSS.listen();
    ok = fona.begin(fonaSS);
#endif
    if (!ok)
    {
        INFLN("Couldn't find FONA");
//        wait(3);
        reboot();
//        error_beep (ERR_NO_FONA);
    }
    INFLN("FONA is OK");
    beep_status(0, false);

#ifndef EXT_GPS
    wait(1);
    INFLN("enable GPS..");
    fona.enableGPS(true);
#endif

    wait(1);

    fona.setGPRSNetworkSettings(F(APN));

    // Print SIM card IMEI number.
    char imei[16] =
    { 0 }; // MUST use a 16 character buffer for IMEI!
    INFLN("read IMEI..");
    uint8_t imeiLen = fona.getIMEI(imei); //fona is an instance of Adafruit_FONA
    if (imeiLen > 0)
    {
        INF("SIM card IMEI: ");
        INFVAR(imei);
        INFLN("");
    }
    else
    {
        INFLN("SIM card IMEI missing! ");
        error_beep (ERR_NO_IMEI);
    }

    beep_status(1, false);

    wait(1);

#ifdef SIM_PIN
    if (! fona.unlockSIM(SIM_PIN))
    {
        INFLN("Unlock Failed" );
        error_beep(ERR_UNLOCK);
    }
    INFLN("Unlock  OK!" );
    beep_status(2, false);
#endif

    wait(6);
    while (1)
    { //searching
        uint8_t n = fona.getNetworkStatus();
        print_network_status(n);
        if (n != 2)
            break;
        INFLN("network_status: searching");
        wait(2);
        beep_status(3, true);
    }
    beep_status(3, false);

//    print_network_status(fona.getNetworkStatus());

    wait(2);

    // turn GPRS on
    while (!fona.enableGPRS(true))
    {
        INFLN("Failed to turn on GPRS");
        wait(2);
        beep_status(4, true);
    }
    INFLN("GPRS OK");
    beep_status(4, false);

//    fona.callerIdNotification(true); // TODO: check why false alarm

}

void http_test()
{
    fonaSS.listen();

    INFLN("http test.. ");
    for (int i = 0; i < 20; i++)
    {
        uint16_t statuscode;
        int16_t length;

        bool ok = fona.HTTP_GET_start(
                "http://www.brainjar.com/java/host/test.html", &statuscode,
                (uint16_t *) &length);
        if (!ok)
        {
            INFLN("Failed!");
            beep_status(5, true);

        }
        else
        {
            INFLN("OK!");
            beep_status(5, false);

            while (length > 0)
            {
                while (sim_available())
                {
                    char c = sim_read();

                    INFC(c);
                    length--;
                    if (!length)
                        break;
                }
            }
        }
        fona.HTTP_GET_end();
        if (ok)
        {
            break;
        }
        wait(3);
    }
}

void sim_read_gsm_pos()
{
    fonaSS.listen();

    INFLN("waiting for GSM position ");

    for (int i = 0; i < 3; i++)
    {
        float tmp_lon; // TODO: double
        float tmp_lat;
        bool ok = fona.getGSMLoc(&tmp_lat, &tmp_lon);
        if (tmp_lon < 0)
        {
            ok = false;
        }
        if (tmp_lat < 0)
        {
            ok = false;
        }
        if (ok)
        {
            gsmlon = tmp_lon;
            gsmlat = tmp_lat;
            INF("GSMLoc (lat,lon) = ");
            INFVAR(gsmlat);
            INF(" ");
            INFVAR(gsmlon);
            INFLN("");
            break;
        }
        else
        {
            INFLN("getGSMLoc Failed!");
            wait(3);
        }
    }
}
/*
boolean getGSMLoc()
{
    char gpsbuffer[70];

    uint16_t returncode;

    // make sure we could get a response
    if (!fona.getGSMLoc(&returncode, gpsbuffer, sizeof(gpsbuffer)))
        return false;

    // make sure we have a valid return code
    if (returncode != 0)
        return false;

    // +CIPGSMLOC: 0,-74.007729,40.730160,2015/10/15,19:24:55
    // tokenize the gps buffer to locate the lat & long
    longp = strtok(gpsbuffer, ",");
    if (!longp)
        return false;

    latp = strtok(NULL, ",");
    if (!latp)
        return false;

    return true;

}
*/
void sim_read_rssi()
{
    fonaSS.listen();

    rssi = fona.getRSSI();
    INF("RSSI = ");
    INFVAR(rssi);
    INFLN("");

}

void sim_read_battery()
{
    fonaSS.listen();

    // read the battery voltage
    if (!fona.getBattPercent(&vbat))
    {
        INFLN("Failed to read Batt");
        wait(10);
        reboot();
    }
    else
    {
        INF("VBat = ");
        INFVAR(vbat);
        INFLN("");
    }
}
