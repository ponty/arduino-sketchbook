#include "MPU6050.h"
#include "Adafruit_FONA.h"
#include "secret.h"
#include "def.h"
#include "config_hw.h"
#include "config_mpu6050.h"

#ifdef __MBED__
#include "mbedport.h"
#else
#include "arduinoport.h"
#include <SoftwareSerial.h>
#endif

Timer timer_check;
Timer timer_after_motion;

bool event_timer;
bool alarm_mode = false;
bool boot = true;
uint8_t motion;
bool call_event;
int event = 0;

#include "audio.h"
#include "mpu.h"
#include "gps.h"
#include "sim.h"
#include "button.h"
#include "ubidots.h"

void setup()
{
    debug_init();
    INFLN("-------------- CarTracker ------------------------------");

    start_beep();

    button_init();

    timer_check.start();
    timer_after_motion.start();

    gps_init();
    mpu_init();
    sim_init();
//    http_test();

}

void check_call()
{
    char phonenum[22];
    phonenum[0] = 0;
    call_event = fona.incomingCallNumber(phonenum);
}


void loop()
{
    check_call();

    event_timer = false;
    if (timer_check.read() > (30 * 60))
    {
        event_timer = true;
        timer_check.stop();  // defend overflow
    }
    bool after_motion_event = timer_after_motion.read() < (2 * 60);
    if (!after_motion_event)
    {
        timer_after_motion.stop(); // defend overflow
    }

    motion = mpu_is_motion();

    if (!motion && !event_timer && !call_event && !after_motion_event && !boot)
    {
        wait(1);
        return;
    }
    if (timer_check.read() < 5)
    {
        wait(1);
        return;
    }

    INFLN("-----");
    if (motion)
    {
        INFLN("MPU interrupt! ");
    }
    if (event_timer)
    {
        INFLN("event_timer! ");
    }
    if (call_event)
    {
        INFLN("call_event from:");
//        INFVAR(phonenum);
//        INFLN("");
    }
    if (after_motion_event)
    {
        INFLN("after_motion_event! ");
    }

    if (motion)
    {
        timer_after_motion.reset();
        timer_after_motion.start();

        if (alarm_mode)
        {
            INFLN("alarm mode, calling phone");
            fona.callPhone(PHONE_NUMBER);
            wait(9);
            INFLN("end call");
            fona.hangUp();
        }
    }

    mpu_read_temperature();
    read_gps();

    sim_read_battery();
    sim_read_gsm_pos();
    sim_read_rssi();

    // read website URL
    uint16_t statuscode;
    int16_t length;

    if (after_motion_event)
    {
        event = 3;
    }
    if (event_timer)
    {
        event = 4;
    }
    if (call_event)
    {
        event = 5;
    }
    if (motion)
    {
        event = 2;
    }
    if (boot)
    {
        event = 1;
    }


    for (int i = 0; i < 3; i++)
    {
        bool ok =
                fona.HTTP_POST_start_func(
                        &url_func,
                        F("application/json"),
                        //(uint8_t*) data, strlen(data),
                        &postdata_func,
                        &statuscode, (uint16_t *) &length);
        if (!ok || statuscode != 200)
        {
            INFLN("Failed!");
            if (i == 2)
            {
                INFLN("<<< REBOOT >>>");

                reboot();
            }
            wait(10);
        }
        else
        {
            while (length > 0)
            {
                timer_check.reset();
                timer_check.start();
                while (sim_available())
                {
                    char c = sim_read();

                    INFC(c);
                    length--;
                    if (!length)
                        break;
                }
            }
            break;
        }
        fona.HTTP_POST_end();
    }

    // reset interrupt flag
//    mpuInterrupt = false;

    boot = false;

    INFLN("\r\n*********************************");
    beep(2000);

}

