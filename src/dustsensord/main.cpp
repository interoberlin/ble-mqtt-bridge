
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include <endpoints/MQTTClient.hpp>
#include <sensors/SDS011.hpp>

//  https://github.com/emilk/loguru.git
#define LOGURU_IMPLEMENTATION 1
#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>


using namespace std;
using namespace std::chrono_literals;

int main( int argc, char* argv[] )
{
	// command line options local vars
	char device_name[64] = "/dev/ttyUSB0";
	char mqtt_broker_host[64] = "buttercup-cumbersnatch.local";
	int  mqtt_broker_port = 1883;

    int option_char;
    char* end = NULL;
    long temp_val;

    loguru::init(argc, argv);

    while ((option_char = getopt (argc, argv, "D:p:h:")) != EOF) {
        switch (option_char) {
            case 'D': {
                snprintf(device_name, sizeof(device_name), "%s", optarg);
            } break;
            case 'p': {
                end = 0;
                errno = 0;
                temp_val = strtol(optarg, &end, 10);
                if (end != optarg && errno != ERANGE && temp_val >= INT_MIN && temp_val <= INT_MAX) {
                    mqtt_broker_port = (int) temp_val;
                } else {
                    perror(strerror(errno));
                }                 
            } break;
            case 'h': {
                snprintf(mqtt_broker_host, sizeof(mqtt_broker_host), "%s", optarg);
            } break;
            case '?':
                 cerr << "usage: " << argv[0] << " [D<val>v<val>p<val>h<val>]" << endl; break;
            break;
        } // switch
    } // while ((option_char ...



	SDS011* sensor = new SDS011(device_name);
	sensor->wakeup();

	uint16_t id = 0;

	LOG_S(INFO) << "Waiting for sensor ID...";
    
	while (id == 0)
	{
	    sensor->read(NULL, NULL, &id);
	    this_thread::sleep_for(1s);
	}
	
    
    LOG_S(INFO) << "Got sensor " << hex << id << ".";
    
	char topic_pm10[42];
	snprintf(topic_pm10, sizeof(topic_pm10), "dustsensor/%x/pm10", id);
	LOG_S(INFO) << "Topic 1 is \"" << topic_pm10 << "\".";
	
    MQTTClient* mqtt10 = new MQTTClient(
                            "dustsensord",
                            (char*) topic_pm10,
                            mqtt_broker_host,
                            mqtt_broker_port
                            );
    mqtt10->setThreadName(topic_pm10);

    char topic_pm25[42];
    snprintf(topic_pm25, sizeof(topic_pm25), "dustsensor/%x/pm2.5", id);
    LOG_S(INFO) << "Topic 2 is \"" << topic_pm25 << "\"." << endl << flush;

    MQTTClient* mqtt25 = new MQTTClient(
                            "dustsensord",
                            (char*) topic_pm25,
                            mqtt_broker_host,
                            mqtt_broker_port
                            );
    mqtt25->setThreadName(topic_pm25);

    float p25, p10;
	while (true)
	{
		if (sensor->read(&p25, &p10, &id))
		{
            LOG_S(INFO) << "Sensor " << id << ": PM2.5=" << p25 << " PM10=" << p10;

            mqtt10->sendMessage(&p10);
            // this_thread::sleep_for(0.5s);
            mqtt25->sendMessage(&p25);
            this_thread::sleep_for(1s);
		}
		else
		{
		    LOG_S(ERROR) << "Sensor Read failed. Sorry :-(";
            this_thread::sleep_for(1s);
		}
	}

	delete sensor;
	delete mqtt10;
	delete mqtt25;
	return 0;
}
