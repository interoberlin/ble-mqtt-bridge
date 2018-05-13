
#include <stdint.h>
#include <string>
#include <iostream>
#include "Joystick.hpp"
#include "MQTTClient.hpp"

using namespace std;

string devicePath = "/dev/input/js0";
string mqtt_id = "joystick-mqtt-service";
string mqtt_topic = "joystick/cyborg3d/";
string mqtt_host = "localhost";
int mqtt_port = 8833;


int main()
{
    Joystick* joystick = new Joystick();
    if (!joystick->open(devicePath.c_str()))
    {
        cerr << "Fatal: Failed to open joystick device file " << devicePath << endl;
        exit(1);
    }

    MQTTClient* mqtt = new MQTTClient(
                            mqtt_id,
                            mqtt_topic,
                            mqtt_host,
                            mqtt_port
                            );

    // Infinite event loop
    while (1)
    {
        joystick->read();
    }

    delete joystick;
    return 0;
}
