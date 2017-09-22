/* Includes ------------------------------------------------------------------*/
#include "application.h"
#include <Ethernet.h>
#include <EthernetClient.h>

//SYSTEM_MODE(MANUAL);

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

IPAddress ip(192,168,1,70);

IPAddress serverIP(192,168,1,53);
int serverPort = 4000;

int safeModeButton = D4;
void enterSafeMode();


/* This function is called once at start up ----------------------------------*/
void setup()
{
	//Wait for user to get Serial terminal open for monitoring.
	delay(5000);

	// start the Ethernet connection:
	Ethernet.begin(mac, ip);

	// print your local IP address:
	Serial.print("My IP address: ");
	for (byte thisByte = 0; thisByte < 4; thisByte++) {
		// print the value of each byte of the IP address:
		Serial.print(Ethernet.localIP()[thisByte], DEC);
		Serial.print(".");
	}
	Serial.println();

	pinMode(safeModeButton, INPUT_PULLUP);
	attachInterrupt(safeModeButton, enterSafeMode, FALLING);

	if(client.connect(serverIP, 4000, 1)){
		Serial.println("Connected to Server");
		client.println("Hello");
	}else{
		Serial.println("Could not connect to server");
	}
}

/* This function loops forever --------------------------------------------*/
void loop()
{

}

void enterSafeMode(){
	System.enterSafeMode();
}
