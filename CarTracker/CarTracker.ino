/***************************************************



 ****************************************************/

#include <Phant.h>

//#define PHANT_HOST        "data.sparkfun.com"
//#define PHANT_PUBLIC_KEY  "VGb2Y1jD4VIxjX3x196z" 
//#define PHANT_PRIVATE_KEY "9YBaDk6yeMtNErDNq4YM"
//#define		APN 		"internet.vodafone.net"
//#define 	SIM_PIN  		"1234"
#include "secret.h"

Phant phant(PHANT_HOST, PHANT_PUBLIC_KEY, PHANT_PRIVATE_KEY);

#include "Adafruit_FONA.h"

#include "hwconfig.h"

#include "accelero.h"

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
	char imei[15] =
	{ 0 }; // MUST use a 16 character buffer for IMEI!
	uint8_t imeiLen = fona.getIMEI(imei);
	if (imeiLen > 0)
	{
		Serial.print("Module IMEI: ");
		Serial.println(imei);
	}

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
	if (! fona.unlockSIM(SIM_PIN))
	{
		Serial.println(F("Unlock Failed"));
	}
	else
	{
		Serial.println(F("Unlock  OK!"));
	}
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

	// read the time
//	char buffer[23];
//	fona.getTime(buffer, 23); // make sure replybuffer is at least 23 bytes!
//	Serial.print(F("Time = "));
//	Serial.println(buffer);

	// check for GSMLOC (requires GPRS)
//	uint16_t returncode;
//	if (!fona.getGSMLoc(&returncode, replybuffer, 250))
//		Serial.println(F("GSMLOC Failed!"));
//	if (returncode == 0)
//	{
//		Serial.println(replybuffer);
//	}
//	else
//	{
//		Serial.print(F("GSMLOC Fail code #"));
//		Serial.println(returncode);
//	}

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

void loop()
{
    int a = accelero_loop();
    if(a)
    {
    	aaWorld.x;
    }
    
	uint8_t rssi = fona.getRSSI();
	Serial.print(F("RSSI = "));
	Serial.println(rssi);

	// read the battery voltage
	uint16_t vbat;
	if (!fona.getBattVoltage(&vbat))
	{
		Serial.println(F("Failed to read Batt"));
	}
	else
	{
		Serial.print(F("VBat = "));
		Serial.print(vbat);
		Serial.println(F(" mV"));
	}

	float lat;
	float lon;
	boolean ok = fona.getGSMLoc(&lat, &lon);
	if (!ok)
	{
		Serial.println(F("getGSMLoc Failed!"));
	}

	phant.add("a", 72);
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

	tone(PIN_SPEAKER, 200, 10);
	Serial.println(F("****"));
	if (!fona.HTTP_GET_start((char*)(phant.url().c_str()), &statuscode, (uint16_t *) &length))
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
//			loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
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

	
	for(int i=0;i<10;i++)
	{
		delay(1000);	// 1 sec
	}
}

void flushSerial()
{
	while (Serial.available())
		Serial.read();
}
