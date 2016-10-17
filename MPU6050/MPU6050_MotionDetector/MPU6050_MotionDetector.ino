// read temperature from IC

#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define PIN_LED LED_BUILTIN


//#define MPU6050_INTERRUPT_MOT_BIT           6
#define  MPU6050_INTERRUPT_MOT_BIT_MASK (1 << MPU6050_INTERRUPT_MOT_BIT)

//// MOT_THR register
//uint8_t getMotionDetectionThreshold();
//void setMotionDetectionThreshold(uint8_t threshold);
//
//// MOT_DUR register
//uint8_t getMotionDetectionDuration();
//void setMotionDetectionDuration(uint8_t duration);

//        bool getIntMotionEnabled();
//        void setIntMotionEnabled(bool enabled);
//
//        bool getIntMotionStatus();
//
//        uint8_t getMotionDetectionCounterDecrement();
//        void setMotionDetectionCounterDecrement(uint8_t decrement);

MPU6050 mpu;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void interruptHandler() {
    mpuInterrupt = true;
}

void setup() {
    pinMode(PIN_LED, OUTPUT);

    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    mpu.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // enable Arduino interrupt detection
    Serial.print(F("Enabling interrupt detection on pin "));
    Serial.println(INTERRUPT_PIN);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interruptHandler, RISING);
    mpuIntStatus = mpu.getIntStatus();

    /** 
     * This register configures the duration counter threshold for Motion interrupt
     * generation. The duration counter ticks at 1 kHz, therefore MOT_DUR has a unit
     * of 1LSB = 1ms. The Motion detection duration counter increments when the
     * absolute value of any of the accelerometer measurements exceeds the Motion
     * detection threshold (Register 31). The Motion detection interrupt is
     * triggered when the Motion detection counter reaches the time count specified
     * in this register.
     *
     * For more details on the Motion detection interrupt, see Section 8.3 of the
     * MPU-6000/MPU-6050 Product Specification document.
     *
     */
    mpu.setMotionDetectionDuration(2);
    
    /** 
     * This register configures the detection threshold for Motion interrupt
     * generation. The unit of MOT_THR is 1LSB = 2mg. Motion is detected when the
     * absolute value of any of the accelerometer measurements exceeds this Motion
     * detection threshold. This condition increments the Motion detection duration
     * counter (Register 32). The Motion detection interrupt is triggered when the
     * Motion Detection counter reaches the time count specified in MOT_DUR
     * (Register 32).
     *
     * The Motion interrupt will indicate the axis and polarity of detected motion
     * in MOT_DETECT_STATUS (Register 97).
     *
     * For more details on the Motion detection interrupt, see Section 8.3 of the
     * MPU-6000/MPU-6050 Product Specification document as well as Registers 56 and
     * 58 of this document.

     */
    mpu.setMotionDetectionThreshold(1);

    /** 
     * The DHPF is a filter module in the path leading to motion detectors (Free
     * Fall, Motion threshold, and Zero Motion). The high pass filter output is not
     * available to the data registers (see Figure in Section 8 of the MPU-6000/
     * MPU-6050 Product Specification document).
     *
     * The high pass filter has three modes:
     *
     * <pre>
     *    Reset: The filter output settles to zero within one sample. This
     *           effectively disables the high pass filter. This mode may be toggled
     *           to quickly settle the filter.
     *
     *    On:    The high pass filter will pass signals above the cut off frequency.
     *
     *    Hold:  When triggered, the filter holds the present sample. The filter
     *           output will be the difference between the input sample and the held
     *           sample.
     * </pre>
     *
     * <pre>
     * ACCEL_HPF | Filter Mode | Cut-off Frequency
     * ----------+-------------+------------------
     * 0         | Reset       | None
     * 1         | On          | 5Hz
     * 2         | On          | 2.5Hz
     * 3         | On          | 1.25Hz
     * 4         | On          | 0.63Hz
     * 7         | Hold        | None
     * </pre>
     *
     * @return Current high-pass filter configuration
     * @see MPU6050_DHPF_RESET
     * @see MPU6050_RA_ACCEL_CONFIG
     */
    mpu.setDHPFMode(4);
    
    
    
    /** 
     * Detection is registered by the Motion detection module after accelerometer
     * measurements meet their respective threshold conditions over a specified
     * number of samples. When the threshold conditions are met, the corresponding
     * detection counter increments by 1. The user may control the rate at which the
     * detection counter decrements when the threshold condition is not met by
     * configuring MOT_COUNT. The decrement rate can be set according to the
     * following table:
     *
     * <pre>
     * MOT_COUNT | Counter Decrement
     * ----------+------------------
     * 0         | Reset
     * 1         | 1
     * 2         | 2
     * 3         | 4
     * </pre>
     *
     * When MOT_COUNT is configured to 0 (reset), any non-qualifying sample will
     * reset the counter to 0. For further information on Motion detection,
     * please refer to Registers 29 to 32.
     *
     */
    mpu.setMotionDetectionCounterDecrement(0);
    
    
    mpu.setIntMotionEnabled(1);
    
    
    Serial.print("getIntEnabled ");        
    Serial.println(mpu.getIntEnabled());        
    
    Serial.print("getIntMotionEnabled ");        
    Serial.println(mpu.getIntMotionEnabled());        
    
    Serial.print("getMotionDetectionThreshold ");        
    Serial.println(mpu.getMotionDetectionThreshold());        
    
    Serial.print("getMotionDetectionDuration ");        
    Serial.println(mpu.getMotionDetectionDuration());        
    
    Serial.print("getMotionDetectionCounterDecrement ");        
    Serial.println(mpu.getMotionDetectionCounterDecrement());        
    
    Serial.print("getDHPFMode ");        
    Serial.println(mpu.getDHPFMode());        
    
    Serial.print("getIntMotionStatus ");        
    Serial.println(mpu.getIntMotionStatus());        
    

}
int led=0;
int16_t ax, ay, az;
int16_t gx, gy, gz;

void loop() {
    while (!mpuInterrupt) {
    }
    led=!led;
    digitalWrite(PIN_LED, led);   
    
    // reset interrupt flag 
    mpuInterrupt = false;

    // get INT_STATUS byte
    mpuIntStatus = mpu.getIntStatus();
    
    if (mpuIntStatus & MPU6050_INTERRUPT_MOT_BIT_MASK) {
        Serial.print("motion interrupt ");        

        // read raw accel/gyro measurements from device
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        Serial.print("ax ay az / gx gy gz:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.print(gz);
    }
//    if (mpuIntStatus & ~MPU6050_INTERRUPT_MOT_BIT_MASK) {
//        Serial.print("other interrupts ");        
//        Serial.print(mpuIntStatus);        
//    }
    Serial.println();
    
    delay(100);

    // reset interrupt flag 
    mpuInterrupt = false;
}
