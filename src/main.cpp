
#include <stdio.h>
#include <stdint.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <MQTTClient.hpp>
#include <SDS011.hpp>

using namespace std;
using namespace std::chrono_literals;


int main()
{
	SDS011* sensor = new SDS011("/dev/ttyUSB1");
	sensor->wakeup();

	float p25, p10;
	uint16_t id;
	while (true)
	{
		if (sensor->read(&p25, &p10, &id))
		{
            printf("Sensor #%d: PM2.5=%f, PM10=%f\n", id, p25, p10);
		}
		else
		{
		    printf("Read failed. Sorry :-(\n");
		}
		this_thread::sleep_for(1s);
	}

	delete sensor;
	return 0;
}
