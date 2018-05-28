#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>


//  https://github.com/emilk/loguru.git
#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>


#include "sensors/SerialPort.hpp"

SerialPort::~SerialPort()
{
	::close(fd);
}

bool SerialPort::open(const char* device, int nBaud )
{

	fd = ::open(device, O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd == -1 )	{
		LOG_S(ERROR) << "open_port: Unable to open device " << device << strerror(errno);

		return false;

	} else {

		/* set the other settings (in this case, 9600 8N1) */
		struct termios settings;
		tcgetattr(fd, &settings);

		cfsetospeed(&settings, nBaud); /* baud rate */
		// settings.c_cflag &= ~PARENB; /* no parity */
		// settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
		// settings.c_cflag &= ~CSIZE;
		// settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
		// settings.c_lflag = ICANON; /* canonical mode */
		// settings.c_oflag &= ~OPOST; /* raw output */

		tcsetattr(fd, TCSANOW, &settings); /* apply the settings */
		tcflush(fd, TCOFLUSH);

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
		LOG_S(ERROR) << "read: unable to read " << strerror(errno);
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
		LOG_S(ERROR) << "write: unable to write " << strerror(errno);
		return -1;
	}

	return wr;
}

void SerialPort::flush()
{
	::fsync(fd);
}
