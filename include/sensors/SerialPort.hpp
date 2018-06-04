// SerialPort.h

#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <stdint.h>

class SerialPort
{

public:
	SerialPort() { };
	~SerialPort();

	bool open(const char* = "/dev/ttyUSB0", int nBaud = 9600 );
	bool close( void );

	int read( void*, int );
	int write( const void*, int );
	int write( uint8_t );
	void flush();

protected:
	int fd; // file descriptoy
};

#endif
