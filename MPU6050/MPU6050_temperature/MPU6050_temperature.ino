// read temperature from IC

#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 accelgyro;

void setup()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(
            accelgyro.testConnection() ?
                    "MPU6050 connection successful" :
                    "MPU6050 connection failed");

    accelgyro.setTempSensorEnabled(1);
}

void loop()
{
    int16_t t = accelgyro.getTemperature();
    Serial.print("temperature: ");
//    Serial.print(t);
//    Serial.print("  ");
    Serial.print(t / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
    Serial.println(" C");

    delay(2000);
}
