/* Includes ------------------------------------------------------------------*/
#include <Ethernet.h>
#include <EthernetClient.h>

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

IPAddress ip(192,168,1,70);

//Set this to the IP of the server you wish to connect to.
IPAddress serverIP(192,168,1,53);
//Set this to the port number the server application is listening on for TCP socket connections
int serverPort = 4000;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	// start the Ethernet connection and obtain an IP from the network DHCP server:
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
	if(!client.connected()){
		/*Attempt to connet to TCP server.  The third argument in this client.connect
		will always be 1 which specifies this is not over WiFi*/
		if(client.connect(serverIP, serverPort, 1)){
			Serial.println("Connected to Server");
			//Send a simple message to the server
			client.println("Hello");
		}else{
			Serial.println("Could not connect to server");
		}
	}
	//check for message from server and print them to the serial line.  Assuming string messages here.
	if(client.connected() && client.available() > 0){
		while(client.available() > 0){
			Serial.print((char)client.read());
		}
		Serial.println();
	}
}
