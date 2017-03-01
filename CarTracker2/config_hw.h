#pragma once

#define     EXT_GPS


/*
Board                                       Digital Pins Usable For Interrupts
===============================================================================
Uno, Nano, Mini, other 328-based            2, 3
Mega, Mega2560, MegaADK                     2, 3, 18, 19, 20, 21
Micro, Leonardo, other 32u4-based           0, 1, 2, 3, 7
 */


// pin def

// FONA TX: TX on FONA!
// FONA RX: RX on FONA!

#ifdef __MBED__

#define PIN_GPS_RX   PB_11
//#define PIN_GPS_TX   ???

#define PIN_FONA_RX  D2
#define PIN_FONA_TX  D8
#define PIN_FONA_RST D6
#define PIN_FONA_RI  D5
// I2C SDA   D14
// I2C SCL   D15
#define PIN_DEBUG_RX  D0
#define PIN_DEBUG_TX  D1
#define PIN_BUTTON  D7
#define PIN_SPK A3

#else

#define PIN_DEBUG_RX  D0
#define PIN_DEBUG_TX  D1

#define PIN_GPS_RX   D8
#define PIN_GPS_TX   D9

#define PIN_FONA_RX  D5
#define PIN_FONA_TX  D3
#define PIN_FONA_RST D4
#define PIN_FONA_RI  D2
// I2C SDA   A4
// I2C SCL   A5
#define PIN_BUTTON  A2

// optional
#define PIN_SPK A3

#endif

//  without MPU_INT:
//#define PIN_MPU_INT   D10


//#define PIN_FONA_RX 	5
//#define PIN_FONA_TX 	3
//#define PIN_FONA_RST 	4
//
//#define PIN_SPEAKER 	12
//
//#define PIN_GPS_RX     10
//#define PIN_GPS_TX     11

// MPU6050
// D2 	interrupt
//#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards

// A4	SDA
// A5	SCL



