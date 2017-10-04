/* Includes ------------------------------------------------------------------*/
#include "application.h"
#include <Ethernet.h>
#include <EthernetClient.h>

//SYSTEM_MODE(MANUAL);

//Mac address to use for Ethernt interface connection.
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/* This function is called once at start up ----------------------------------*/
void setup()
{
	// start the Ethernet connection and aquire an IP address via DHCP:
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// no point in carrying on, so do nothing forevermore:
		for (;;)
			;
	}
	// print your local IP address:
	Serial.print("My Ethernet IP address: ");
	Serial.println(Ethernet.localIP());
}

/* This function loops forever --------------------------------------------*/
void loop()
{

}
