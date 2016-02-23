#pragma once
//  cmake -DENABLE_DISPMANX=OFF -DENABLE_SPIDEV=OFF -DENABLE_X11=OFF -DENABLE_V4L2=OFF -DENABLE_PROTOBUF=OFF .

// STL includes0
#include <fstream>
#include <string>
#include "boost/asio.hpp"

// Leddevice includes
#include <leddevice/LedDevice.h>

///
class LedDeviceTpm2net : public LedDevice
{
public:
	///
	/// Constructs the test-device, which opens an output stream to the file
	///
	LedDeviceTpm2net(const std::string& output, uint16_t port);

	virtual ~LedDeviceTpm2net();

	///
	/// Writes the given led-color values to the output stream
	///
	/// @param ledValues The color-value per led
	///
	/// @return Zero on success else negative
	///
	virtual int write(const std::vector<ColorRgb> & ledValues);

	/// Switch the leds off
	virtual int switchOff();

private:
		
	/// The buffer containing the packed RGB values
	std::vector<uint8_t> _ledBuffer;
	
	/// The host of the master brick
	std::vector<std::string> _output_ip;

	/// The port of the master brick
	const uint16_t _port;

};
