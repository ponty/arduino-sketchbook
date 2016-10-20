#include <MicroNMEA.h>
#include <SoftwareSerial.h>

SoftwareSerial gps(10, 11); // RX, TX
#define console  Serial

char nmeaBuffer[85];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

unsigned long previousMillis = 0;
const long interval = 3000;

void setup(void)
{
	console.begin(115200);
	gps.begin(9600);

	// Compatibility mode off
	// MicroNMEA::sendSentence(gps, "$PONME,2,6,1,0");
//  MicroNMEA::sendSentence(gps, "$PONME,,,1,0");

	// Clear the list of messages which are sent.
//  MicroNMEA::sendSentence(gps, "$PORZB");

	// Send RMC and GGA messages.
//  MicroNMEA::sendSentence(gps, "$PORZB,RMC,1,GGA,1");
	// MicroNMEA::sendSentence(gps, "$PORZB,RMC,1,GGA,1,GSV,1");

}

void loop(void)
{
    while (gps.available())
    {
        char c = gps.read();
        console.print(c);
        nmea.process(c);
    }
    
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval)
	{
		previousMillis = currentMillis;
		
		// MicroNMEA::sendSentence(gps, "$PONME,2,6,1,0");

		// Output GPS information from previous second
		console.print("Valid fix: ");
		console.println(nmea.isValid() ? "yes" : "no");
		
        if (!nmea.isValid())
          return;
        
		console.print("Nav. system: ");
		if (nmea.getNavSystem())
			console.println(nmea.getNavSystem());
		else
			console.println("none");

		console.print("Num. satellites: ");
		console.println(nmea.getNumSatellites());

		console.print("HDOP: ");
		console.println(nmea.getHDOP() / 10., 1);

		console.print("Date/time: ");
		console.print(nmea.getYear());
		console.print('-');
		console.print(int(nmea.getMonth()));
		console.print('-');
		console.print(int(nmea.getDay()));
		console.print('T');
		console.print(int(nmea.getHour()));
		console.print(':');
		console.print(int(nmea.getMinute()));
		console.print(':');
		console.println(int(nmea.getSecond()));

		long latitude_mdeg = nmea.getLatitude();
		long longitude_mdeg = nmea.getLongitude();
		console.print("Latitude (deg): ");
		console.println(latitude_mdeg / 1000000., 6);

		console.print("Longitude (deg): ");
		console.println(longitude_mdeg / 1000000., 6);

		long alt;
		console.print("Altitude (m): ");
		if (nmea.getAltitude(alt))
			console.println(alt / 1000., 3);
		else
			console.println("not available");

		console.print("Speed: ");
		console.println(nmea.getSpeed() / 1000., 3);
		console.print("Course: ");
		console.println(nmea.getCourse() / 1000., 3);

		console.println("-----------------------");
	}

}
