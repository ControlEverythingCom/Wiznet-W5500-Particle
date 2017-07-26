/*
 modified 12 Aug 2013
 by Soohwan Kim (suhwan@wiznet.co.kr)
 */
#include "Ethernet.h"
#include "Dhcp.h"

// XXX: don't make assumptions about the value of MAX_SOCK_NUM.
uint8_t EthernetClass::_state[MAX_SOCK_NUM] = { 0, };
uint16_t EthernetClass::_server_port[MAX_SOCK_NUM] = { 0, };



#if defined(WIZ550io_WITH_MACADDRESS)
int EthernetClass::begin(void)
{
	byte mac_address[6] ={0,};
	_dhcp = new DhcpClass();

	// Initialise the basic info
	W5100.init();
	W5100.setIPAddress(IPAddress(0,0,0,0).raw_address());
	W5100.getMACAddress(mac_address);

	// Now try to get our config info from a DHCP server
	int ret = _dhcp->beginWithDHCP(mac_address);
	if(ret == 1)
	{
		// We've successfully found a DHCP server and got our configuration info, so set things
		// accordingly
		W5100.setIPAddress(_dhcp->getLocalIp().raw_address());
		W5100.setGatewayIp(_dhcp->getGatewayIp().raw_address());
		W5100.setSubnetMask(_dhcp->getSubnetMask().raw_address());
		_dnsServerAddress = _dhcp->getDnsServerIp();
	}

	return ret;
}

void EthernetClass::begin(IPAddress local_ip)
{
	// Assume the DNS server will be the machine on the same network as the local IP
	// but with last octet being '1'
	IPAddress dns_server = local_ip;
	dns_server[3] = 1;
	begin(local_ip, dns_server);
}

void EthernetClass::begin(IPAddress local_ip, IPAddress dns_server)
{
	// Assume the gateway will be the machine on the same network as the local IP
	// but with last octet being '1'
	IPAddress gateway = local_ip;
	gateway[3] = 1;
	begin(local_ip, dns_server, gateway);
}

void EthernetClass::begin(IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
	IPAddress subnet(255, 255, 255, 0);
	begin(local_ip, dns_server, gateway, subnet);
}

void EthernetClass::begin(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
	W5100.init();
	W5100.setIPAddress(local_ip.raw_address());
	W5100.setGatewayIp(gateway.raw_address());
	W5100.setSubnetMask(subnet.raw_address());
	_dnsServerAddress = dns_server;
}
#else
int EthernetClass::begin(uint8_t *mac_address)
{
	_dhcp = new DhcpClass();

	// Initialise the basic info
	W5100.init();
	W5100.setMACAddress(mac_address);
	byte* ip = (byte*)&IPAddress(0,0,0,0).raw().ipv4;
	W5100.setIPAddress(ip);

	// Now try to get our config info from a DHCP server
	int ret = _dhcp->beginWithDHCP(mac_address);
	if(ret == 1)
	{
		// We've successfully found a DHCP server and got our configuration info, so set things
		// accordingly
		byte* lA = (byte*)&_dhcp->getLocalIp().raw().ipv4;
		byte* gA = (byte*)&_dhcp->getGatewayIp().raw().ipv4;
		byte* sA = (byte*)&_dhcp->getSubnetMask().raw().ipv4;
		W5100.setIPAddress(lA);
		W5100.setGatewayIp(gA);
		W5100.setSubnetMask(sA);
		_dnsServerAddress = _dhcp->getDnsServerIp();
	}

	return ret;
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip)
{
	// Assume the DNS server will be the machine on the same network as the local IP
	// but with last octet being '1'
	IPAddress dns_server = local_ip;
	dns_server[3] = 1;
	begin(mac_address, local_ip, dns_server);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server)
{
	// Assume the gateway will be the machine on the same network as the local IP
	// but with last octet being '1'
	IPAddress gateway = local_ip;
	gateway[3] = 1;
	begin(mac_address, local_ip, dns_server, gateway);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
	IPAddress subnet(255, 255, 255, 0);
	begin(mac_address, local_ip, dns_server, gateway, subnet);
}

void EthernetClass::begin(uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
	W5100.init();
	W5100.setMACAddress(mac);
	byte* lA = (byte*)&local_ip.raw().ipv4;
	byte* gA = (byte*)&gateway.raw().ipv4;
	byte* sA = (byte*)&subnet.raw().ipv4;
	W5100.setIPAddress(lA);
	W5100.setGatewayIp(gA);
	W5100.setSubnetMask(sA);
	_dnsServerAddress = dns_server;
}

#endif

int EthernetClass::maintain(){
	int rc = DHCP_CHECK_NONE;
	if(_dhcp != NULL){
		//we have a pointer to dhcp, use it
		rc = _dhcp->checkLease();

		byte* lA = (byte*)&_dhcp->getLocalIp().raw().ipv4;
		byte* gA = (byte*)&_dhcp->getGatewayIp().raw().ipv4;
		byte* sA = (byte*)&_dhcp->getSubnetMask().raw().ipv4;
		W5100.setIPAddress(lA);
		W5100.setGatewayIp(gA);
		W5100.setSubnetMask(sA);

		switch ( rc ){
		case DHCP_CHECK_NONE:
			//nothing done
			break;
		case DHCP_CHECK_RENEW_OK:
		case DHCP_CHECK_REBIND_OK:
			//we might have got a new IP.



			W5100.setIPAddress(lA);
			W5100.setGatewayIp(gA);
			W5100.setSubnetMask(sA);
			_dnsServerAddress = _dhcp->getDnsServerIp();
			break;
		default:
			//this is actually a error, it will retry though
			break;
		}
	}
	return rc;
}

IPAddress EthernetClass::localIP()
{
	IPAddress ret;
	byte* retBytes = (byte*)&ret.raw().ipv4;
	W5100.getIPAddress(retBytes);
	return ret;
}

IPAddress EthernetClass::subnetMask()
{
	IPAddress ret;
	byte* retBytes = (byte*)&ret.raw().ipv4;
	W5100.getSubnetMask(retBytes);
	return ret;
}

IPAddress EthernetClass::gatewayIP()
{
	IPAddress ret;
	byte* retBytes = (byte*)&ret.raw().ipv4;
	W5100.getGatewayIp(retBytes);
	return ret;
}

IPAddress EthernetClass::dnsServerIP()
{
	return _dnsServerAddress;
}

EthernetClass Ethernet;
