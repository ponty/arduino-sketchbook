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

// Hardware serial is also possible!
//  HardwareSerial *fonaSerial = &Serial1;

// Use this for FONA 800 and 808s
Adafruit_FONA fona = Adafruit_FONA(PIN_FONA_RST);
// Use this one for FONA 3G
//Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);

//uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

uint8_t type;

void setup()
{
    while (!Serial)
        ;

    Serial.begin(115200);
    
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    
    /////////////////////////////////////////////////////////
    // MPU6050
    /////////////////////////////////////////////////////////
    
    // initialize device
//    Serial.println("Initializing I2C devices...");
    mpu.initialize();

    // verify connection
//    Serial.println("Testing device connections...");
//    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // enable Arduino interrupt detection
//    Serial.print(F("Enabling interrupt detection on pin "));
//    Serial.println(INTERRUPT_PIN);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interruptHandler, RISING);
    mpuIntStatus = mpu.getIntStatus();

    mpu.setMotionDetectionDuration(MOTION_DETECTION_DURATION);
    mpu.setMotionDetectionThreshold(MOTION_DETECTION_THRESHOLD);
    mpu.setDHPFMode(DHPF_MODE);
    mpu.setMotionDetectionCounterDecrement(MOTION_DETECTION_COUNTER_DECREMENT);
    mpu.setIntMotionEnabled(1);
    
    
//    Serial.print("getIntEnabled ");        
//    Serial.println(mpu.getIntEnabled());        
//    
//    Serial.print("getIntMotionEnabled ");        
//    Serial.println(mpu.getIntMotionEnabled());        
//    
//    Serial.print("getMotionDetectionThreshold ");        
//    Serial.println(mpu.getMotionDetectionThreshold());        
//    
//    Serial.print("getMotionDetectionDuration ");        
//    Serial.println(mpu.getMotionDetectionDuration());        
//    
//    Serial.print("getMotionDetectionCounterDecrement ");        
//    Serial.println(mpu.getMotionDetectionCounterDecrement());        
//    
//    Serial.print("getDHPFMode ");        
//    Serial.println(mpu.getDHPFMode());        
    
//    Serial.print("getIntMotionStatus ");        
//    Serial.println(mpu.getIntMotionStatus());        
    

    
    /////////////////////////////////////////////////////////
    // GSM
    /////////////////////////////////////////////////////////
    Serial.println(F("FONA basic test"));
    Serial.println(F("Initializing....(May take 3 seconds)"));

    fonaSerial->begin(4800);
    if (!fona.begin(*fonaSerial))
    {
        Serial.println(F("Couldn't find FONA"));
        while (1)
            ;
    }
    type = fona.type();
    Serial.println(F("FONA is OK"));
    Serial.print(F("Found "));
    switch (type)
    {
    case FONA800L:
        Serial.println(F("FONA 800L"));
        break;
    case FONA800H:
        Serial.println(F("FONA 800H"));
        break;
    case FONA808_V1:
        Serial.println(F("FONA 808 (v1)"));
        break;
    case FONA808_V2:
        Serial.println(F("FONA 808 (v2)"));
        break;
    case FONA3G_A:
        Serial.println(F("FONA 3G (American)"));
        break;
    case FONA3G_E:
        Serial.println(F("FONA 3G (European)"));
        break;
    default:
        Serial.println(F("???"));
        break;
    }

    // Print module IMEI number.
//    char imei[15] =
//    { 0 }; // MUST use a 16 character buffer for IMEI!
//    uint8_t imeiLen = fona.getIMEI(imei);
//    if (imeiLen > 0)
//    {
//        Serial.print("Module IMEI: ");
//        Serial.println(imei);
//    }

    // Optionally configure a GPRS APN, username, and password.
    // You might need to do this to access your network's GPRS/data
    // network.  Contact your provider for the exact APN, username,
    // and password values.  Username and password are optional and
    // can be removed, but APN is required.
    //fona.setGPRSNetworkSettings(F("your APN"), F("your username"), F("your password"));
    fona.setGPRSNetworkSettings(F(APN));

    // Optionally configure HTTP gets to follow redirects over SSL.
    // Default is not to follow SSL redirects, however if you uncomment
    // the following line then redirects over SSL will be followed.
    //fona.setHTTPSRedirect(true);

    //printMenu();

    delay(1000);

    Serial.print(F("RSSI = "));
    Serial.println(fona.getRSSI());

    // enable network time sync
//	if (!fona.enableNetworkTimeSync(true))
//		Serial.println(F("Failed to enable network time sync"));

#ifdef SIM_PIN  
    while (! fona.unlockSIM(SIM_PIN))
    {
        Serial.println(F("Unlock Failed"));
        delay(1000);
    }
    Serial.println(F("Unlock  OK!"));
#endif

    while (read_network_status() == 2)	//searching
    {
        delay(1000);
        tone(PIN_SPEAKER, 500, 10);
    }

    // turn GPRS on
    while (!fona.enableGPRS(true))
    {
        Serial.println(F("Failed to turn on GPRS"));
        delay(1000);
    }
    Serial.println(F("GPRS OK"));

    // read the network/cellular status
    uint8_t n = fona.getNetworkStatus();
    Serial.print(F("Network status "));
    Serial.print(n);
    Serial.print(F(": "));
    if (n == 0)
        Serial.println(F("Not registered"));
    if (n == 1)
        Serial.println(F("Registered (home)"));
    if (n == 2)
        Serial.println(F("Not registered (searching)"));
    if (n == 3)
        Serial.println(F("Denied"));
    if (n == 4)
        Serial.println(F("Unknown"));
    if (n == 5)
        Serial.println(F("Registered roaming"));

}

int read_network_status()
{
    // read the network/cellular status
    uint8_t n = fona.getNetworkStatus();
    Serial.print(F("Network status "));
    Serial.print(n);
    Serial.print(F(": "));
    if (n == 0)
        Serial.println(F("Not registered"));
    if (n == 1)
        Serial.println(F("Registered (home)"));
    if (n == 2)
        Serial.println(F("Not registered (searching)"));
    if (n == 3)
        Serial.println(F("Denied"));
    if (n == 4)
        Serial.println(F("Unknown"));
    if (n == 5)
        Serial.println(F("Registered roaming"));
    return n;
}

uint16_t vbat;
uint8_t rssi;
uint8_t motion;
//float gsmlat;
//float gsmlon;
float temp;
char gpsbuffer[70];
char *longp=0;
char *latp=0;

void loop()
{
    motion=0;
    if (mpuInterrupt)
    {
        // reset interrupt flag 
        mpuInterrupt = false;

        // get INT_STATUS byte
        mpuIntStatus = mpu.getIntStatus();

        if (mpuIntStatus & MPU6050_INTERRUPT_MOT_BIT_MASK)
        {
            Serial.println("motion interrupt ");
        }
        motion=1;
    }
    
    int16_t t = mpu.getTemperature();
    temp = t / 340.00 + 36.53; //equation for temperature in degrees C from datasheet
    Serial.print("temperature: ");
    Serial.print(temp); 
    Serial.println(" C");
    
    
    // read the battery voltage
    if (!fona.getBattVoltage(&vbat))
    {
        Serial.println(F("Failed to read Batt"));
        while(1)
            ;
    }
    else
    {
        Serial.print(F("VBat = "));
        Serial.print(vbat);
        Serial.println(F(" mV"));
    }

    rssi = fona.getRSSI();
    Serial.print(F("RSSI = "));
    Serial.println(rssi);


    boolean ok = getGSMLoc();
    if (ok)
    {
        Serial.print(F("GSMLoc (lat,lon) = "));
        Serial.print(latp);
        Serial.print(F(" "));
        Serial.println(longp);       
    }
    else
    {
        Serial.println(F("getGSMLoc Failed!"));
        while(1)
            ;
    }

//    phant.add("a", 72);
//	phant.url()

    // read website URL
    uint16_t statuscode;
    int16_t length;
//	char url[80];

//	flushSerial();
//	Serial.println(F("NOTE: in beta! Use small webpages to read!"));
//	Serial.println(
//			F("URL to read (e.g. www.adafruit.com/testwifi/index.html):"));
//	Serial.print(F("http://"));
//	readline(url, 79);
//	Serial.println(url);

//    tone(PIN_SPEAKER, 200, 10);
    Serial.println(F("****"));

//    Serial.print(F("url: "));
//    Serial.println(phant.url());
//    
//    String xx = phant.url();
//    Serial.print(F("url: "));
//    Serial.println((char*) (xx.c_str()));
    
//    Serial.print(F("url: "));
//    Serial.println((char*) (phant.url().c_str()));
//    while(1);
//    return;
   
    if (!HTTP_GET_start(&statuscode, (uint16_t *) &length))
//    if (!fona.HTTP_GET_start((char*) (phant.url().c_str()), &statuscode,
//            (uint16_t *) &length))
    {
        tone(PIN_SPEAKER, 1000, 100);
        Serial.println("Failed!");
    }
    else
        while (length > 0)
        {
            while (fona.available())
            {
                char c = fona.read();

                // Serial.write is too slow, we'll write directly to Serial register!
//#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//			loop_until_bit_is_set(UCSR0A, UDRE0); // Wait until data register empty. 
//			UDR0 = c;
//#else
                Serial.write(c);
//#endif
                length--;
                if (!length)
                    break;
            }
        }
    Serial.println(F("\n****"));
    fona.HTTP_GET_end();
    tone(PIN_SPEAKER, 400, 10);

    for (int i = 0; i < 10; i++)
    {
        delay(1000);	// 1 sec
    }
    
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

