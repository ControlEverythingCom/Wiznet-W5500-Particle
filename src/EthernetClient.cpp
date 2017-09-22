#include "utility/w5500.h"
#include "utility/socket.h"

extern "C" {
#include "string.h"
}

#include "Arduino.h"

#include "Ethernet.h"
#include "EthernetClient.h"
#include "EthernetServer.h"
#include "Dns.h"

uint16_t EthernetClient::_srcport = 1024;

EthernetClient::EthernetClient() : _sock(MAX_SOCK_NUM) {
}

EthernetClient::EthernetClient(uint8_t sock) : _sock(sock) {
}

int EthernetClient::connect(const char* host, uint16_t port, network_interface_t) {
	// Look up the host first
	int ret = 0;
	DNSClient dns;
	IPAddress remote_addr;

	dns.begin(Ethernet.dnsServerIP());
	ret = dns.getHostByName(host, remote_addr);
	if (ret == 1) {
		return connect(remote_addr, port, 1);
	} else {
		return ret;
	}
}

int EthernetClient::connect(IPAddress ip, uint16_t port, network_interface_t) {
	Serial.println("in connect function");
	if (_sock != MAX_SOCK_NUM){
		Serial.println("_sock != MAX_SOCK_NUM");
		return 0;
	}
	Serial.println("Here");

	for (int i = 0; i < MAX_SOCK_NUM; i++) {
		Serial.println("Inside For Loop");
		uint8_t s = w5500.readSnSR(i);
		if (s == SnSR::CLOSED || s == SnSR::FIN_WAIT || s == SnSR::CLOSE_WAIT) {
			_sock = i;
			break;
		}
	}
	Serial.println("After for loop in connect");

	if (_sock == MAX_SOCK_NUM){
		Serial.println("_sock != MAX_SOCK_NUM, after loop");
		return 0;
	}

	_srcport++;
	if (_srcport == 0){
		_srcport = 1024;
	}
	socket(_sock, SnMR::TCP, _srcport, 0);

	delay(1000);

	byte* lA = (byte*)&ip.raw().ipv4;
	for (int i = 0; i < (4 / 2); i++) {
		float temporary = lA[i];                 // temporary wasn't declared
		lA[i] = lA[(4 - 1) - i];
		lA[(4 - 1) - i] = temporary;
	}
	Serial.printf("Attempting to connect to: %i.%i.%i.%i on port %i", lA[3], lA[2], lA[1], lA[0], port);
	Serial.printf("_sock = %i\n", _sock);
	if (!::connect(_sock, lA, port)) {
		_sock = MAX_SOCK_NUM;
		return 0;
	}

	uint8_t ipArray[4];
	w5500.readSnDIPR(_sock, ipArray);
	uint16_t setPort = w5500.readSnDPORT(_sock);
	Serial.printf("_sock IP and port: %i.%i.%i.%i on port %i", ipArray[3], ipArray[2], ipArray[1], ipArray[0], setPort);

	while (status() != SnSR::ESTABLISHED) {
		delay(10);
		uint8_t state = status();
		if (state == SnSR::CLOSED) {
			_sock = MAX_SOCK_NUM;
			Serial.println("Got here");
			return 0;
		}else{
		}
	}

	return 1;
}

size_t EthernetClient::write(uint8_t b) {
	return write(&b, 1);
}

size_t EthernetClient::write(const uint8_t *buf, size_t size) {
	if (_sock == MAX_SOCK_NUM) {
		setWriteError();
		return 0;
	}
	if (!send(_sock, buf, size)) {
		setWriteError();
		return 0;
	}
	return size;
}

int EthernetClient::available() {
	if (_sock != MAX_SOCK_NUM){
		return w5500.getRXReceivedSize(_sock);
	}
	return 0;
}

int EthernetClient::read() {
	uint8_t b;
	if ( recv(_sock, &b, 1) > 0 )
	{
		// recv worked
		return b;
	}
	else
	{
		// No data available
		return -1;
	}
}

int EthernetClient::read(uint8_t *buf, size_t size) {
	return recv(_sock, buf, size);
}

int EthernetClient::peek() {
	uint8_t b;
	// Unlike recv, peek doesn't check to see if there's any data available, so we must
	if (!available()){
		return -1;
	}
	::peek(_sock, &b);
	return b;
}

void EthernetClient::flush() {
	::flush(_sock);
}

void EthernetClient::stop() {
	if (_sock == MAX_SOCK_NUM){
		return;
	}

	// attempt to close the connection gracefully (send a FIN to other side)
	disconnect(_sock);
	unsigned long start = millis();

	// wait a second for the connection to close
	while (status() != SnSR::CLOSED && millis() - start < 1000){
		delay(1);
	}

	// if it hasn't closed, close it forcefully
	if (status() != SnSR::CLOSED){
		close(_sock);
	}

	EthernetClass::_server_port[_sock] = 0;
	_sock = MAX_SOCK_NUM;
}

uint8_t EthernetClient::connected() {
	if (_sock == MAX_SOCK_NUM) return 0;

	uint8_t s = status();
	return !(s == SnSR::LISTEN || s == SnSR::CLOSED || s == SnSR::FIN_WAIT ||
			(s == SnSR::CLOSE_WAIT && !available()));
}

uint8_t EthernetClient::status() {
	if (_sock == MAX_SOCK_NUM){
		return SnSR::CLOSED;
	}
	return w5500.readSnSR(_sock);
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.

EthernetClient::operator bool() {
	return _sock != MAX_SOCK_NUM;
}

bool EthernetClient::operator==(const EthernetClient& rhs) {
	return _sock == rhs._sock && _sock != MAX_SOCK_NUM && rhs._sock != MAX_SOCK_NUM;
}
