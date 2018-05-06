// SerialPort.h

#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

class SerialPort
{

public:
	SerialPort() { };
	~SerialPort();

	bool Open(const char* = "/dev/ttyUSB0", int nBaud = 9600 );
	bool Close( void );

	int Read( void*, int );
	int Write( const void*, int );
	void Flush();

protected:
	int fd; // file descriptoy
};

#endif
