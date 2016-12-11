/***************************************************



 ****************************************************/

#include "secret.h"
#include "config.h"
#include "hwconfig.h"

//#include <Phant.h>

//Phant phant(PHANT_HOST, PHANT_PUBLIC_KEY, PHANT_PRIVATE_KEY);

#include "Adafruit_FONA.h"

#include "I2Cdev.h"
#include "MPU6050.h"

#include <MicroNMEA.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#define  MPU6050_INTERRUPT_MOT_BIT_MASK (1 << MPU6050_INTERRUPT_MOT_BIT)

MPU6050 mpu;
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void interruptHandler() {
    mpuInterrupt = true;
}

//#include "accelero.h"

// this is a large buffer for replies
//char replybuffer[255];

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines 
// and uncomment the HardwareSerial line
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(PIN_FONA_TX, PIN_FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

SoftwareSerial gps(PIN_GPS_RX, PIN_GPS_TX); // RX, TX
char nmeaBuffer[85];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

// Hardware serial is also possible!
//  HardwareSerial *fonaSerial = &Serial1;

Adafruit_FONA fona = Adafruit_FONA(PIN_FONA_RST);

//uint8_t type;

#define HALT()    while(1);


void setup()
{
    while (!Serial)
        ;

    console.begin(115200);
    
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    /////////////////////////////////////////////////////////
    // GPS
    /////////////////////////////////////////////////////////
    gps.begin(9600);

    /////////////////////////////////////////////////////////
    // MPU6050
    /////////////////////////////////////////////////////////
    
    // initialize device
    console.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    console.println(F("Testing device connections..."));
    if(mpu.testConnection())
    {
        console.println(F("MPU6050 connection successful"));        
    }
    else
    {
        console.println(F("MPU6050 connection failed"));  
        HALT();
    }

    // enable Arduino interrupt detection
    console.print(F("Enabling interrupt detection on pin "));
    console.println(INTERRUPT_PIN);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interruptHandler, RISING);
    mpuIntStatus = mpu.getIntStatus();

    mpu.setMotionDetectionDuration(MOTION_DETECTION_DURATION);
    mpu.setMotionDetectionThreshold(MOTION_DETECTION_THRESHOLD);
    mpu.setDHPFMode(DHPF_MODE);
    mpu.setMotionDetectionCounterDecrement(MOTION_DETECTION_COUNTER_DECREMENT);
    mpu.setIntMotionEnabled(1);
       
    /////////////////////////////////////////////////////////
    // GSM
    /////////////////////////////////////////////////////////
    console.println(F("FONA basic test"));
    console.println(F("Initializing....(May take 3 seconds)"));

    fonaSerial->begin(4800);
    
    if (!fona.begin(*fonaSerial))
    {
        console.println(F("Couldn't find FONA"));
        while (1)
            ;
    }
    console.println(F("FONA is OK"));

    fona.setGPRSNetworkSettings(F(APN));

#ifdef SIM_PIN  
    if (! fona.unlockSIM(SIM_PIN))
    {
        console.println(F("Unlock Failed"));
        HALT();
    }
    console.println(F("Unlock  OK!"));
#endif

    while (read_network_status() == 2)	//searching
    {
        console.println(F("network_status: searching"));
        delay(1000);
        tone(PIN_SPEAKER, 500, 10);
    }

    // read the network/cellular status
    uint8_t n = fona.getNetworkStatus();
    console.print(F("Network status "));
    console.print(n);
    console.print(F(": "));
    if (n == 0)
        console.println(F("Not registered"));
    if (n == 1)
        console.println(F("Registered (home)"));
    if (n == 2)
        console.println(F("Not registered (searching)"));
    if (n == 3)
        console.println(F("Denied"));
    if (n == 4)
        console.println(F("Unknown"));
    if (n == 5)
        console.println(F("Registered roaming"));

    // turn GPRS on
    while (!fona.enableGPRS(true))
    {
        console.println(F("Failed to turn on GPRS"));
        delay(2000);
    }
    console.println(F("GPRS OK"));

}

int read_network_status()
{
    // read the network/cellular status
    uint8_t n = fona.getNetworkStatus();
    console.print(F("Network status "));
    console.print(n);
    console.print(F(": "));
    if (n == 0)
        console.println(F("Not registered"));
    if (n == 1)
        console.println(F("Registered (home)"));
    if (n == 2)
        console.println(F("Not registered (searching)"));
    if (n == 3)
        console.println(F("Denied"));
    if (n == 4)
        console.println(F("Unknown"));
    if (n == 5)
        console.println(F("Registered roaming"));
    return n;
}

uint16_t vbat;
uint8_t rssi;
uint8_t motion;
float temp;
char gpsbuffer[70];
char *longp=0;
char *latp=0;

unsigned long previousMillis = 0;

long latitude_mdeg = 0;
long longitude_mdeg = 0;
long speed = 0;
uint8_t hdop = 0;

void loop()
{
   
    if(!mpuInterrupt)
    {
        return;
    }

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis < 10000)
    {
        return;
    }
    
    nmea.clear();
    gps.listen();
    while (1)
    {
        if(gps.available())
        {
            char c = gps.read();
            console.print(c);
            nmea.process(c);
        }
        if(nmea.getHDOP()<255)
            break;
    }
    console.println();
    
    fonaSS.listen();

    previousMillis = currentMillis;
    
    motion=0;
    if (mpuInterrupt)
    {
        // reset interrupt flag 
        mpuInterrupt = false;

        // get INT_STATUS byte
        mpuIntStatus = mpu.getIntStatus();

        if (mpuIntStatus & MPU6050_INTERRUPT_MOT_BIT_MASK)
        {
            console.println("motion interrupt ");
        }
        motion=1;
    }
    
    int16_t t = mpu.getTemperature();
    temp = t / 340.00 + 36.53; //equation for temperature in degrees C from datasheet
    console.print("temperature: ");
    console.print(temp); 
    console.println(" C");
    
    // GPS
    if (nmea.isValid())
    {
        console.println(F("GPS valid"));

        hdop = nmea.getHDOP();
        console.print(F("HDOP:"));
        console.println(hdop);
         
        latitude_mdeg = nmea.getLatitude();
        console.print(F("Latitude (deg): "));
        console.println(latitude_mdeg);

        longitude_mdeg = nmea.getLongitude();
        console.print(F("Longitude (deg): "));
        console.println(longitude_mdeg);

//        long alt;
//        console.print(F("Altitude (m): "));
//        if (nmea.getAltitude(alt))
//            console.println(alt / 1000., 3);
//        else
//            console.println(F("not available"));
        
        speed = nmea.getSpeed();
        console.print(F("Speed: "));
        console.println(speed);

    }
    else
    {
        console.println(F("GPS invalid"));
        return;
    }
        
    // read the battery voltage
    if (!fona.getBattVoltage(&vbat))
    {
        console.println(F("Failed to read Batt"));
        delay(1000);
        return;
    }
    else
    {
        console.print(F("VBat = "));
        console.print(vbat);
        console.println(F(" mV"));
    }

    rssi = fona.getRSSI();
    console.print(F("RSSI = "));
    console.println(rssi);


    boolean ok = getGSMLoc();
    if (ok)
    {
        console.print(F("GSMLoc (lat,lon) = "));
        console.print(latp);
        console.print(F(" "));
        console.println(longp);       
    }
    else
    {
        console.println(F("getGSMLoc Failed!"));
        delay(1000);
        return;
    }


    // read website URL
    uint16_t statuscode;
    int16_t length;
//	char url[80];

//	flushSerial();
//	console.println(F("NOTE: in beta! Use small webpages to read!"));
//	console.println(
//			F("URL to read (e.g. www.adafruit.com/testwifi/index.html):"));
//	console.print(F("http://"));
//	readline(url, 79);
//	console.println(url);

//    tone(PIN_SPEAKER, 200, 10);
    console.println(F("sending.."));

   
    if (!HTTP_GET_start(&statuscode, (uint16_t *) &length))
//    if (!fona.HTTP_GET_start((char*) (phant.url().c_str()), &statuscode,
//            (uint16_t *) &length))
    {
        tone(PIN_SPEAKER, 1000, 100);
        console.println("Failed!");
    }
    else
        while (length > 0)
        {
            while (fona.available())
            {
                char c = fona.read();

                // console.write is too slow, we'll write directly to console register!
//#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//			loop_until_bit_is_set(UCSR0A, UDRE0); // Wait until data register empty. 
//			UDR0 = c;
//#else
                console.write(c);
//#endif
                length--;
                if (!length)
                    break;
            }
        }
    console.println(F("\n****"));
    fona.HTTP_GET_end();
    tone(PIN_SPEAKER, 400, 10);


    
}

boolean HTTP_para_url() {
    fona.HTTP_para_start(F("URL"), true);
    
    fonaSS.print(F("http://"));
    fonaSS.print(F(PHANT_HOST));
    fonaSS.print(F("/input/"));
    fonaSS.print(F(PHANT_PUBLIC_KEY));
    fonaSS.print(F(".txt"));
    fonaSS.print(F("?private_key="));
    fonaSS.print(F(PHANT_PRIVATE_KEY));
    
    fonaSS.print(F("&battery="));
    fonaSS.print(vbat);

    fonaSS.print(F("&gsmlat="));
    fonaSS.print(latp);

    fonaSS.print(F("&gsmlon="));
    fonaSS.print(longp);

    fonaSS.print(F("&rssi="));
    fonaSS.print(rssi);
    
    fonaSS.print(F("&temp="));
    fonaSS.print(temp);
    
    fonaSS.print(F("&motion="));
    fonaSS.print(motion);
    
    fonaSS.print(F("&lat="));
    fonaSS.print(latitude_mdeg);

    fonaSS.print(F("&lon="));
    fonaSS.print(longitude_mdeg);

    fonaSS.print(F("&hdop="));
    fonaSS.print(hdop);

    fonaSS.print(F("&speed="));
    fonaSS.print(speed);

    return fona.HTTP_para_end(true);
}

boolean HTTP_setup() {
  // Handle any pending
  fona.HTTP_term();

  // Initialize and set parameters
  if (! fona.HTTP_init())
    return false;
  if (! fona.HTTP_para(F("CID"), 1))
    return false;
  if (! fona.HTTP_para(F("UA"), F("FONA")))
    return false;
  if (! HTTP_para_url())
    return false;

  // HTTPS redirect
//  if (httpsredirect) {
//    if (! HTTP_para(F("REDIR"),1))
//      return false;
//
//    if (! HTTP_ssl(true))
//      return false;
//  }

  return true;
}

boolean HTTP_GET_start(uint16_t *status, uint16_t *datalen){
  if (! HTTP_setup())
    return false;

  // HTTP GET
  if (! fona.HTTP_action(FONA_HTTP_GET, status, datalen, 30000))
    return false;

  DEBUG_PRINT(F("Status: ")); DEBUG_PRINTLN(*status);
  DEBUG_PRINT(F("Len: ")); DEBUG_PRINTLN(*datalen);

  // HTTP response data
  if (! fona.HTTP_readall(datalen))
    return false;

  return true;
}


boolean getGSMLoc() {

  uint16_t returncode;

  // make sure we could get a response
  if (! fona.getGSMLoc(&returncode, gpsbuffer, sizeof(gpsbuffer)))
    return false;

  // make sure we have a valid return code
  if (returncode != 0)
    return false;

  // +CIPGSMLOC: 0,-74.007729,40.730160,2015/10/15,19:24:55
  // tokenize the gps buffer to locate the lat & long
  longp = strtok(gpsbuffer, ",");
  if (! longp) return false;

  latp = strtok(NULL, ",");
  if (! latp) return false;

  return true;

}

