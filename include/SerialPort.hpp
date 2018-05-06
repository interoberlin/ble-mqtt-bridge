// SerialPort.h

#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <stdint.h>

class SerialPort
{

public:
	SerialPort() { };
	~SerialPort();

	bool Open(const char* = "/dev/ttyUSB0", int nBaud = 9600 );
	bool Close( void );

	int Read( void*, int );
	int Write( const void*, int );
	int Write( uint8_t );
	void Flush();

protected:
	int fd; // file descriptoy
};

#endif
