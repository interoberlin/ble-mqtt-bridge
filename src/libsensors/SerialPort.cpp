#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "sensors/SerialPort.hpp"

SerialPort::~SerialPort()
{
	::close(fd);
}

bool SerialPort::open(const char* device, int nBaud )
{

	fd = ::open(device, O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd == -1 )	{
		perror("open_port: Unable to open /dev/ttyS0 – ");

		return false;

	} else {
	
		fcntl(fd, F_SETFL,0);
		
		// printf("Port has been sucessfully opened and %d is the file description\n", fd);
	}

	return true;
}

bool SerialPort::close( void )
{
	::close(fd);
	return true;
}

int SerialPort::read(void* buf, int length)
{
	int rd;
	rd = ::read(fd, buf, length);
	if (rd < 0 ) {
		perror("read: unable to read – ");
		return -1;
	}
	return rd;
}

int SerialPort::write(uint8_t i)
{
	char buf[2];
	snprintf(buf, sizeof(buf), "%d", i);
	write(&buf, 1);
	return 1;
}

int SerialPort::write(const void* buf, int len)
{
	int wr;
	wr = ::write( fd, buf,len );

	if (wr < 0 ) {
		perror("write: unable to write – ");
		return -1;
	}

	return wr;
}

void SerialPort::flush()
{
	::fsync(fd);
}
