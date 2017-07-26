/* Includes ------------------------------------------------------------------*/
#include "application.h"
#include <Ethernet.h>
#include <EthernetClient.h>

//SYSTEM_MODE(MANUAL);

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

bool success = false;

IPAddress ip(192,168,1,70);

EthernetUDP Udp;

const char message[47] = "My name is Travis and I kick all the asses!\n";

IPAddress broadcast = IPAddress(255,255,255,255);

unsigned long broadcastInterval = 3000;
unsigned long lastBroadcast = 0;


/* This function is called once at start up ----------------------------------*/
void setup()
{
	//Wait for user to get Serial terminal open for monitoring.
	delay(5000);

	// start the Ethernet connection:
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		for (;;)
			;
	}
	// print your local IP address:
	Serial.print("My IP address: ");
	for (byte thisByte = 0; thisByte < 4; thisByte++) {
		// print the value of each byte of the IP address:
		Serial.print(Ethernet.localIP()[thisByte], DEC);
		Serial.print(".");
	}
	Serial.println();
	Particle.process();
}

/* This function loops forever --------------------------------------------*/
void loop()
{

}
