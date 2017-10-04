/* Includes ------------------------------------------------------------------*/
#include "application.h"
#include <Ethernet.h>
#include <EthernetClient.h>

//SYSTEM_MODE(MANUAL);

//Mac address to use for Ethernt interface connection.
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

bool success = false;

EthernetUDP Udp;

//Message to broadcast on network.
const char message[14] = "Hello World!\n";

//IP to send the broadcast on 255x4 sends to every device on the network.
IPAddress broadcast = IPAddress(255,255,255,255);
//Port number to broadcast via UDP on.
int broadcastPort = 55555;

//Interval at which to broadcast our message to the network via UDP.
unsigned long broadcastInterval = 3000;
//Time of last broadcast.
unsigned long lastBroadcast = 0;


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
	//Check to see if it's time to send another broadcast or not.
	if(millis() > lastBroadcast + broadcastInterval){
		Serial.println("Broadcasting message");
		lastBroadcast = millis();
		//Make sure there is not a previous UDP socket open
		Udp.stop();
		//Attempt to open a UDP socket on the target port.
		if(Udp.begin(broadcastPort) == 0){
			Serial.println("Could not open UDP socket");
		}else{
			Serial.println("UDP socket Open");
			Udp.beginPacket(broadcast, broadcastPort);
			//Send the message
			Udp.write(message);
			//Finish.
			Udp.endPacket();
		}
	}
}
