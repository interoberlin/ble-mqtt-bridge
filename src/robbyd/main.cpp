
#include <iostream>
#include <stddef.h>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "endpoints/BLEClient.hpp"
#include "MQTTClientRobby.hpp"

//  https://github.com/emilk/loguru.git
#define LOGURU_IMPLEMENTATION 1
#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

typedef enum
{
    DEBUG_NONE,
    DEBUG_MORE,
    DEBUG_ALL
} debug_flags;

using namespace std;
using namespace std::chrono_literals;

/*
 * TODO:
 * For now, the beacon configuration is hard-coded.
 * Later on, this can be out-sourced, e.g. to a configuration file.
 */
#define ROBBY_BEACON_ADDRESS           "78:A5:04:80:9D:EC"
#define ROBBY_UUID_SERVICE             "0000ffe0-0000-1000-8000-00805f9b34fb"
#define ROBBY_UUID_CHARACTERISTIC      "0000ffe1-0000-1000-8000-00805f9b34fb"
#define ROBBY_TOPIC_ALL                "joystick/cyborg3d/+"

MQTTClientRobby* mqtt_client = NULL;
BLEClient* ble_client = NULL;
debug_flags debug_flag = DEBUG_ALL;

void bridge_robby()
{
    mqtt_client = new MQTTClientRobby(
                            "robby-daemon",
                            ROBBY_TOPIC_ALL,
                            "localhost",
                            1883
                            );

    ble_client = new BLEClient(
                            BLEClientRole::WRITER,
                            ROBBY_BEACON_ADDRESS,
                            ROBBY_UUID_SERVICE,
                            ROBBY_UUID_CHARACTERISTIC
                            );
    mqtt_client->subscribe(NULL, ROBBY_TOPIC_ALL, 0);
    mqtt_client->registerOnMessageEventReceiver(ble_client);
}




int main()
{
    bridge_robby();

    // Sleep until a signal arrives
    pause();

    delete ble_client;
    delete mqtt_client;
}
