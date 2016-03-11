#pragma once

// STL includes
#include <fstream>
#include <string>
#include <QUdpSocket>
#include <QStringList>
// Leddevice includes
#include <leddevice/LedDevice.h>

///
class LedDeviceTpm2net :public QObject,  public LedDevice
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
	QStringList _output_ip;

	/// The port of the master brick
	const uint16_t _port;
	QUdpSocket _socket;
};
