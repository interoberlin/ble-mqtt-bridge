
#include <stdio.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <MQTTClient.hpp>
#include <SDS011.hpp>

using namespace std;
using namespace std::chrono_literals;


int main()
{
	SDS011* sensor = new SDS011("/dev/ttyUSB0");
	sensor->wakeup();

	float p25, p10;
	while (true)
	{
		sensor->read(&p25, &p10);
		printf("Values: PM2.5=%f, PM10=%f\n", p25, p10);
		this_thread::sleep_for(1s);
	}

	delete sensor;
	return 0;
}
