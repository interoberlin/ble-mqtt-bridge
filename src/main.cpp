
#include <stdint.h>
#include <string>
#include <iostream>
#include "Joystick.hpp"
#include "MQTTClient.hpp"
#include "Steering.hpp"

using namespace std;


string devicePath = "/dev/input/js0";
string mqtt_id = "joystick-mqtt-service";
string mqtt_topic = "joystick/cyborg3d";
string mqtt_host = "localhost";
int mqtt_port = 1883;
bool daemonize = false;


void parseArgs(int argc, char** argv)
{
    int c;
    while ((c = getopt(argc, argv, "?f:n:t:h:p:d")) != -1)
    {
        switch (c)
        {
            case 'f':
                devicePath = string(optarg);
                break;
            case 'n':
                mqtt_id = string(optarg);
                break;
            case 't':
                mqtt_topic = string(optarg);
                break;
            case 'h':
                mqtt_host = string(optarg);
                break;
            case 'p':
                mqtt_port = atoi(optarg);
                break;
            case 'd':
                daemonize = true;
                break;
            case '?':
                cout << "-f <path to joystick device file>" << endl;
                cout << "\tDefault: " << devicePath << endl;
                cout << "-n <MQTT client name>" << endl;
                cout << "\tDefault: " << mqtt_id << endl;
                cout << "-t <MQTT topic>" << endl;
                cout << "\tDefault: " << mqtt_topic << endl;
                cout << "-h <MQTT broker host>" << endl;
                cout << "\tDefault: " << mqtt_host << endl;
                cout << "-p <MQTT broker port>" << endl;
                cout << "\tDefault: " << mqtt_port << endl;
                cout << "-d (daemonize)" << endl;
                cout << "\tDefault: " << (daemonize ? "on" : "off") << endl;
                cout << flush;
                exit(0);

//                if (optopt == 'c')
//                    fprintf(stderr, "Option -%c requires an argument.\n",
//                            optopt);
//                else if (isprint(optopt))
//                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
//                else
//                    fprintf(stderr, "Unknown option character `\\x%x'.\n",
//                            optopt);
                exit(0);
            default:
                abort();
        }
    }
}


int main(int argc, char** argv)
{
    parseArgs(argc, argv);

    // Create joystick object
    Joystick* joystick = new Joystick();
    if (!joystick->open(devicePath.c_str()))
    {
        cerr << "Fatal: Failed to open joystick device file " << devicePath << endl;
        exit(1);
    }

    // Create MQTT client
    MQTTClient* mqtt = new MQTTClient(
                            mqtt_id.c_str(),
                            mqtt_topic.c_str(),
                            mqtt_host.c_str(),
                            mqtt_port
                            );

    // Create event processor
    Steering* steering = new Steering();
    steering->registerMQTTClient(mqtt);
    joystick->registerEventReceiver(steering);

    // Infinite event loop
//    pause();

    while (true)
    {
        steering->process();
        this_thread::sleep_for(200ms);
    }

    delete steering;
    delete joystick;
    delete mqtt;
    return 0;
}
