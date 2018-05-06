
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

const char* mqtt_broker_host = "192.168.0.250";
const int mqtt_broker_port = 1883;


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

	char topic_pm10[42];
	snprintf(topic_pm10, sizeof(topic_pm10), "dustsensor/%x/pm10", id);
	cout << "Topic 1 is \"" << topic_pm10 << "\"." << endl << flush;
	MQTTClient* mqtt10 = new MQTTClient(
                            "dustsensord",
                            (char*) topic_pm10,
                            mqtt_broker_host,
                            mqtt_broker_port
                            );

    char topic_pm25[42];
    snprintf(topic_pm25, sizeof(topic_pm25), "dustsensor/%x/pm2.5", id);
    cout << "Topic 2 is \"" << topic_pm25 << "\"." << endl << flush;
    MQTTClient* mqtt25 = new MQTTClient(
                            "dustsensord",
                            (char*) topic_pm25,
                            mqtt_broker_host,
                            mqtt_broker_port
                            );

    float p25, p10;
	while (true)
	{
		if (sensor->read(&p25, &p10, &id))
		{
            printf("Sensor %x: PM2.5=%f, PM10=%f\n", id, p25, p10);
            mqtt10->sendFloat(&p10);
            this_thread::sleep_for(0.5s);
            mqtt25->sendFloat(&p25);
            this_thread::sleep_for(0.5s);
		}
		else
		{
		    printf("Read failed. Sorry :-(\n");
            this_thread::sleep_for(1s);
		}
	}

	delete sensor;
	delete mqtt10;
	delete mqtt25;
	return 0;
}
