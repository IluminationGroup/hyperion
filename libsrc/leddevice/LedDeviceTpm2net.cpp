// STL includes
#include <cstring>
#include <cstdio>
#include <iostream>

// Boost Lib include for UDP mgt
#include "boost/asio.hpp"

 using namespace boost::asio;

// Local-Hyperion includes
#include "LedDeviceTpm2net.h"


LedDeviceTpm2net::LedDeviceTpm2net(const std::string & output, uint16_t port) :
		_port(port)
{
	// split output by space
	std::stringstream ss(output+' ');
    std::string item;
    while(std::getline(ss, item, ' ')) 
    {
        _output_ip.push_back(item);
				
    }
    
}

LedDeviceTpm2net::~LedDeviceTpm2net()
{
	// empty
}


int LedDeviceTpm2net::write(const std::vector<ColorRgb> & ledValues)
{

	/* LED BUFFER */
		if (_ledBuffer.size() == 0)
		{
				_ledBuffer.resize(7 + 3*ledValues.size());
				_ledBuffer[0] = 0x9c; // block-start byte TPM.NET
				_ledBuffer[1] = 0xDA;
				_ledBuffer[2] = ((3 * ledValues.size()) >> 8) & 0xFF; // frame size high byte
				_ledBuffer[3] = (3 * ledValues.size()) & 0xFF; // frame size low byte
				_ledBuffer[4] = 1; // packets number
				_ledBuffer[5] = 1; // Number of packets 
				_ledBuffer.back() = 0x36; // block-end byte
		}

		// write data
		memcpy(6 + _ledBuffer.data(), ledValues.data() /*Max 1,490 bytes*/, ledValues.size() * 3);
	/* EOF LED BUFFER */

	/* UDP */
	io_service io_service;
	ip::udp::socket socket(io_service);
	ip::udp::endpoint remote_endpoint;
	boost::system::error_code err;

	// Broadcast mode (slower)
	/*
		socket.open(ip::udp::v4());
		 socket.set_option(ip::udp::socket::reuse_address(true));
		socket.set_option(socket_base::broadcast(true));

		ip::udp::endpoint senderEndpoint(ip::address_v4::broadcast(), 65506);            

		socket.send_to(buffer(_ledBuffer.data(), _ledBuffer.size()), senderEndpoint, 0, err);
		socket.close();
		
	*/
	
	//Loop thru all dest
	for ( int i = 0; i < _output_ip.size(); ++i) {
		 
		/* Simple send */
		socket.open(ip::udp::v4());
		remote_endpoint = ip::udp::endpoint(ip::address::from_string(_output_ip[i]), _port);
		socket.send_to(buffer(_ledBuffer.data(), _ledBuffer.size()), remote_endpoint, 0, err);
		//std::cout << _output_ip[i] << std::endl;
		socket.close();
		/* Eof simple send */
	}
	
	/* eof UDP */
	
	return 0;
}

int LedDeviceTpm2net::switchOff()
{
	/* LED BUFFER */
	memset(6 + _ledBuffer.data(), 0, _ledBuffer.size() - 5);
	/* EOF LED BUFFER */
	return 0;
}
