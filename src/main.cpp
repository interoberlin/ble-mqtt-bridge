
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
#include <MQTTClient.hpp>
#include <SDS011.hpp>

using namespace std;
using namespace std::chrono_literals;

// command line option global vars
typedef enum { DEBUG_NONE=0, DEBUG_NORMAL, DEBUG_MORE, DEBUG_MOST, DEBUG_ALL=9} debug_flags;
debug_flags debug_flag = DEBUG_NONE;


int main( int argc, char* argv[] )
{
	// command line options local vars
	char device_name[64] = "/dev/ttyUSB0";
	char mqtt_broker_host[64] = "10.42.0.1";
	int  mqtt_broker_port = 1883;

    int option_char;
    char* end = NULL;
    long temp_val;

    while ((option_char = getopt (argc, argv, "D:d:p:h:")) != EOF) {
        switch (option_char) {
            case 'D': {
                snprintf(device_name, sizeof(device_name), "%s", optarg);
            } break;
            case 'd': {
                end = 0;
                errno = 0;
                temp_val = strtol(optarg, &end, 10);
                if (end != optarg && errno != ERANGE && temp_val >= DEBUG_NONE && temp_val <= DEBUG_ALL) {
                    debug_flag = (debug_flags) temp_val;
                }
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
                 cerr << "usage: " << argv[0] << " [D<val>d<val>p<val>h<val>]" << endl; break;
            break;
        } // switch
    } // while ((option_char ...



	SDS011* sensor = new SDS011(device_name);
	sensor->wakeup();

	uint16_t id = 0;

	if (debug_flag > DEBUG_MORE) 
    {
         cout << "Waiting for sensor ID..." << endl << flush;
    }

	while (id == 0)
	{
	    sensor->read(NULL, NULL, &id);
	    this_thread::sleep_for(1s);
	}
	
    if (debug_flag > DEBUG_MORE)
    {
	    cout << "Got sensor " << hex << id << "." << endl << flush;
    }

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
            if (debug_flag > DEBUG_NORMAL) {
                 cout << "Sensor " << id << ": PM2.5=" << p25 << " PM10=" << p10 << endl << flush;
            }
            mqtt10->sendFloat(&p10);
            this_thread::sleep_for(0.5s);
            mqtt25->sendFloat(&p25);
            this_thread::sleep_for(0.5s);
		}
		else
		{
		    cerr << "Sensor Read failed. Sorry :-(";
            this_thread::sleep_for(1s);
		}
	}

	delete sensor;
	delete mqtt10;
	delete mqtt25;
	return 0;
}
