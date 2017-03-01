#pragma once

#include "config_hw.h"
#include "Wire.h"

#define  MPU6050_INTERRUPT_MOT_BIT_MASK (1 << MPU6050_INTERRUPT_MOT_BIT)

MPU6050 mpu;
double temp = 0;

#ifdef PIN_MPU_INT
#ifdef __MBED__
InterruptIn impu(PIN_MPU_INT);
#endif

volatile bool mpuInterrupt = false; // indicates whether MPU interrupt pin has gone high

void interruptHandler()
{
    mpuInterrupt = true;
}
#endif

void mpu_init()
{
    /////////////////////////////////////////////////////////
    // MPU6050
    /////////////////////////////////////////////////////////
    Wire.begin();

    INFLN("MPU6050 initialize ");

    mpu.initialize();
    INFLN("MPU6050 testConnection ");

    bool mpu6050TestResult = mpu.testConnection();
    if (mpu6050TestResult)
    {
        INFLN("MPU6050 test passed ");
    }
    else
    {
        INFLN("MPU6050 test failed ");
    }

    mpu.setMotionDetectionDuration(MOTION_DETECTION_DURATION);
    mpu.setMotionDetectionThreshold(MOTION_DETECTION_THRESHOLD);
    mpu.setDHPFMode(DHPF_MODE);
    mpu.setMotionDetectionCounterDecrement(MOTION_DETECTION_COUNTER_DECREMENT);
    mpu.setIntMotionEnabled(1);

#ifdef PIN_MPU_INT

#ifdef __MBED__
    impu.rise(&interruptHandler);
#else
    attachInterrupt(digitalPinToInterrupt(PIN_MPU_INT), interruptHandler,
            RISING);
#endif

#endif
}

void mpu_read_temperature()
{
    int16_t t = mpu.getTemperature();
    temp = t / 340.00 + 36.53; //equation for temperature in degrees C from datasheet
    INFLN("temperature: ");
    INFVAR(temp);
    INFLN("");

}

bool mpu_is_motion()
{
    // get INT_STATUS byte
    uint8_t mpuIntStatus = mpu.getIntStatus();

    bool motion = (mpuIntStatus & MPU6050_INTERRUPT_MOT_BIT_MASK);
    if (motion)
    {
//        INFLN("motion interrupt ");
    }
    return motion;
}
