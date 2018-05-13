
#include <stdint.h>
#include <string>
#include <iostream>
#include "Joystick.hpp"
#include "MQTTClient.hpp"
#include "Steering.hpp"

using namespace std;

string devicePath = "/dev/input/js0";
string mqtt_id = "joystick-mqtt-service";
string mqtt_topic = "joystick/cyborg3d/";
string mqtt_host = "localhost";
int mqtt_port = 1883;


int main()
{
    // Create joystick object
    Joystick* joystick = new Joystick();
    if (!joystick->open(devicePath.c_str()))
    {
        cerr << "Fatal: Failed to open joystick device file " << devicePath << endl;
        exit(1);
    }

    // Create MQTT client
    MQTTClient* mqtt = new MQTTClient(
                            mqtt_id,
                            mqtt_topic,
                            mqtt_host,
                            mqtt_port
                            );

    // Create event processor
    Steering* steering = new Steering();
    steering->registerMQTTClient(mqtt);
    joystick->registerEventReceiver(steering);

    // Infinite event loop
    pause();

    delete steering;
    delete joystick;
    delete mqtt;
    return 0;
}
