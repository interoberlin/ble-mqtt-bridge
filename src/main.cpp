
#include <stdio.h>
#include <iostream>
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
	SDS011* sensor = new SDS011("/dev/ttyUSB0");
	sensor->wakeup();

	uint16_t id = 0;

	cout << "Waiting for sensor ID..." << endl << flush;
	while (id == 0)
	{
	    sensor->read(NULL, NULL, &id);
	    this_thread::sleep_for(1s);
	}
	cout << "Got sensor " << hex << id << "." << endl << flush;

	char topic[42];

	snprintf(topic, sizeof(topic), "dustsensor/%x/pm10", id);
	MQTTClient* mqtt10 = new MQTTClient(
                            "dustsensord",
                            topic,
                            "192.168.10.239",
                            1883
                            );

    snprintf(topic, sizeof(topic), "dustsensor/%x/pm2.5", id);
    MQTTClient* mqtt25 = new MQTTClient(
                            "dustsensord",
                            topic,
                            "192.168.10.239",
                            1883
                            );

    float p25, p10;
	while (true)
	{
		if (sensor->read(&p25, &p10, &id))
		{
            printf("Sensor %x: PM2.5=%f, PM10=%f\n", id, p25, p10);
            mqtt10->sendFloat(&p10);
            mqtt25->sendFloat(&p25);
		}
		else
		{
		    printf("Read failed. Sorry :-(\n");
		}
		this_thread::sleep_for(1s);
	}

	delete sensor;
	delete mqtt10;
	delete mqtt25;
	return 0;
}
