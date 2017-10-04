/* Includes ------------------------------------------------------------------*/
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Set this to the port number the server should listen on for TCP socket connections
int serverPort = 5000;
EthernetServer server(serverPort);

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
  //Start the server
  server.begin();
	// print your local IP address:
	Serial.printf("Server now listening on port %i at IP: ", serverPort);
	Serial.println(Ethernet.localIP());
}

/* This function loops forever --------------------------------------------*/
void loop()
{
  //Check to see if a client is connected
  EthernetClient client = server.available();
  if(client){
    Serial.println("Client connected");
    while(client.connected()){
      //Check for data from client
      if(client.available() > 0){
        //Print message from client to serial terminal
        Serial.print((char)client.read());
      }
    }
    Serial.println("Client Disconnected");
  }
}
