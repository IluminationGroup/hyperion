// STL includes
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>

// Local-Hyperion includes
#include "LedDeviceTpm2net.h"


LedDeviceTpm2net::LedDeviceTpm2net(const std::string & output, uint16_t port) :
		_port(port), _socket(this)
{
	// split output by space
	std::stringstream ss(output+' ');
	std::string item;
	while(std::getline(ss, item, ' ')) 
		_output_ip << item.c_str();
}

LedDeviceTpm2net::~LedDeviceTpm2net()
{
}


int LedDeviceTpm2net::write(const std::vector<ColorRgb> & ledValues)
{
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
	
	QByteArray datagram((char*)(_ledBuffer.data()), (int)_ledBuffer.size() );

	for ( int i = 0; i < _output_ip.size(); ++i) {
		std::cout << (int)_ledBuffer.size() << " " << (int)_ledBuffer[7] << " " << (int)_ledBuffer[8] << " " << (int)_ledBuffer[9] << std::endl;
		_socket.connectToHost(QHostAddress(_output_ip[i]),_port);
		_socket.write(datagram);
		_socket.close();
	}

	return 0;
}

int LedDeviceTpm2net::switchOff()
{
	memset(6 + _ledBuffer.data(), 0, _ledBuffer.size() - 5);
	return 0;
}
